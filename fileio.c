#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileio.h"
#include "vdocerror.h"

#define crlf(x) (((x)=='\r') || ((x)=='\n'))

char lend_str[4][3]={"\n","\r","\n","\r\n"};

int slow_mygetc(MYFILE * f)
{
	size_t len;

	if (myfeof(f)){
		return EOF;
	}
		
	len=fread(f->buf,sizeof(char),BUFSIZE,f->f);
	f->pos +=len;
	f->ptr=f->buf;
	f->end=f->buf + ((len>0)?len:0);
	if (len!=BUFSIZE){
		f->eof=EOF;
	}
	if (myfeof(f)){
		return EOF;
	}
	f->ptr=f->buf;
	f->end=f->buf + len;
	return *(f->ptr)++;
}/*slow_mmygetc()*/

MYFILE * stropen(const char * str){
	MYFILE * f;
	size_t len;
	if (str==NULL){
		return NULL;
	}
	f=(MYFILE *) malloc (sizeof(MYFILE));
	if (f==NULL){
		vdoc_errno=VDOC_ERR_LIBC;
		return NULL;
	}
	len=strlen(str);
	f->buf=(unsigned char *)malloc(sizeof(unsigned char) * (len+1));
	if (f->buf==NULL){
		free(f);
		vdoc_errno=VDOC_ERR_LIBC;
		return NULL;	
	}
	strcpy((char *)f->buf,str);
	f->pos=len;
	f->f=NULL;
	f->ptr=f->buf;
	f->end=f->buf+len;
	f->eof=-1;
	return f;

}

MYFILE * myfopen(const char * filename,const char * mode)
{ 
	MYFILE * f;
	f=(MYFILE *) malloc (sizeof(MYFILE));
	if (f==NULL){
		vdoc_errno=VDOC_ERR_LIBC;
		return NULL;
	}
	f->buf=(unsigned char *)malloc(sizeof(unsigned char) * BUFSIZE);
	if (f->buf==NULL){
		free(f);
		vdoc_errno=VDOC_ERR_LIBC;
		return NULL;	
	}
	/*standardni vstup ...*/
	f->f=fopen(filename,mode);
	
	if (f->f==NULL)
	{
		free(f->buf);
		free(f);
		vdoc_errno=VDOC_ERR_LIBC;
		return NULL;
	}
	f->pos=0;
	f->ptr=f->buf+BUFSIZE;
	f->end=f->buf+BUFSIZE;
	f->eof=0;
	return f;

}/*END myfopen()*/


int myfclose(MYFILE *stream)
{
	int retval;
	retval=stream->f?fclose(stream->f):0;
	free(stream->buf);
	free(stream);
	if (retval==EOF){
		vdoc_errno=VDOC_ERR_LIBC;
		return -1;
	}
	return 0;
}/*END myfclose()*/

long myfseek(MYFILE *stream,long offset,int origin)
{
	if (stream->f==NULL){
		switch (origin){
			case SEEK_SET:
				stream->ptr=stream->buf+offset+1;
			break;
			case SEEK_END:
				stream->ptr=stream->end-offset;
			break;
			case SEEK_CUR:
				stream->ptr=stream->ptr+offset;
			break;
		}
		if (stream->ptr>=stream->end){
			return EOF;
		}
		return 0;
	}
	if (fseek(stream->f,offset,origin)!=EOF)
	{
		stream->pos=ftell(stream->f);
		stream->ptr=stream->buf+BUFSIZE;
		stream->end=stream->buf+BUFSIZE;
		stream->eof=0;
	}
	else
	{
		vdoc_errno=VDOC_ERR_LIBC;
		stream->eof=EOF;
	}
	return stream->eof;
}/*END myfseek()*/

/*
 cte radek po radku soubor f, vraci pozici ukazovatka v souboru pred nactenim
 radku, konec souboru je indikovan navratovou hodnotou EOF (-1), nacte len-1 znaku,
 posledni je ukoncovaci 0
 */
long myfgets(char * line,int len,MYFILE * f,int * eoln)
{
	long l = myftell(f);
	int lend = UNDEF;
	int llen;
/*	char * pom=line;*/
	--len;
	llen=len-1;
		
	if (len<=0 || myfeof(f))
	{
		*line=0;
		return EOF;
	}
	/*nacteni radku*/
	do
	{
		*line=mygetc(f);
		--len;++line;
	}
	while (len && !myfeof(f) &&  !crlf(*(line-1)));
	if (llen==len && myfeof(f))
	{
		f->eof=EOF;
		*(line-1)=0;
		return EOF;
	}


	switch(*(line-1)){
		case '\n':
			lend=LF;
			*(line-1)=0;
		break;
		case '\r':
			*(line-1)='\n';
			*(line-1)=0;
			if (mygetc(f)=='\n')
			{
				lend=CRLF;
			}
			else
			{
				lend=CR;
				myungetc(f);
			}
		break;	
	}
	if (eoln)
	{
	
		*eoln=lend;
	}
	*line=0;
/*	printf("%ld %s\n",l, pom);*/
	return l;
} /*END myfgets()*/

