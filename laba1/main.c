#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void MyFun(){
	printf("ATEXIT: it's me! Atexit\n");
	printf("ATEXIT: current PID = %d\n", getpid());
	printf("ATEXIT: current PPID = %d\n", getppid());
}

int main()
{
	pid_t pid;
	int exit_status = 0;
	atexit(MyFun);
	switch(pid=fork()) {
		case -1:
			perror("Something wrong");
			exit(1); 
		case 0:
			printf(" CHILD: this is a child process\n");
			printf(" CHILD: PID = %d\n", getpid());
			printf(" CHILD: parents PID =  %d\n",getppid());
			printf(" CHILD: Exit!\n");
			exit(exit_status);
		default:
			printf("PARENTS: This is parents process!\n");
			printf("PARENT: current parent PID = %d\n", getpid());
			printf("PARENT: PPID = %d\n",getppid());
			printf("PARENT: Waiting for exit...\n");
			wait(&exit_status);
			printf("PARENT: Child code: %d\n",WEXITSTATUS(exit_status));
			printf("PARENT: Exit!\n");
	}
	return 0; 
}

