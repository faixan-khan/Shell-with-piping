#include "library.h"
#include "functions.h"

char* printprompt(void) {
	// Get UID of user
	int uid = getuid();
	
	// Get data about the current User
	struct passwd *curr_user = getpwuid(uid);
	
	// Get Name of the current User
	char *name = curr_user -> pw_name;
	
	//Get the hostname
	char hostname[31];
	gethostname(hostname, 30);
    //printf("%s\n",hostname);

	char ret[1000];
	char ret2[1000];

	sprintf(ret, "<\e[96m%s\e[39m@\e[96m%s\e[39m:\e[95m", name, hostname);

	// Get the Current Working Directory
	getcwd(pwd, sizeof(pwd));
	
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
	// printf("%s", ret);
	strcpy(prompt, ret);
	return prompt;
} 
