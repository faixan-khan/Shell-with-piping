#include "library.h"
#include "functions.h"	

int nibba_clock(int argc,char** args,int redirection){
	if(argc != 5)
		perror("ERROR IN ARGUMENTS ");
	else if(strcmp(args[1],"-t")!=0)
		perror("ERROR IN ARGUMENTS ");
	else if(strcmp(args[3],"-n")!=0)
		perror("ERROR IN ARGUMENTS ");
	else{
		int duration =atoi(args[4]);
		int inter = atoi(args[2]);
		//printf("%d\n",inter);
		while(duration>0){
					duration = duration - inter;
					char myclock[1000];
					char date[1000];
					char mydate[100];
					char mytime[100];

					strcpy(myclock,"/sys/class/rtc/rtc0");
					strcpy(date,myclock);
					strcat(date,"/date");

					FILE * dat=fopen(date,"r");
					fscanf(dat,"%s",mydate);
					printf("%s ",mydate);

					strcpy(date,myclock);
					strcat(date,"/time");

					FILE * tym=fopen(date,"r");
					fscanf(tym,"%s",mytime);
					printf("%s\n",mytime);
					sleep(inter);
		}
	}
	return 0;
}

int nibba_job(char **args, int argc,int redirection){
	for(int i=1;i<=proc_count;i++){
		if(job[i].active==1)
			printf("%d %s %s[%d]\n",i,job[i].state,job[i].name,job[i].pid);
	}
	return 0;
}

pid_t search(int proc_num){
	
	for(int i = 1 ; i <= proc_count; i++) {
        if(i == proc_num) {
			return job[i].pid;
		}
	}
	return 0;	
}
int nibba_kjob(int argc,char **args,int redirection){
	if (argc < 3) {
		fprintf(stderr, "Usage:\n\rkjob <job number> <signal number>\n");
		return -1;
	}
	//printf("%s\n",args[1]);
	int proc_num = atoi(args[1]);
	int sig = atoi(args[2]);
	pid_t iid = search(proc_num);
	//printf("%d\n",iid);
	if (iid == 0) {
		fprintf(stderr, "os-shell: Wrong process number!\n");
		return -1;
	}
	int ret = kill(iid, sig);
	rem_job(iid);
	if (ret)
		fprintf(stderr, "os-shell:dead\n");

	return ret;
}

int nibba_cd (int argc, char** args,int redirection) {
	if (argc == 1 || strcmp(args[1], "~") == 0){
		chdir(home);
	}
	else{
		int check = chdir(args[1]);
		if (check < 0)
			perror("cd : Error encountered");
	}
	return 0;
}
int nibba_exit(int argc,char** args,int redirection){
	exit(0);
	exit(1);
}
int nibba_echo(int argc,char** args,int redirection){
	if(argc <= 1){
		printf("\n");
		return 1;
	}
	else{
		for (int i=1;i<argc;i++){
			printf("%s", args[i]);
		}
  	}
    printf("\n");
  	return 1;
}
int nibba_pwd(int argc,char** args,int redirection){
    printf("%s\n",pwd);
    return 0;
}

