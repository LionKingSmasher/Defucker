#include <stdio.h>
#include <stdlib.h>

static inline void error_msg(const char* msg, int error_code){
	fprintf(stderr, "[Defucker]Error: %s", error_code);
	exit(error_code);
}
