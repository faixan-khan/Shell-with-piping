#include "library.h"
#include "functions.h"

int njobs=0;

int tokeniser(char input3[], char list[][30], char* t)			//To tokenise anything, name misleading
{
	char *token;
	token=strtok(input3,t);
	int i=0;

	while(token)
	{
		strcpy(list[i++],token);
		token=strtok(NULL, t);
	}
	return i;

}

void getinputfile(char input[])
{
	//int len=strlen(input),
    int i;
	for(i=0; input[i]!='\0'; i++)
	{
		if(input[i]=='<')
		{
			while(input[i+1]==' ')
				i++;
			break;
		}
	}

	sscanf(input+i+1, "%[^> \n]", ipfile); 	//Scan into ipfile the string after "<"" but ignoring ">""
}

char* whitespaces(char str[])
{
	char *arr = str;
	int i,j=0;
	for (i=0;i<strlen(str);i++) 
	{
		if (isspace(str[i]) || str[i]=='\t') 
		{
			do 
			{
				i++; 
			}while (isspace(str[i]) || str[i]=='\t');
			--i;
		}
		arr[j++] = str[i];		//To include only 1 space
	}
	arr[j]='\0';
	while(isspace(*arr) || *arr=='\t') 			//Incase any spaces left in the beginning
		arr++;
	
	for(i=strlen(arr)-1;i>=0; i--)					//trailing spaces at the last part
	{
		//if((isspace(arr[i]) && isspace(arr[i-1])) || arr[i]=='\t')
		if(arr[i]!='\t')
			break;
		else if(arr[i]=='\t')		//remove trailing tabs
		{
			arr[i]='\0';
		}
	}
	for(i=0; i<strlen(arr); i++)
	{
		if(arr[i]=='\t')
		{
			arr[i]=' ';
		}
	}
	if(arr[strlen(arr)-1]=='\t')
		arr[strlen(arr)-1]='\0';
	return arr;
}

//void remove_job(pid_t pid)
//{
//	int jno=jobnumber[pid]; 		//To get jobnumber of process which got ctr+Z
//	int ptr;
//	for(ptr=jno; ptr<njobs-1; ptr++)
//	{
//		job[ptr]=job[ptr+1];
//		jobnumber[job[ptr].pid]--;
//
//	}
//	njobs--;
//
//}

void getoutputfile(char input[])
{
	//int len=strlen(input),
    int i;
	for(i=0; input[i]!='\0'; i++)
	{
		if(input[i]=='>')
		{
			while(input[i+1]==' ')
				i++;
			break;
		}
	}

	sscanf(input+i+1, "%[^< \n]", opfile); 			//scan into opfile the string after ">" but ignoring "<"
}

