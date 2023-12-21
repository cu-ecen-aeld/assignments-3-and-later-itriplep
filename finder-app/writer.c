/*
Write a shell script finder-app/writer.sh as described below
    1.Accepts the following arguments: the first argument is a full path to a file (including 
        filename) on the filesystem, referred to below as writefile; the second argument is
        a text string which will be written within this file, referred to below as writestr
    2.Exits with value 1 error and print statements if any of the arguments above were not 
        specified
    3.Creates a new file with name and path writefile with content writestr, overwriting any
        existing file and creating the path if it doesn’t exist. Exits with value 1 and error
        print statement if the file could not be created.

    One difference from the write.sh instructions in Assignment 1:  You do not need to make your
        "writer" utility create directories which do not exist.  You can assume the directory is 
        created by the caller.

    Setup syslog logging for your utility using the LOG_USER facility.

    Use the syslog capability to write a message “Writing <string> to <file>” where <string> is 
        the text string written to file (second argument) and <file> is the file created by the script. 
        This should be written with LOG_DEBUG level.

    Use the syslog capability to log any unexpected errors with LOG_ERR level.
*/
#include <stdio.h>
#include <errno.h>
#include <syslog.h>
#include <stdlib.h>

extern int errno;

int main(int argc, char *argv[]){

    openlog("assignment-2-itriplep", LOG_PID | LOG_CONS , LOG_USER);

    if (argc-1 < 2){
        printf ("ERROR: two arguement are required.\n\r");
        syslog (LOG_ERR, "ERROR: two arguement are required.");
        closelog();
        return 1;
    }

    char *filename_with_path = argv[1];
    char *string_to_write = argv[2];

    syslog ( LOG_DEBUG, "Writing %s to %s", string_to_write, filename_with_path);

    FILE *file;

    file = fopen(filename_with_path,"w");
    if (file == NULL){
        syslog(LOG_ERR, "File not exist!(errno: %d)",errno);
        printf("File not exist!(errno: %d)\n\r",errno);
    }else{
        if (fputs(string_to_write,file)==EOF){
            syslog(LOG_ERR, "Can not write on file!(errno: %d)",errno);
            printf("Can not write on file!(errno: %d)\n\r",errno);
        }
    }

    if(fclose(file) != 0){
        syslog(LOG_ERR, "Can not close the file!(errno: %d)",errno);
        printf("Can not close the file!(errno: %d)\n\r",errno);
    }

    closelog();

    return 0;
}