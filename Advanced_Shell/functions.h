int builtin_or_execute(char **args,int argc,char *commands);
int launch(int argc,char** args,char* dat_commands);
void child_cheking( );
char* printprompt(void);
int read_line();
char** split_commands_into_tokens(char * commandit);
int nibba_clock(int argc,char** args,int redirection);
int nibba_cd(int argc,char** args,int redirection);
int nibba_exit(int argc,char** args,int redirection);
int nibba_echo(int argc,char** args,int redirection);
int nibba_pwd(int argc,char** args,int redirection);
int nibba_ls(int argc,char** args,int redirection);
int nibba_pinfo(int argc,char** args,int redirection);
int nibba_job(char **args, int argc,int redirection);
int nibba_kjob(int argc,char **args,int redirection);
char** redirect (int argc, char** args);
void pipe_commands(char input[]);
int tokeniser(char input3[], char list[][30], char* t);
void getinputfile(char input[]);
char* whitespaces(char str[]);
void rem_job(pid_t pid);
void getoutputfile(char input[]);
char* state_proc(int pid);
int nibba_fg(int argc,char** args,int redirection);
int nibba_bg( int argc,char **args,int redirection);
int nibba_setenv( int argc,char **args,int redirection);
int nibba_unsetenv( int argc,char **args,int redirection);	
int nibba_overkill (int argc, char** args,int redirection);	



char home[1000];  
char pwd[1000];   
char *commands[100]; // Array for ';' commands
char prompt[1000];
char ipfile[100], opfile[100];
int mflag;
int jobnumber[1000000], ctrZ_flag, bg_flag;

typedef struct jobs
{
	pid_t pid;
	char state[30];
	int active;
	char name[100];
}jobs;
jobs job[1000000];
//sigjmp_buf ctrlz_buf;
int proc_count;