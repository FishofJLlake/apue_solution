
#include <sys/stat.h>
#include <stdlib.h>		/* for convenience */
#include <stdio.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

/*
 * Default file access permissions for new files.
 */
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/*
 * Default permissions for new directories.
 */
#define	DIR_MODE	(FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

int main(int argc, char *argv[])
{
    const char* dirname = "mydir";

    long pathmax = pathconf(".", _PC_PATH_MAX);
    printf("pathmax: %ld\n", pathmax);
    char path[pathmax];
    long curr_len;
    // We keep looping until we exceed pathmax.
    
    while ((curr_len = strlen(getcwd(path, pathmax))) < pathmax)  //length include '/'
    { 
        // DIR_MODE is defined in apue.h
        // Note, mkdir returns 0 on success, not the fd of the dir.
        if (mkdir(dirname, DIR_MODE) < 0) {
            perror("mkdir error, did you make sure mydir didn't exist before running");
            return -1;
        }
        if(chdir(dirname) < 0)
        {
          perror("can not chdir");
        }
        printf("Current absolute path length: %ld\n", curr_len);
    }
    
    return 0;
}