#include "systemcalls.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/

    int ret = system(cmd);

    return (ret == 0);
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

    int childPid;
    int wStatus;
    bool result;

    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

    childPid = fork();
    if(childPid < 0)
    {
        result = false;
    }

    if(childPid == 0)
    {
        /* Child process */
        execv(command[0], command);

        /* At this point, execv has failed */
        exit(1);
    }
    else
    {
        /* Parent process */
        if(-1 == waitpid(childPid, &wStatus, 0))
        {
            result = false;
        }
        else
        {
            result = ((WIFEXITED(wStatus)) && (0 == WEXITSTATUS(wStatus))) ? true : false;
        }
    }

    va_end(args);

    return result;
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

    int childPid;
    int wStatus;
    bool result;
    int fd;

    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

    fd = open(outputfile, O_WRONLY|O_TRUNC|O_CREAT, 0644);
    if(fd < 0)
    {
        return false;
    }

    childPid = fork();
    if(childPid < 0)
    {
        result = false;
        close(fd);
    }
    else if(childPid == 0)
    {
        /* Child process */
        if(dup2(fd, 1) <0)
        {
            exit(1);
        }

        execv(command[0], command);

        /* At this point, execv has failed */
        exit(1);
    }
    else
    {
        /* Parent process */
        close(fd);

        if(-1 == waitpid(childPid, &wStatus, 0))
        {
            result = false;
        }
        else
        {
            result = ((WIFEXITED(wStatus)) && (0 == WEXITSTATUS(wStatus))) ? true : false;
        }
    }

    va_end(args);

    return result;
}