void pipe_commands(char input[])
{
    
    //printf("%s\n",input);
	char list[10][30],parse[100], temp[10][30];
	char*input2=malloc(100*sizeof(char));
	int i, j,k, num=0, fd[2], argc=0, status, fin,fout, in=0;
	pid_t pid;
	
    	//To tokenise wrt "|"
	num=tokeniser(input, list, "|");
	
	for(i=0; i<num; i++)
	{
		bg_flag=0;				//For each subpart, bg_flag=0 
		
		//Checking for ip and op file
		ipfile[0]='\0';
		opfile[0]='\0';
		strcpy(input, list[i]);
		for(k=0; k<strlen(input);k++)
		{
			if(input[k]=='<')
			{
				getinputfile(input);
				break;
			}

		}
		for(k=0; k<strlen(input);k++)
		{
			if(input[k]=='>')
			{
				getoutputfile(input);
				break;
			}

		}
		sscanf(input,"\n%[^<>]", input2);
		input2=whitespaces(input2);
		strcpy(parse,input2);						//Keep extra copy of input in input2

		//Tokenise each subpart of pipe into temp, parse is destroyed by strtok
		argc=tokeniser(parse,temp," ");


		char **argv=malloc(265*sizeof(char*));
		j=0;
		while(j<argc)
		{
			argv[j]=temp[j]; 					//to get argument list for execvp as execvp takes char**
			j++;
		}

		if(strcmp(temp[argc-1], "&")==0)
		{
			bg_flag=1;							//Implies command is a bg
			strcpy(temp[argc-1],"\0");
			argc--;
		}
		else if(temp[argc-1][strlen(temp[argc-1])-1]=='&')
		{
			bg_flag=1;								//Implies command is a bg
			temp[argc-1][strlen(temp[argc-1])-1]='\0';
		}

		pipe(fd);					//Make a pipe
		
		pid=fork();					
		
		if(pid==0)
		{
			if(fd<0)
				printf("Pipe error\n");
			else
			{

				if(fd[0]!=0)			//As long as input is not STDIN, duplicate it as read end of the pipe
				{
					dup2(in, 0);
					close(fd[0]);
				}
				if(i!=num-1)
				{
					dup2(fd[1], 1);				//Except for last one, output is to write end of the pipe
					close(fd[1]);
				}



				//If input redirection is there in subpart of pipe
				if(ipfile[0]!='\0')
				{
					if(access(ipfile,F_OK)!=0)
					{
						printf("File %s doesn't exist\n",ipfile);
						continue;
					}
					else if(access(ipfile,R_OK)!=0)
					{
						printf("You dont have permission to access %s\n",ipfile);
						continue;
					}
					else
					{
						fin=open(ipfile,O_RDONLY,0600);			//opening file in RDONLY with permissions
						if(fin<0)
							printf("1 File error\n");
						else if(dup2(fin,0) < 0)
							printf("2 File error\n");
						close(fin);
					}
				}
				//If op redirection is there in subpart of pipe
				if(opfile[0]!='\0')
				{
					if(access(opfile,F_OK)==0 && access(opfile,W_OK)!=0)		//Permission to open in Write mode
					{
						printf("You dont have permission to access %s\n",opfile);
						continue;
					}
					else
					{
						if(access(opfile,F_OK))				//write permissions only for user
							creat(opfile,0600);
						fout=open(opfile,O_WRONLY,0600);
						if(fout<0)
							printf("1 File error\n");
						if(dup2(fout,1)<0)
							printf("2 File error\n");
						close(fout);
					}
				}

				//If subpart of pipe is echo
				if(strcmp(temp[0],"echo")==0)
				{
					int stdout;
					if(opfile[0]!='\0')
					{
						if(access(opfile,F_OK)==0 && access(opfile,W_OK)!=0)		//Permission to open in Write mode
						{
							printf("You dont have permission to access %s\n",opfile);
							break;														//If you cant write, no point going ahead in pipe		
						}
						else
						{
							if(access(opfile,F_OK))				//write permissions only for user
								creat(opfile,0600);
							fout=open(opfile,O_WRONLY,0600);
							stdout=dup(1);							//To keep copy of stdout, to make it point to itself again later
							if(fout<0)
								printf("1 File error\n");
							if(dup2(fout,1)<0)
								printf("2 File error\n");
							close(fout);			//Dont need fout after dup
						}
					}

					if( temp[1][0] != 34)			//ASCII Value of " is 34
 					{
 						for(k=1; k<argc; k++)
							for(j=0; j<strlen(temp[k]); j++)
        	    				 printf("%c", temp[k][j]);
            			printf("\n");
            		}
            		else
            		{
            		    int count=0;
            	    	for(k=1; k<argc; k++)
            	   			for(j=0; j<strlen(temp[k]); j++)
            	        		if(temp[k][j]==34 && temp[k][j-1]!= 92)
            	        	    	count++;
                
            		    if(count%2!=0)
            	    	    printf("Error: Incomplete number of \"");
            	    	else
            	    	{
            	    		for(k=1; k<argc; k++)
            	        		for(j=1; j<strlen(temp[k])-1; j++)
            	            		if(temp[k][j]==92 && temp[k][j+1] == 34)			//if p[k][j]==" \ " and next letter at [j+1]==" " "//Not to print /
                	        		{
                	           		;
                	         		} 
                	        		else
                	            		printf("%c", temp[k][j]);
                	 	}
                	    printf("\n");
         	   		}
         	   		dup2(stdout,1);		//After redirecting output to file, control regained by stdout
         	   		_exit(1);

				}
				else
				{
					execvp(argv[0], argv);			//execute normal subparts in pipe
					_exit(1);
				}

			}		//else of (fd<0)

		}			//end of pid==0	

		else if(pid>0)
		{
			if(fd<0)
				printf("Pipe error\n");
			else
			{
				//job[njobs].pid=pid;			//current process pid
				//strcpy(job[njobs].name, input2);
				//job[njobs].name[strlen(temp[0])]='\0';
				//njobs++;
				//jobnumber[pid]=njobs-1;

				if(bg_flag!=1)
				{
					ctrZ_flag=0;
					waitpid(pid,&status,WUNTRACED);
					if(pid<0)
					{
						perror("Error!");
    	  				_exit(1);
					}
					//if(ctrZ_flag==0)
					//	remove_job(pid);			//no ctrl Z pressed, job done

				}
				else{
						pid_t w_pid;
						int status;
						w_pid = waitpid(-1,&status,WNOHANG);
						printf("[%d] %s \n",w_pid, input2);
				}

				close(fd[1]);
				in=fd[0];		
			}
		}
		else
			printf("Fork error\n");

    }	

}			

	
	