int nibba_pinfo (int argc, char** args,int redirection) {

	int take_input=0,write_output = 0,append = 0;
	for (int i = 0; i < argc; i++){
		if (strcmp(args[i], ">")==0){
			args[i] = NULL;
			write_output = i;
		}
		else if (strcmp(args[i], "<")==0){
			args[i] = NULL;
			take_input = i;
		}
		else if (strcmp(args[i], ">>")==0){
			args[i] = NULL;
			append = i;
		}
	}
	
	if (take_input != 0){
		// open the file into fd 
		int fd_input = open(args[take_input + 1], O_RDONLY,0);

		if(fd_input < 0){
			perror("Could not open the input file");
			return 0;
		}
		
		args[take_input + 1] = NULL;
		// 0 stdinput to the command
		dup2(fd_input, 0);
		close(fd_input);
	}
	
	if (write_output != 0){
		// Redirect to output for writing  
		int fd_write = open(args[write_output + 1],  O_WRONLY| O_TRUNC | O_CREAT, 0644);
		//printf("%s\n",args[write_output + 1]);
		if(fd_write < 0){
			perror("Could not open the output file");
			return 0;
		}
		args[write_output + 1] = NULL;
		dup2(fd_write,1);
		printf("seg\n");
		//close(fd_write);
		
	}

	if (append != 0){
		// Redirect for output for appending
		int fd_append = open(args[append + 1], O_APPEND | O_RDWR | O_CREAT, 0644);

		if(fd_append < 0){
			perror("Could not open the output file");
			return 0;
		}
		args[append + 1] = NULL;
		dup2(fd_append, 1);
		close(fd_append);
	}

	
	//close(1);
	char process[100000];
	char statpath[1024],process_path[1024];
	int pos=0; 
	
	char **tokens=malloc(101 * sizeof(char*));
	char *token;


	strcpy(process_path, "/proc/");
	//printf("%s\n",process_path);
	//printf("%d\n",argc);
	if(argc > 10)
		strcat(process_path, args[1]);
	else
		strcat(process_path, "self");

	strcpy(statpath, process_path);
	strcat(statpath, "/stat");

	int fd = open(statpath, O_RDONLY);
	// open stat file of process in read mode
	if( fd < 0 ){ // No stat file found
		perror("Shell ");
		return 0;
	}
	read(fd,&process,100);
	//printf("%s\n",process);
    if(!tokens){
        fprintf(stderr,"Memory allocation error");
        exit(EXIT_FAILURE);
    }
    token = strtok(process," ");
   // printf("1\n");
    while(token != NULL){
		//printf("%d\n",pos);
        tokens[pos] = token;
        //printf("%s\n",tokens[pos]);
        pos++;
        token = strtok(NULL," ");
    }
    tokens[pos] = NULL;
	fprintf(stdout,"pid:%s\n", tokens[0]);
	fprintf(stdout, "Name: %s\n", tokens[1]);
	fprintf(stdout, "Process Status: %s\n",tokens[2]);
	fprintf(stdout, "V Mem: %s\n",tokens[22]);
	close(fd);
	
	char exe_path[1000];

	strcpy(statpath, process_path);
	strcat(statpath, "/exe");

	// procpath/exe is sybolink link to the executable path
	
	readlink(statpath, exe_path, sizeof(exe_path));
	
	char relative_path[1024];

	if (strlen(exe_path) > strlen(home) && strncmp(pwd, home, strlen(home)) == 0){
		strcpy(relative_path, "~");
		strcat(relative_path, exe_path + strlen(home));
	}
	else{
		strcpy(relative_path, exe_path);
	}
	// clearing exepath for further use
	int i = 0;

	while(exe_path[i]){
		exe_path[i] = '\0';
		i++;
	}

	fprintf(stdout, "Executable Path: %s\n", relative_path);
	return 0;
}
int nibba_ls(int argc,char** args,int redirection){
	int expanded_files = 0,x=0;
	int hidden_files = 0;
	int* flag_files = malloc(100*sizeof(int));
	for(int i=1;i<argc;i++){
		if(strcmp(args[i],"-l")==0)
			expanded_files = 1;
		else if(strcmp(args[i],"-a")==0)
			hidden_files = 1;
		else if(strcmp(args[i],"-la")==0 || strcmp(args[i],"-al")==0){
			expanded_files = 1;
			hidden_files = 1;
		}
		else
			flag_files[x++] = i;
	}
	flag_files[x]='\0';
	if(x==0){
		args[1]=".";
		flag_files[0] = 1;
		x=1;
	}
	for(int i=0;i<x;i++){
		DIR *current_dir;
		current_dir=opendir(args[flag_files[i]]);
		if(current_dir == NULL){
			perror("Error in opening: ");
			printf("filename: %s\n", args[flag_files[i]]);
			return 0;
		}
		struct dirent* current_files;
		struct stat file_stat;
		struct stat attrib;
    	char date[10];
		char file_path[1000];
		while((current_files=readdir(current_dir)) != 0){
			if (hidden_files == 0 && current_files->d_name[0] == '.'){
				// not printing hidden files
				continue;
			}
			strcpy (file_path, args[flag_files[i]]);
			strcat (file_path, "/");
			strcat (file_path, current_files->d_name);
			if(expanded_files==1){
				//use stat to get permissions

				//printf("%s\n",file_path);

				if(stat(file_path,&file_stat) == 0){
					stat(file_path, &attrib);
					struct passwd* user = getpwuid(file_stat.st_uid);
					struct group* grp = getgrgid(file_stat.st_gid);
					printf((S_ISDIR(file_stat.st_mode))  ? "d" : "-");
					printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
					printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
					printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
					printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
					printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
					printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
					printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
					printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
					printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");
					printf("\t%d\t",(int) file_stat.st_nlink);
					printf("\t%s\t",user->pw_name);
					printf("\t%s\t",grp->gr_name);
					printf("\t%ld\t",(long int) file_stat.st_size);
					strftime(date, 20, "%b %d %H:%M", localtime(&(attrib.st_ctime)));
    				printf("\t%s\t", date);
					printf("\t%s\n ",current_files->d_name);

				}
			}
			else
			printf ("%s\t", current_files->d_name);
		}
		printf("\n");
	}
	return 0;
}