/*
 zapise pole "co"  delky "delka" do souboru "dst"
a doplni je dle konstanty prislusnym koncem radku dle "lend"
vraci 0 pri uspesnem zapisu jinak EOF
*/
int swrite (FILE * dst,char * co,size_t delka,int lend){ 

	if(!dst)
	{
		return EOF;
	}

	while (delka>BUFSIZE)
	{	
		/*nepodarilo se vse zapsat do souboru*/
		if(fwrite(co,sizeof(char),BUFSIZE,dst)!=BUFSIZE) 
			return EOF;
		co+=BUFSIZE;
		delka-=BUFSIZE;
	}
	
	/*nepodarilo se vse zapsat do souboru*/
	if(delka && fwrite(co,sizeof(char),delka,dst)!=delka) 
	{
		return EOF;
	}
	
	switch (lend){
	case CR:
		if(fwrite("\r",sizeof(char),1,dst)!=1) /*nepodarilo se vse zapsat do souboru*/
			return EOF;
		break;
	default: /*pokud je lend nejaka divna hodnota, unixovy konec radky*/

	case LF:
		if(fwrite("\n",sizeof(char),1,dst)!=1) /*nepodarilo se vse zapsat do souboru*/
			return EOF;
		break;
	case CRLF:
		if(fwrite("\r\n",sizeof(char),2,dst)!=2) /*nepodarilo se vse zapsat do souboru*/
			return EOF;
		break;
	}
	return 0;
}/*END swrite()*/


/**
\brief zapise vybrany blok na pozici "poz" delky "len" ze souboru "src" na konec souboru "dst"
vraci 0 pri uspechu, -2 jeden ze souboru neni otevren, EOF chyba pri cteni/zapisu*/
int bwrite (FILE * src,FILE * dst,long poz,size_t len)
{ 
	char buf[BUFSIZE];
	size_t nacteno,i;
	if (len==0){
		return 0;
	}
	if (len<0){
#ifdef _DEBUG
		fprintf(stderr,"!!!!blok se zapornou delkou \n");
#endif
		return -1;
	}

	if (fseek(src,poz,SEEK_SET)!=0){ /*neuspesny posun ukazovatka v souboru*/
		vdoc_errno = VDOC_ERR_LIBC;
		return -1;
	}

	for(i=0;i<(len / BUFSIZE);++i){
		nacteno=fread(buf,sizeof(char),BUFSIZE,src);
		if ((nacteno == 0) && !feof(src)){
			vdoc_errno = VDOC_ERR_LIBC;
			return -1;
		}	
		if (fwrite(buf,sizeof(char),nacteno,dst)!=BUFSIZE){
			vdoc_errno = VDOC_ERR_LIBC;
			return -1;
		}
	}

	if ((i=(len % BUFSIZE))!=0){
		nacteno=fread(buf,sizeof(char),i,src);
		if ((nacteno == 0) && !feof(src)){
			vdoc_errno = VDOC_ERR_LIBC;
			return -1;
		}	
		if (fwrite(buf,sizeof(char),nacteno,dst)!=i){
			vdoc_errno = VDOC_ERR_LIBC;
			return -1;
		}
	}
	return 0;
}
size_t myfread(void * where, size_t size,size_t nmemb,MYFILE * stream){
	long poz=myftell(stream);
	long read;
	unsigned char * str;
	int c;
       	str=(unsigned char *) where;
	if (stream->f!=NULL){
		if(myfseek(stream,poz,SEEK_SET)==-1){
			message(FATAL,"seek erroro\n");
		}
		read=fread(where,size,nmemb,stream->f);
		if (read==nmemb){
			if (myfseek(stream,poz+read,SEEK_SET)==-1){
				message(FATAL, "seek error\n");
			}
			return read;
		}
		return read;
	}
	else {
		for (read=0;read<size * nmemb;++read){
			if ((c=mygetc(stream))==EOF){
				return read/size;
			}
			*str=c;
			++str;
		}
		return read/size;
	}
}/*myfread()*/

size_t myfrread(void * where, size_t size,size_t nmemb,MYFILE * stream){
	size_t fpoz,howmuch;
	fpoz=ftell(stream->f);
	howmuch=size * nmemb < fpoz?size * nmemb:fpoz;
	fseek(stream->f,fpoz-howmuch,SEEK_SET);
	howmuch=fread(where,size,howmuch/size, stream->f);	
	fseek(stream->f,fpoz-howmuch,SEEK_SET);
         stream->pos=0;
	 stream->ptr=stream->buf;
	 stream->end=stream->buf;
	 stream->eof=0;
	return howmuch;
}/*myfrread()*/

size_t myfwrite(void * where, size_t size,size_t nmemb,MYFILE * stream){

	return size;
}/*myfwrite()*/

size_t myfsize(MYFILE * stream){
       size_t last_poz;
       size_t fsize;
       last_poz=ftell(stream->f);
       fseek(stream->f,0,SEEK_END);
       fsize=ftell(stream->f);
       fseek(stream->f,last_poz,SEEK_SET);
	return fsize;
}/*myfsize()*/
