#include "common.h"
#include "vldoc.h"

char * skipwhspaces(char * s){
	while (isspace(*s)){
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
int _asp_len;
int asprintf(char **strp, const char *fmt, ...){
	char * str = NULL;
	size_t size = 0;
	int ret_val;
	va_list ap;

	va_start(ap,fmt);
	size = vsnprintf(str, 0, fmt, ap);
	va_end(ap);
	size +=1;
	str = (char *) malloc(sizeof(char) * (size));
	if (str == NULL){
		va_end(ap);
		return -1;
	}
	va_start(ap,fmt);
	ret_val = vsnprintf(str, size, fmt, ap);
	va_end(ap);
	*strp = str;
	return ret_val;

}
#endif

