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






