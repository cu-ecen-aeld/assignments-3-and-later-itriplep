#include "systemcalls.h"
#include <stdlib.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <bits/waitflags.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

extern int errno;

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{
    int checkSystem;

    openlog("assignment-3-do_system", LOG_PID | LOG_CONS , LOG_USER);
    
    syslog(LOG_DEBUG , "System command are runnig");
    checkSystem = system(cmd);

    if (checkSystem == -1){
        syslog(LOG_ERR, "Syslog doesn't run successfully");
        return false;
    }else{
        return true;
    }
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/
bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

    openlog("assignment-3-do_exec", LOG_PID | LOG_CONS, LOG_USER);

    syslog(LOG_DEBUG, "Create fork from the parent process.");
    pid_t child_pid;
    child_pid = fork();

    if(child_pid == -1){
        syslog( LOG_ERR, "Fork didn't create!(errno is %d)", errno);
        va_end(args);
        return false;

    }else if (child_pid == 0){
        execv(command[0],command);
            //syslog( LOG_ERR, "Execv Error!(errno is %d)", errno);
            exit(-1); 
            va_end(args);
            return false;
    }
    
    int status;
    if( waitpid(child_pid,&status,0) == -1){
        va_end(args);
        return false;
    }else{
        if( WIFEXITED(status) ){
            int exit_status = WEXITSTATUS(status);
            va_end(args);
            return ( exit_status == 0 ? true : false);
        }else{
            va_end(args);
            return false;
        }
    }

    va_end(args);

    return true;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    // command[count] = command[count];

    openlog("assignment-3-do_exec_redirect", LOG_CONS | LOG_PID, LOG_USER);

    syslog( LOG_DEBUG, "Create for from the parent process.");
    pid_t child_pid;

    int fd = open (outputfile, O_WRONLY | O_TRUNC | O_CREAT , 0644);

    if (fd < 0) { 
        syslog(LOG_ERR, "The file didn't open!(errno: %d)", errno);
        va_end(args);
        return false; 
    }

    switch (child_pid = fork()) {
        case -1: 
            syslog(LOG_ERR, "The fork didn't create!(errno: %d)",errno);
            va_end(args);
            close(fd);
            return false;

        case 0:
            if (dup2(fd, 1) < 0) { 
                syslog(LOG_ERR, "It can not redirect to the file!(errno: %d)", errno);
                va_end(args);
                close(fd);
                return false;
            }

            close(fd);
            execv(command[0], command); 
            perror("execvp");
            va_end(args);
            return false;
            
        default:
            close(fd); 
            int status;
            waitpid(child_pid,&status,0);
    }

/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/

    va_end(args);

    return true;
}
