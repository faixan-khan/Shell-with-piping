#include "library.h"
#include "functions.h"

char** redirect (int argc, char** args){
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
		int fd_take_input = open(args[take_input + 1], O_RDONLY,0);

		if(fd_take_input < 0){
			perror("Could not open the input file");
			return 0;
		}
		
		args[take_input + 1] = NULL;
		// 0 stdinput to the command
		dup2(fd_take_input, 0);
		close(fd_take_input);
	}
	
	if (write_output != 0){
		// Redirect to output for writing  
		int fd_write_output = open(args[write_output + 1],  O_WRONLY| O_TRUNC | O_CREAT, 0644);
		//printf("%s\n",args[write_output + 1]);
		if(fd_write_output < 0){
			perror("Could not open the output file");
			return 0;
		}
		args[write_output + 1] = NULL;
		dup2(fd_write_output, 1);
		close(fd_write_output);
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





	return args;
}