int nibba_fg(int argc,char** args,int redirection){
	if(argc != 2){
		printf("Invalid Arguments to fg command\n");
		return 1;
	}
	int proc_fg = atoi(args[1]);
	if(job[proc_fg].active != 0){
		int proc_id;
		int status;
		proc_id=job[proc_fg].pid;
		char state[10];
		strcpy(state,state_proc(proc_id));
		kill(proc_id,SIGCONT);
		waitpid(proc_id, &status, WUNTRACED);
		if(WIFSTOPPED(status)){
			
			printf("\n%s with pid %d has stopped!\n", args[0], proc_id);
			
			for(int i=1;i<=proc_count;i++){
				if(job[i].pid == proc_id){
					strcpy(job[i].state,state);
				}
			}
		}
		else{
			job[proc_fg].active = 0;
						
			for(int i=1;i<=proc_count;i++){
				if(job[i].pid == proc_id){
					strcpy(job[i].state,state);
				}
			}
		}
		return 1;
	}
	else if(job[proc_fg].active == 0){
		printf("No such job exists\n");
		return 1;
	}
	else{
		printf("No job found\n");
		return 1;
	}
}

int nibba_bg( int argc,char **args,int redirection){
	if (argc != 2){
		fprintf(stderr, "Usage:\n\rfg <pid>\n");
		return -1;
	}
	int proc_num = atoi(args[1]);
	pid_t cp = search(proc_num);
	int proc_id=job[proc_num].pid;
	if (cp == 0) {
		fprintf(stderr, "os-shell: Wrong process number!\n");
		return -1;
	}
	int ret = kill(cp, SIGCONT);
	int status;pid_t pid;
	strcpy(job[proc_num].state,"R");
	if (ret)
		printf("os-shell error:\n");

	return 1;
}

int nibba_setenv( int argc,char **args,int redirection){
	if (args[2] == NULL) 
		args[2] = " ";
	if (setenv(args[1],args[2],1) != 0) 
		perror("shell");
	return 1;	
}

int nibba_unsetenv( int argc,char **args,int redirection){
	if (unsetenv(args[1]) != 0) 
		perror("shell");
	return 1;
}

int nibba_overkill (int argc, char** args,int redirection){
//	printf("jsvjhv\n");
	if (argc != 1) {
		fprintf(stderr, "Usage:\n overkill \n");
		return -1;
	}
	for(int i=1;i<=proc_count;i++){
    	job[i].active = 0;
        int ret = kill(job[i].pid,9);
		if (ret){
			printf("%d\n",i);
			fprintf(stderr, "os-shell: error\n");
		}
	}

	return 1;
}