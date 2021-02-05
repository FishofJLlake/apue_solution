#include <sys/stat.h>
#include <stdlib.h>		/* for convenience */
#include <stdio.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <utime.h>

int main(int argc, char *argv[])
{
  struct stat statbuf;
  if (lstat(argv[1], &statbuf) < 0)	/* stat error */
  {
    perror("can not stat");
  }
  
  printf("访问时间： %ld\n", statbuf.st_atim.tv_sec);
  printf("修改时间： %ld\n", statbuf.st_mtime);

  /*********** TODO #2 **************/
  struct timeval atimes[2];
  atimes[0].tv_sec = statbuf.st_atime;
  //atimes[0].tv_usec = 330009451;

  atimes[1].tv_sec = statbuf.st_mtime;
  //atimes[1].tv_usec = 330009451;
  
  if(utimes(argv[1], &atimes) < 0)
  {
    perror("get utimes error");
  }

  struct stat statbuf1;
  if (lstat(argv[1], &statbuf1) < 0)	/* stat error */
  {
    perror("can not stat");
  }

  printf("use utimes 访问时间： %ld\n", statbuf1.st_atime);
  printf("use utimes 修改时间： %ld\n", statbuf1.st_mtime);

  /**********************************/

	struct timespec	times[2];

  times[0].tv_sec = 1612494030;
  times[0].tv_nsec = UTIME_OMIT;    //keep the current value unchanged
  times[1].tv_sec = 1612494045;
  times[1].tv_nsec = 330009451;
  if (utimensat(AT_FDCWD, argv[1], times, 0) < 0)		/* reset times */
    perror("futimens error");

  // close(fd);

  struct stat statbuf2;
  if (lstat(argv[1], &statbuf2) < 0)	/* stat error */
  {
    perror("can not stat");
  }

  printf("use utimensat 访问时间： %ld\n", statbuf2.st_atime);
  printf("use utimensat 修改时间： %ld\n", statbuf2.st_mtime);


  exit(0);
}