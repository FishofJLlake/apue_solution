/*
 * @Author: lxy
 * @Date: 2021-01-22 12:45:21
 * @LastEditors: lxy
 * @LastEditTime: 2021-01-22 13:53:43
 */
#include <stdio.h>
#include <error.h>
#include <stdio.h>
#include <fcntl.h>

#define BUFFERSIZE 4096
#define TEST_WR

int main()
{
  int fd;
  int n;
  int mode = 0777;
  char buf[BUFFERSIZE];

  fd = open("result", O_APPEND | O_RDWR | O_CREAT | O_TRUNC, mode);

  if(write(fd, "helloworld!\n", 12) != 12)
  {
    fprintf(stderr, "write error!\n");
  };

#ifdef TEST_WR
  off_t offset = lseek(fd, 0, SEEK_SET);
  if(write(fd, "My name is LXY!\n", 10) != 10)
  {
    fprintf(stderr, "write error!\n");
  };
#endif

#ifdef TEST_RD
  off_t offset1 = lseek(fd, 4, SEEK_SET);
  if((n = read(fd, buf, BUFFERSIZE)) < 0)
  {
    fprintf(stderr, "read error!\n");    
  }
  if(write(0, buf, n) != n)
  {
    fprintf(stderr, "write error!\n");
  };
#endif
  exit(0);
}