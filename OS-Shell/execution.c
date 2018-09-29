#include "library.h"
#include "functions.h"

char *builtin_function_names[] = { "pinfo" , "exit"  , "clock" , "jobs" , "kjob" , "fg" , "bg" , "setenv" , "unsetenv" , "overkill" , "cd" , "echo" , "pwd" , "ls"};
int (*builtin_function_pointers[]) (int, char **,int) = { &nibba_pinfo , &nibba_exit , &nibba_clock , &nibba_job , &nibba_kjob , &nibba_fg ,&nibba_bg , &nibba_setenv , &nibba_unsetenv, &nibba_overkill ,&nibba_cd,&nibba_echo,&nibba_pwd,&nibba_ls};
int count_of_inbuild_commands = 10;
int redirection=0;
int pipe_need=0;
char* state_proc(int pid);
void add_proc(char ** args,int wpid);
void rem_job(int wpid);

void child_cheking( ){
	pid_t w_pid;
	int status;
	w_pid = waitpid(-1,&status,WNOHANG);

	if(w_pid > 0 && WIFEXITED(status)==1)
		printf("\nProcess exited normally with pid:%d\n",w_pid);
	if(w_pid > 0 && WIFSIGNALED(status)==1)
		printf("\nProcess exited due to a signal generated with pid:%d\n",w_pid);
}

int launch(int argc,char** args,char dat_commands[]){
    int background_check=0;
    if(strcmp(args[argc-1],"&")==0){
        background_check = 1;
        args[argc-1]=NULL;
        argc--;
    }
    else if(strcmp(args[0],"remindme")==0)
        background_check = 1;
    int status;
    pid_t pid;//,wpid;
    pid = fork();

    if (pid<0)//fork fail
        perror("can't create a fork");
    else if(pid == 0){
        //child process
        int remind = strcmp(args[0],"remindme");
        if(remind == 0){
            int delay = atoi(args[1]);
            //printf("%d\n",delay);   
            setpgid(0,0);
            //printf("%d\n",delay);
	        sleep(delay);
            for(int i=2;i<argc;i++)
                printf("%s ",args[i]);
            printf("\n");
            remind = 1;
            //exit(EXIT_FAILURE);
        }
        else{ 
            if(redirection!=0)
                args=redirect(argc,args);
            if(pipe_need != 0){
                  pipe_commands(dat_commands);
                  return 1;
                }

            int child_process = execvp(args[0],args);
            if(child_process == -1)
                perror("Error Executing>");
		    if(background_check==1)
		        setpgid(0,0);
            exit(EXIT_FAILURE);
        }
    }
    else if(background_check==0){
        	int status;
		waitpid(pid, &status, WUNTRACED);

		if(!WIFSTOPPED(status)) rem_job(pid);
		else
		{
			add_proc(args,pid);
			char state[10];
			 fprintf(stderr, "\n%s with pid %d has stopped!\n", args[0], pid);
			 strcpy(state,state_proc(pid));

		
			 for(int i=1;i<=proc_count;i++)
			 {
				 if(job[i].pid==pid)
				 {
					 strcpy(job[i].state,state);
				 }
			 }
		}
    }

    else add_proc(args,pid);
}


int builtin_or_execute(char **args,int argc,char dat_commands[]){
	argc = 0; // Number of Arguments

	while (args[argc] != NULL){argc++;}

	if (argc == 0){
		printf("\n");
		return 1;
	}

	char* main_command = args[0];
	int i = 0;
	for (i = 0; i < count_of_inbuild_commands; ++i){
		if (strcmp(main_command, builtin_function_names[i]) == 0 && strcmp(args[argc-1],"&") != 0){ // if main_command is in builtin command list
            return (*builtin_function_pointers[i])(argc, args,redirection); // Call the builtin function pointer
		}
	}

	return launch(argc, args,dat_commands);
}

void add_proc(char ** args,int wpid){
	pid_t pid;
	int num=0;
	int i,a=1;

	if(args[1] == NULL)
    	pid = getpid();
	else 
        pid = atoi(args[1]);

	char* path= malloc(sizeof(char)*1000);
	char* data=malloc(sizeof(char)*100000);
	char** proc_state=malloc((sizeof(char)*1000)*1000);
	char buffer;

	char * token=malloc(sizeof(char)*1024);

	proc_count++;
	job[proc_count].pid = (int) wpid;
	strcpy(job[proc_count].name,args[0]);

	printf("[%d]  %d\n",proc_count,wpid);

	sprintf(path,"/proc/%d/stat",wpid);

    int fd=open(path,O_RDONLY);
    if(fd<0){
	    perror("Error while opening the proc/pid/status file\n");
    }
    else{
	    for(i=0;a>=0;i++){
	        a=lseek(fd,i,SEEK_SET);
	        read(fd,&buffer,1);
	        data[i]=buffer;
	        if(buffer=='\n' || buffer=='\0')
	        	break;
	    }
	token=strtok(data," ");
		while(token!=NULL){
			proc_state[num]=token;
            //printf("%s\n",token);
			num+=1;
			token=strtok(NULL," ");
		}
	job[proc_count].active=1;
	strcpy(job[proc_count].state,proc_state[2]);
    }
}


void rem_job(int wpid) {
        int i;
        for(i = 1 ; i <= proc_count; i++) {
                if(job[i].pid == wpid) {
                        job[i].active = 0;
                        break;
                }
        }
}

char* state_proc(int pid){
	int num=0,i=0,j=1;
    char buffer;
	char* path= malloc(sizeof(char)*1024);
	char* data=malloc(sizeof(char)*100000);
	char * token=malloc(sizeof(char)*1024);
	char** state_proc=malloc((sizeof(char)*1024)*1024);

	sprintf(path,"/proc/%d/stat",pid);

	int fd=open(path,O_RDONLY);

	if(fd<0){
		    perror("Error while opening the status file of the process\n");
		    _exit(-1);
	    }
	else{
			for(i=0;j>=0;i++){
				j=lseek(fd,i,SEEK_SET);
				read(fd,&buffer,1);
				data[i]=buffer;
				if(buffer=='\n' || buffer=='\0')
				    break;
			}
			token=strtok(data," ");
			while(token != NULL){
				state_proc[num]=token;
				num=num+1;
				token=strtok(NULL," ");
			}
			return state_proc[2];
		}
}