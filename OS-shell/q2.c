#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int file_chk(const char file[]){
    char s1[1000000],s2[1000000],s3[1000000];
	long long int i=0,j=0;
    int fd = open(file, O_RDONLY);
    
    if(fd < 0)
    {
      char text[]= "Failed to open given file\n"   ;
      write( STDOUT_FILENO, text, sizeof( text ) - 1 );
      exit(1);
    }
    int fo = open ( "Assignment/ans.txt"  , O_RDONLY );
    
    if(fo < 0)
    {
      char text[]= "Open Error\n";
      write( STDOUT_FILENO, text, sizeof( text ) - 1 );
      exit(1);
    }
    
    int file_length = lseek(fd, 0, SEEK_END);

    for(i=file_length;i>1000000;i=i-1000000){
		lseek(fd,i-1000000,SEEK_SET);
		read(fd,&s1,1000000);
        read(fo,&s3,1000000);
		for(j=0;j<1000000;j++){
			if(s1[j] >= 97 && s1[j] <= 122)
            s1[j] = s1[j] - 32;
        else if(s1[j] >= 65 && s1[j] <= 90)
            s1[j] = s1[j] + 32;
			s2[1000000-j-1]=s1[j];
		}
		for (int k = 0; k < 1000000 ; k++)
        { 
          if(s2[k]!=s3[k])
            return 0;
        }
	}
	if(i<0){
		close(fd);
		close(fo);
		return 0;
	}
	else{
		lseek(fd,0,SEEK_SET);
		char a1[i],a2[i],a3[i];
        read(fd,&a1,i);
		read(fo,&a3,i);
		for(j=0;j<i;j++){
			if(a1[j] >= 97 && a1[j] <= 122)
			a1[j] = a1[j] - 32;
		else if(a1[j] >= 65 && a1[j] <= 90)
			a1[j] = a1[j] + 32;
			a2[i-j-1] = a1[j];
			}	
		for(int k=0;k<i;k++){
            if(a2[k]!=a3[k])
                return 0;
        }
    }
    close(fd);
    close(fo);
    return 1;
}

