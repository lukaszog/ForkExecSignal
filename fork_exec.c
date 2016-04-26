
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


#define bufor 50
int pid = 0, pid2 = 0;
int flaga = 1;

void siginit(int s) {
	if ( pid != 0 && pid2 != 0) {
		kill(pid,SIGINT);
		kill(pid2,SIGINT);
	}
	flaga = 0;
 }


int main(int argc, char *argv[]) {


	int n = atoi(argv[1]);
	int status, i;
	char par[50];
	char nrPid[bufor] = "";
	char add[50];


	sigset_t iset;
	sigemptyset(&iset);
	sigaddset(&iset, SIGTSTP);
	sigprocmask(SIG_BLOCK, &iset, NULL);


	sigset_t iset2;
	struct sigaction act;
	sigemptyset(&iset2);
	act.sa_handler = &siginit;
	act.sa_mask = iset2;
	act.sa_flags = 0;
	sigaction(SIGINT, &act, NULL);



	if (argc > 2) {
		for (i = 2; i<argc; i++)
		strcat(nrPid, argv[i]);
	}


	sprintf(add, "%d ", getpid());
	strcat(nrPid, add);
	 if (n >0 ) {
			n--;
			sprintf(par, "%d", n);

			if ((pid = fork()) == 0)
			{
				setpgid(0,0);
				execl(argv[0],argv[0],par,nrPid,NULL);
				printf("if reached this place that means sth went wrong in execl\n");
				exit(0);
			}


			if ((pid2 = fork()) == 0)
			{
				setpgid(0,0);
				execl(argv[0],argv[0],par,nrPid,NULL);
				printf("if reached this place that means sth went wrong in SECOND execl\n");
				exit(0);
			}

	}

	while (flaga)
	{sleep(1);}

	pid_t pid = wait(&status);
	pid = wait(&status);
	if( WEXITSTATUS(pid) )
		printf("%s\n",nrPid);



	sigset_t iset3;
	sigemptyset(&iset3);
	sigpending(&iset3);

	if (sigismember(&iset3,SIGTSTP )== 1) {
		printf("Ctrl+z detected\n");
		struct sigaction act2;
		sigemptyset(&iset3);
		act2.sa_handler = SIG_IGN;
		sigaction(SIGTSTP, &act2, NULL);
	}


	sigprocmask(SIG_UNBLOCK, &iset, NULL);

	return 0;
}


