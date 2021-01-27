/*
 * @Author: lxy
 * @Date: 2021-01-27 08:52:43
 * @LastEditors: lxy
 * @LastEditTime: 2021-01-27 09:09:17
 */
#include <sys/stat.h>
#include <stdlib.h>		/* for convenience */
#include <stdio.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
#include <fcntl.h>

int main(int argc, char *argv[])
{
  struct stat statbuf;
  size_t num = 0;
  if(lstat(argv[1], &statbuf) < 0)
  {
    fprintf(stderr, "stat error!\n");
    return(0);
  }

  num = statbuf.st_size;
  printf(" %s length: %ld\n", argv[1], num);
  return(0);

}