int main(int argc, char const *argv[]) {
    
    char fn[]="Assignment/ans.txt";
    char sln[]="test.symlink";
    int fd = symlink(fn,sln);
    struct stat file;
    struct stat dir;
    
    if(argc != 2){
    const char out[] = "Usage: ./a.out [filename]\n";
    write( STDOUT_FILENO, out, sizeof( out )-1 );      
    return 0; 
    }
    stat("Assignment", &dir);
    
    //checking user permissions for file

    if (S_ISDIR(dir.st_mode)){
    char text[]= "Checking whether the directory has been created: Yes\n"   ;
    write( STDOUT_FILENO, text, sizeof( text ) - 1 );   
    }
    else{
       char text[]= "Checking whether the directory has been created: No\n"   ;
        write( STDOUT_FILENO, text, sizeof( text ) - 1 ); 
    }

    stat("Assignment/ans.txt", &file);
    
    if (S_ISREG(file.st_mode)){
    char text[]= "Checking whether the file has been created: Yes\n"   ;
    write( STDOUT_FILENO, text, sizeof( text ) - 1 );   
    }
    else{
        char text[]= "Checking whether the file has been created: No\n"   ;
        write( STDOUT_FILENO, text, sizeof( text ) - 1 );   
    }
    
    if(fd < 0){
    	char text[]= "Checking whether the symlink has been created: No\n"   ;
    	write( STDOUT_FILENO, text, sizeof( text ) - 1 );
    	}
    
    if(fd == 0){
    	char text[]= "Checking whether the symlink has been created: Yes\n"   ;
    	write( STDOUT_FILENO, text, sizeof( text ) - 1 );
    }
    // calling a function to check the case and content reversal of the file in q1 has worked.
    if (file_chk(argv[1])==1) {
        char text[]= "Checking whether file contents have been reversed and case-inverted: Yes\n"   ;
    	write( STDOUT_FILENO, text, sizeof( text ) - 1 );

    }
    else{
        char text[]= "Checking whether file contents have been reversed and case-inverted: No\n"   ;
    	write( STDOUT_FILENO, text, sizeof( text ) - 1 );
    }

    if ((file.st_mode & S_IRUSR)==0){
    const char msg[]="User has read permission on file: No\n";
    write( STDOUT_FILENO, msg, sizeof( msg )-1 );      
    }
    else{
        const char msg[]="User has read permission on file: Yes\n";
        write( STDOUT_FILENO, msg, sizeof( msg )-1 );
    }

    if ((file.st_mode & S_IWUSR)==0){
    const char msg[]="User has write permission on file: No\n";
    write( STDOUT_FILENO, msg, sizeof( msg )-1 );      
    }
    else{
        const char msg[]="User has write permission on file: Yes\n";
        write( STDOUT_FILENO, msg, sizeof( msg )-1 );
    }

    if ((file.st_mode & S_IXUSR)==0){
    const char msg[]="User has execute permission on file: No\n";
    write( STDOUT_FILENO, msg, sizeof( msg )-1 );      
    }
    else{
        const char msg[]="User has execute permission on file: Yes\n";
        write( STDOUT_FILENO, msg, sizeof( msg )-1 );
    }

    // checking group permissions for file
    if ((file.st_mode & S_IRGRP)==0){
    const char msg[]="Group has read permission on file: No\n";
    write( STDOUT_FILENO, msg, sizeof( msg )-1 );      
    }
    else{
        const char msg[]="Group has read permission on file: Yes\n";
        write( STDOUT_FILENO, msg, sizeof( msg )-1 );
    }

    if ((file.st_mode & S_IWGRP)==0){
    const char msg[]="Group has write permission on file: No\n";
    write( STDOUT_FILENO, msg, sizeof( msg )-1 );      
    }
    else{
        const char msg[]="Group has write permission on file: Yes\n";
        write( STDOUT_FILENO, msg, sizeof( msg )-1 );
    }

    if ((file.st_mode & S_IXGRP)==0){
    const char msg[]="Group has execute permission on file: No\n";
    write( STDOUT_FILENO, msg, sizeof( msg )-1 );      
    }
    else{
        const char msg[]="Group has execute permission on file: Yes\n";
        write( STDOUT_FILENO, msg, sizeof( msg )-1 );
    }
    // checking other's permission for file
    if ((file.st_mode & S_IROTH)==0){
    const char msg[]="Others has read permission on file: No\n";
    write( STDOUT_FILENO, msg, sizeof( msg )-1 );      
    }
    else{
        const char msg[]="Others has read permission on file: Yes\n";
        write( STDOUT_FILENO, msg, sizeof( msg )-1 );
    }

    if ((file.st_mode & S_IWOTH)==0){
    const char msg[]="Others has write permission on file: No\n";
    write( STDOUT_FILENO, msg, sizeof( msg )-1 );      
    }
    else{
        const char msg[]="Others has write permission on file: Yes\n";
        write( STDOUT_FILENO, msg, sizeof( msg )-1 );
    }

    if ((file.st_mode & S_IXOTH)==0){
    const char msg[]="Others has execute permission on file: No\n";
    write( STDOUT_FILENO, msg, sizeof( msg )-1 );      
    }
    else{
        const char msg[]="Others has execute permission on file: Yes\n";
        write( STDOUT_FILENO, msg, sizeof( msg )-1 );
    }

    //checking user permissions for directory
    if ((dir.st_mode & S_IRUSR)==0){
    const char msg[]="User has read permission on directory: No\n";
    write( STDOUT_FILENO, msg, sizeof( msg )-1 );      
    }
    else{
        const char msg[]="User has read permission on directory: Yes\n";
        write( STDOUT_FILENO, msg, sizeof( msg )-1 );
    }

    if ((dir.st_mode & S_IWUSR)==0){
    const char msg[]="User has write permission on directory: No\n";
    write( STDOUT_FILENO, msg, sizeof( msg )-1 );      
    }
    else{
        const char msg[]="User has write permission on directory: Yes\n";
        write( STDOUT_FILENO, msg, sizeof( msg )-1 );
    }

    if ((dir.st_mode & S_IXUSR)==0){
    const char msg[]="User has execute permission on directory: No\n";
    write( STDOUT_FILENO, msg, sizeof( msg )-1 );      
    }
    else{
        const char msg[]="User has execute permission on directory: Yes\n";
        write( STDOUT_FILENO, msg, sizeof( msg )-1 );
    }

    //checking group permissions for directory
    if ((dir.st_mode & S_IRGRP)==0){
    const char msg[]="Group has read permission on directory: No\n";
    write( STDOUT_FILENO, msg, sizeof( msg )-1 );      
    }
    else{
        const char msg[]="Group has read permission on directory: Yes\n";
        write( STDOUT_FILENO, msg, sizeof( msg )-1 );
    }

    if ((dir.st_mode & S_IWGRP)==0){
    const char msg[]="Group has write permission on directory: No\n";
    write( STDOUT_FILENO, msg, sizeof( msg )-1 );      
    }
    else{
        const char msg[]="Group has write permission on directory: Yes\n";
        write( STDOUT_FILENO, msg, sizeof( msg )-1 );
    }

    if ((dir.st_mode & S_IXGRP)==0){
    const char msg[]="Group has execute permission on directory: No\n";
    write( STDOUT_FILENO, msg, sizeof( msg )-1 );      
    }
    else{
        const char msg[]="Group has execute permission on directory: Yes\n";
        write( STDOUT_FILENO, msg, sizeof( msg )-1 );
    }

    //checking others permissions for directory
    if ((dir.st_mode & S_IROTH)==0){
    const char msg[]="Others has read permission on directory: No\n";
    write( STDOUT_FILENO, msg, sizeof( msg )-1 );      
    }
    else{
        const char msg[]="Others has read permission on directory: Yes\n";
        write( STDOUT_FILENO, msg, sizeof( msg )-1 );
    }

    if ((dir.st_mode & S_IWOTH)==0){
    const char msg[]="Others has write permission on directory: No\n";
    write( STDOUT_FILENO, msg, sizeof( msg )-1 );      
    }
    else{
        const char msg[]="Others has write permission on directory: Yes\n";
        write( STDOUT_FILENO, msg, sizeof( msg )-1 );
    }

    if ((dir.st_mode & S_IXOTH)==0){
    const char msg[]="Others has execute permission on directory: No\n";
    write( STDOUT_FILENO, msg, sizeof( msg )-1 );      
    }
    else{
        const char msg[]="Others has execute permission on directory: Yes\n";
        write( STDOUT_FILENO, msg, sizeof( msg )-1 );
    }
    return 0;
    }