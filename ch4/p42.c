/*
 * @Author: lxy
 * @Date: 2021-01-26 16:55:00
 * @LastEditors: lxy
 * @LastEditTime: 2021-01-26 17:20:24
 */

#include <sys/stat.h>
#include <stdlib.h>		/* for convenience */
#include <stdio.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
#include <fcntl.h>

int main()
{
  if(creat("test.txt",S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH
  |S_IXOTH) < 0)
  {
    fprintf(stderr, "create test error!\n");
  }

  umask(0777);
  if(creat("test1.txt",S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH
  |S_IXOTH) < 0)
  {
    fprintf(stderr, "create test1 error!\n");
  }

  exit(0);
}