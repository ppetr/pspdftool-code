#include "common.h"
#include "vldoc.h"

#ifdef MALOC_CHECK
size_t __count;
#endif

char * skipwhspaces(char * s){
	while (isspace((int)(*s))){
		++s;
	}
	return s;
}

char * strtoupper(char * s){
	char * S = s;
	for(S=s;*S;++S){
		*S=toupper(*S);
	}
	return s;
}

char * strlower(char * s){
	char * S = s;
	for(S=s;*S;++S){
		*S=tolower(*S);
	}
	return s;
}

int strint(char * what,char * from[]){
	int i;
	what = skipwhspaces(what);

	for (i=0;*(from + i);++i){
		if (starts(what,from[i])){
			return i;
		}
	}
	return -1;
}





#ifndef HAVE_ASPRINTF
#include <stdarg.h>
int asprintf(char **strp, const char *fmt, ...){
	/* Guess we need no more than 100 bytes. */
	 int n, size = 100;
	 char *p, *np;
	 va_list ap;

	 if ((p = malloc (size)) == NULL){
	    return -1;
	 }

	 while (1) {
	    /* Try to print in the allocated space. */
	    va_start(ap, fmt);
	    n = vsnprintf (p, size, fmt, ap);
	    va_end(ap);
	    /* If that worked, return the string. */
	    if ((n > -1) && (n < size)){
	    	*strp = p;
	       return n;
	    }
	    /* Else try again with more space. */
	    if (n > -1)    /* glibc 2.1 */
	       size = n+1; /* precisely what is needed */
	    else           /* glibc 2.0 */
	       size *= 2;  /* twice the old size */
	    if ((np = realloc (p, size)) == NULL) {
	       free(p);
	       return -1;
	    } else {
	       p = np;
	    }
	 }
	 return -1;
}
#endif

