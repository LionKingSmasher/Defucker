#ifndef __DEFUCKER_H__
#define __DEFUCKER_H__

#include <iostream>
#include <map>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

using namespace std;

typedef unsigned long reg_64_t;
typedef map<string, reg_64_t> Reisters;

class Defucker {
	pid_t  debug_pid;
	string debug_prog;
	Registers registers;
public:
	Defucker(const char* debug_prog) : debug_prog(debug_prog) { }
	void run(const char* argv[]);
	bool next_syscall();
	bool next_function_call();
};
#endif
