#include "library.h"
#include "functions.h"

int read_line(){
	char * user_info = printprompt();
	
	 //printf("%s\n", user_info);
	char *line = readline(user_info); //, &buffer, stdin);
	 //printf("%s\n", line);
	
	if (line == NULL){
		printf("\nCtrl+D recieved,exiting now\n");
		exit(0);
	}

	add_history (line);

	// printf("%s\n", line);
	const char delim[2] = ";";
	int temp = 0;
	commands[temp] = strtok(line, delim); // Breaking line into commands

	while( commands[temp] != NULL )     {
		//printf("%s\n",commands[temp]);
		commands[++temp] = strtok(NULL, delim);
	}

	return temp; // Returning number of commands
}

#define TOK_DELIM " \t\r\n\a"
char** split_commands_into_tokens(char * commandit){
    ssize_t buffer_size=1000;
    int pos=0;
    char **tokens=malloc(buffer_size * sizeof(char*));
    char *token,**temp_tokens;
    if(!tokens){
        fprintf(stderr,"Memory allocation error");
        exit(EXIT_FAILURE);
    }
    token = strtok(commandit,TOK_DELIM);
   // printf("1\n");
    while(token != NULL){
        
        tokens[pos] = token;
       // printf("%s\n",token);
        pos++;
        if(pos >= buffer_size){
            buffer_size += 1000;
            temp_tokens = tokens;
            tokens = realloc(tokens,buffer_size * sizeof(char*));
            if (!tokens) {
				free(temp_tokens);
				fprintf(stderr, "Memory allocation error\n");
				exit(EXIT_FAILURE);
			}
        }
        token = strtok(NULL,TOK_DELIM);
    }
    tokens[pos] = NULL;
    return tokens;
} 


