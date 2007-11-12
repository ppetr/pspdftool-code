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
int asprintf(char **strp, const char *fmt, ...){
/*TODO: implement it.*/
	va_list params;
	unsigned int length = 0;
	va_start(params, fmt);
	if (fmt == 0) return 0;
	while (*fmt) {
		if (*fmt == '%') {
			++fmt;
			switch(*fmt) {
				/* Character */
				case 'c':
					length += putc((char) va_arg(params, int));
					break;
				/* String */
				case 's':
					length += puts(va_arg(params, const char*));
					break;
				/* Decimal */
				case 'd':
				case 'i':
					length += putd(va_arg(params, const int));
					break;
				/* Unsigned decimal */
				case 'u':
					length += putu(va_arg(params, const unsigned int));
					break;
				/* Hexadecimal */
				case 'x':
					length += puth(va_arg(params, const unsigned int));
					break;
				/* Pointer */
				case 'p':
					length += putp(va_arg(params, const void *));
					break;
				/* The % character */
				case '%':
					length += putc('%');
					break;
			}
			++fmt;
		} else {
			putc(*fmt++);
			++length;
		}
	}
	va_end(params);
	return length;
}
#endif

