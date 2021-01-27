/*
 * @Author: lxy
 * @Date: 2021-01-26 20:06:03
 * @LastEditors: lxy
 * @LastEditTime: 2021-01-26 20:32:25
 */
#include <sys/stat.h>
#include <stdlib.h>		/* for convenience */
#include <stdio.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
#include <fcntl.h>

int main(int argc, char *argv[])
{
  struct stat statbuf;

  if(argc < 2)
  {
    fprintf(stderr, "please input a file!\n");
  }

  if(stat(argv[1], &statbuf) < 0)
  {
    fprintf(stderr, "stat error!\n");
  }

  if(chmod(argv[1], statbuf.st_mode & ~S_IRUSR) < 0)
  {
    fprintf(stderr, "change mode failed!\n");
  }

  if(access(argv[1], R_OK) < 0)
  {
    fprintf(stderr, "can not read this file!\n");
  }

  return(0);
}