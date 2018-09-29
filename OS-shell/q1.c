#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>


int main(int argc, char *argv[]){
	
	// file name required in arguments
	if(argc != 2){
		const char out[] = "Usage: ./a.out [filename]\n";
		write( STDOUT_FILENO, out, sizeof( out )-1 );      // to aviod end of line char
		return 0;	
	}

	int fd = open(argv[1], O_RDONLY);
	long long int file_length = lseek(fd,0,SEEK_END);

	if(fd < 0){
		char text[]= "Failed to open the given file\n"   ;
		write( STDOUT_FILENO, text, sizeof( text ) - 1 );
		exit(1);
	}

	//printf("%lld ",file_length);
	
	mkdir("./Assignment",0700);
	
	int fo = open("./Assignment/ans.txt", O_CREAT | O_WRONLY,00700);

	if(fo < 0)
	{
		char text[]= "Error In Opening\n";
		write( STDOUT_FILENO, text, sizeof( text ) - 1 );
		exit(1);
	}
	// reversing the case and contents of the given file
	
	char s1[1000000],s2[1000000],s3[1000000];
	long long int i=0,j=0;
	
	for(i=file_length;i>1000000;i=i-1000000){
		lseek(fd,i-1000000,SEEK_SET);
		read(fd,&s1,1000000);
		for(j=0;j<1000000;j++){
			if(s1[j] <= 122 && s1[j] >= 97)
            s1[j] = s1[j] - 32;
        else if(s1[j] >= 65 && s1[j] <= 90)
            s1[j] = s1[j] + 32;
			s2[1000000-j-1]=s1[j];
		}
		write(fo,&s2,1000000);
	}
	if(i<0){
		close(fd);
		close(fo);
		return 0;
	}
	else{
		lseek(fd,0,SEEK_SET);
		char a1[i],a2[i];
		read(fd,&a1,i);
		for(j=0;j<i;j++){
			if(a1[j] <= 122 && a1[j] >= 97)
			a1[j] = a1[j] - 32;
		else if(a1[j] >= 65 && a1[j] <= 90)
			a1[j] = a1[j] + 32;
			a2[i-j-1] = a1[j];
		}	
		write(fo,&a2,i);
	}
	close(fd);
	close(fo);
	return 0;
	} 
    
	
