
A shell implemented in C. Written as a part of Operating Systems coursework.

EXECUTION :
	make
	./shell

Functionalities:

    1.The following functions have been written explicitly in C.
    
        1.1 : cd : Change directory

        1.2: pwd : Present Working directory

        1.3: ls : List contents of pwd
                ls -l : List the contents in long listing format
                ls -a : List contents of pwd along with hidden files in pwd.
                ls -al or ls -la : List the contents in long listing format along with those of hidden files in pwd.

        1.4: echo : prints the arguments to the shell.

        1.5: exit : To exit the shell

        1.6: pinfo : Prints the process related info of the shell program.

        1.7: Commands with last argument as "&" are processed in background and the shell spawns that process and doesn't wait for
            the process to exit. It keeps on taking user commands and printd to the shell when the process is finsihed.

        1.8: All the commands executable through execvp have been implemented and are to be used in the same way as in a normal shell.

        1.9: Personal Reminder -
                A reminder command ‘remindme’ which reminds with a custom message after the given number of seconds elapse.
                    To be executed as follows:
                        remindme <time> <custom message>
                        e.g: remindme 20 shooo
                        This will print shooo to the terminal after 20 secs.

        1.10: Clock Command :
                A ‘clock’ command which displays dynamic date and time in the shell.
                Updates are done based on fixed time interval given as an argument.
                To be executed as follows:
                    clock -t <time interval> -n <duration for which to be runned>
                    e.g: clock -t 3 -n 10
                    This will show the date and time after every 3 seconds till 10 seconds have passed. 




New Features added since last assignment :

	1.Implemented input-output redirection functionality.

	2.Implemented command redirection using pipes.

	3.User-defined commands:
			setenv,unsetenv,jobs,kjob,fg,bg,overkill,exit,CTRL+C and CTRL+Z.

