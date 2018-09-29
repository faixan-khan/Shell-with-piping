#include "library.h"
#include "functions.h"

char* printprompt(void) {
	
	int uid = getuid();				// Get UID of user
	
	struct passwd *curr_user = getpwuid(uid);		// Get data about the current User
	
	char *name = curr_user -> pw_name;			// Get Name of the current User
	
	char hostname[31];			
	gethostname(hostname, 30);		//Get the hostname
    

	char ret[1000];
	char ret2[1000];

	sprintf(ret, "<\e[96m%s\e[39m@\e[96m%s\e[39m:\e[95m", name, hostname);

	getcwd(pwd, sizeof(pwd));			// Get the Current Working Directory
	
	if (strcmp(pwd, home) == 0){
		strcat(ret, "~>\e[39m");
	}
	else {

		if (strlen(pwd) > strlen(home) && strncmp(pwd, home, strlen(home)) == 0){
			sprintf(ret2, "~%s>\e[39m",pwd + strlen(home));
			strcat(ret, ret2);
		}
		else{
			sprintf(ret2, "%s>\e[39m", pwd);
			strcat(ret, ret2);
		}
	}
	
	strcpy(prompt, ret);
	return prompt;
} 
