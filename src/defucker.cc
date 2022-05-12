#include <defucker.h>
#include <defucker_error.hpp>

#include <sstream>

#include <sys/wait.h>
#include <sys/user.h>
#include <sys/ptrace.h>

/*
static inline bool trace_next(pid_t pid) {
	static int stat = 0;
	ptrace(PTRACE_SYSCALL, pid, 0, 0);
	waitpid(pid, &stat, 0);
	
	if(WIFEXITED(stat)){ // normally turned off
		int debug_prog_ret = WEXITSTATUS(pid);
		printf("[%d] Program Done Normally! : %d\n", pid, debug_prog_ret);
		return false;
	}
	if(WIFSIGNALED(stat)){
		return false;
	}
	return true;
}
*/

union hex_dump {
	unsigned long mem[2];
	unsigned char bytes[8];
	/*
	 * 0 ~ 4 : used
	 * 5 ~ 7 : reserved
	 *
	 * */
};

static inline void trace_getregs(pid_t pid, struct user_regs_struct* regs) {
	ptrace(PTRACE_GETREGS, pid, 0, regs);
}

static inline void trace_print_regs(struct user_regs_struct regs){
	cout << "================== Register Status ==================\n";
	cout << "RAX = " << hex << regs.rax << endl;
	cout << "RBX = " << hex << regs.rbx << endl;
	cout << "RCX = " << hex << regs.rcx << endl;
	cout << "RDX = " << hex << regs.rdx << endl;
	cout << "REX = " << hex << regs.eflags << endl;
	cout << "RBP = " << hex << regs.rbp << endl;
	cout << "RSP = " << hex << regs.rsp << endl;
	cout << "=====================================================\n";
}

void Defucker::run(const char* argv[]) {
	struct user_regs_struct regs; // regiter struct
				      
	this->debug_pid = fork();
	switch(debug_pid) {
	case -1: // Create debug process failed
		error_msg("Fork failed", -1);	
	case 0:  // Debug process section
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		execvp(debug_prog.c_str(), (char* const*)(argv + 1));
		error_msg("Exec done", 0);
	}

	waitpid(debug_pid, 0, 0);

	cout << "Start Defucker System!\n";
}	

bool Defucker::next_syscall() {
	static int stat = 0;
	ptrace(PTRACE_SYSCALL, pid, 0, 0);
	waitpid(pid, &stat, 0);
	
	if(WIFEXITED(stat)){ // normally turned off
		int debug_prog_ret = WEXITSTATUS(pid);
		printf("[%d] Program Done Normally! : %d\n", pid, debug_prog_ret);
		return false;
	}
	if(WIFSIGNALED(stat)){
		return false;
	}
	return true;

}

static inline bool is_call(pid_t pid) {
	int rip = 0;
	unsigned char op;
	union hex_dump hex;

	rip = ptrace(PTRACE_PEEKUSER, pid, 16 * 8, 0); // get rip
	
	hex.mem[0] = (unsigned char)ptrace(PTRACE_PEEKTEXT, pid, rip, NULL);
	hex.mem[1] = (unsigned char)ptrace(PTRACE_PEEKTEXT, pid, rip + sizeof(long), NULL);
	
	op = hex.bytes[4];

	if(op == 0xE8) {
		return true;
	}

	return false;
}

bool Defucker::function_call() {
	static int stat = 0;
	do{
		ptrace(PTRACE_SINGLESTEP, debug_pid, 0, 0);
		waitpid(pid, &stat, 0);
	}while(is_call(debug_pid));

}
