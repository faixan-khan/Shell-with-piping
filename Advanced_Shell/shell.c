#include "library.h"
#include "functions.h"


extern int njobs;
char cmnd[1000]={0};


	
	

void sig_handler(int signo)
{
	if (signo == SIGUSR1)
	{
		sleep(0);
	}
	else if (signo == SIGKILL)
	{
		sleep(0);
	}
	else if (signo == SIGSTOP)
	{	
		sleep(0);
	}
	else if (signo == SIGINT)
	{
		
		sleep(0);
	}
	else if(signo==SIGTSTP)
	{
		printf("\n");
		sleep(0);

	}
	else if (signo==SIGCHLD)
	{
		sleep(0);
	}
}

	//if(signo != SIGCHLD){
//}
  

int main(int argc, char const *argv[]){


	if (signal(SIGUSR1, sig_handler) == SIG_ERR)
		sleep(0);
	if (signal(SIGKILL, sig_handler) == SIG_ERR)	
		sleep(0);	
	if (signal(SIGSTOP, sig_handler) == SIG_ERR)
		sleep(0);
	if (signal(SIGINT, sig_handler) == SIG_ERR)
		sleep(0);
	if (signal(SIGCHLD, sig_handler) == SIG_ERR)
	{
		printf("\n");
		sleep(0);
	}

	signal(SIGTSTP,sig_handler);
	//if (signal(SIGTSTP, sig_handler1))			//To check if any command had a ctr+Z so that it terminates
	//{;}

	int i=0,j=0;
	char path_exe[100]="/proc/",res[100];
	char temp[100];
	//char home_dir[100];
	int current_pid=getpid();
	sprintf(res, "%d",current_pid);
	strcat(path_exe,res);
	strcat(path_exe,"/exe");
	
  	while(1){
    	if (readlink(path_exe,temp,100) == -1)
      		perror("Error In opening:");
    	break;
  	}
  	for (i = strlen(temp)-1; i >=0 ; i--)
    	if(temp[i]=='/')
      		break;
  	for (j = 0; j < i; j++)
    	home[j]=temp[j];
  	home[j]='\0';
	
	int number_of_commands;
	char **args;
	//signal(SIGINT,event_handler);
	while(1) {
		for(int i=0;i<1000;i++)
			cmnd[i]='\0';
		// printprompt(); // printpromt moved to read_line
		number_of_commands = read_line();
		//printf("%d\n",number_of_commands);
		for (int i = 0; i < number_of_commands; i++){
			strcpy(cmnd,commands[i]);
			args = split_commands_into_tokens(commands[i]);
			//printf("%s\n",commands[i]);
			builtin_or_execute(args,number_of_commands,cmnd);
		}
	}

	return 0;
}
