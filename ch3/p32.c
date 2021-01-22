/*
 * @Author: liuxiangyang
 * @Date: 2021-01-22 12:45:21
 * @LastEditors: liuxiangyang
 * @LastEditTime: 2021-01-22 13:08:14
 */

#include <apue.h>
#include <error.h>
#include <stdio.h>
#include <fcntl.h>


int dup_self(int fd1, int fd2)
{
  char *fdptr;
  int openmax = sysconf(_SC_OPEN_MAX);
  int tmpfd;
  int fdarr[openmax];
  int count;

  /* fd must not be bigger than OPEN_MAX */
  if(fd1 > openmax || fd2 > openmax)
  {
    fprintf(stderr, "the argument error\n");
    exit(1);
  }

  /* Determines whether the file descriptor is openned */
  fdptr = malloc(30 * sizeof(char));
  sprintf(fdptr, "/proc/%d/fd/%d", getpid(), fd1);
  if(access(fdptr, F_OK) < 0)
  {
    fprintf(stderr, "the fd1 not open\n");
    exit(2);
  }
  sprintf(fdptr, "/proc/%d/fd/%d", getpid(), fd2);
  if(access(fdptr, F_OK) < 0)
  {
    fprintf(stderr, "the fd2 not open\n");
    exit(3);
  }

  /* Determine fd1 and fd2 are equal or not */
  if(fd1 == fd2)
  {
    return fd2;
  }
  else
  {
    close(fd2);
  }

  /* using dup function to match fd */
  while(tmpfd = dup(fd1))
  {
    if(tmpfd == fd2)
    {
      break;
    }
    else
    {
      fdarr[count++] = tmpfd;     /* save the value and close the fd later */
    }
  }
  printf("%d\n", fd2);
  for(int i ; i < count; ++i)
  {
    close(fdarr[i]);
  }

  return fd2;
}

int main()
{
  int mode = 0777;
  int fd1 = open("result", O_RDWR | O_CREAT | O_TRUNC, mode);
  printf("fd1:%d\n",fd1);
  int flag = dup_self(fd1, 1);  /* at this time，stdout redirect to fd1 */
  printf("flag:%d\n",flag);  /* also the stdout is redirected to fd1 */
  write(fd1, "hello\n", 6);
  int fl = fcntl(fd1, F_GETFD, 0);
  printf("file flag: %d\n", fl);
  exit(0);
}