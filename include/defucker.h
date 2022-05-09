#ifndef __DEFUCKER_H__
#define __DEFUCKER_H__

#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

using namespace std;

class Defucker {
	pid_t  debug_pid;
	string debug_prog;
public:
	Defucker(const char* debug_prog) : debug_prog(debug_prog) { }
	void run(const char* argv[]);
};
#endif
