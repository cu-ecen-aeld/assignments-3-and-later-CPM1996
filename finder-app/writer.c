#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>

int main(int argc, const char *argv[])
{
    const char *filePath;
    const char *fileContent;

    int fileFd;

    int retVal = 0;

    openlog(NULL, 0, LOG_USER);

    if(argc != 3)
    {
        syslog(LOG_ERR, "invalid number of arguments %u, expected 3", argc);
        closelog();
        return 1;
    }
    filePath = argv[1];
    fileContent = argv[2];

    syslog(LOG_DEBUG, "Writing %s to %s", fileContent, filePath);


    fileFd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if(fileFd == -1)
    {
        syslog(LOG_ERR, "Error while opening file %s", filePath);
        retVal = 1;
    }
    else
    {
        if(-1 == write(fileFd, fileContent, strlen(fileContent) + 1))
        {
            syslog(LOG_ERR, "Error while writing file %s", filePath);
            retVal = 1;
        }
        close(fileFd);
    }

    closelog();
    return retVal;
}
