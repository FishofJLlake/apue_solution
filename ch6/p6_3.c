#include <stdio.h>
#include <sys/utsname.h>
#include <stdlib.h>

int main(void)
{
  // struct utsname name; //在栈上静态分配内存
  struct utsname *name;
  name = (struct utsname *)malloc(sizeof(struct utsname)); //为该指针指向的变量在堆上分配内存
  if(uname(name) < 0)
  {
    perror("call uname error\n");
  }
  printf("sysname = %s\n", name->sysname);
  printf("nodename = %s\n", name->nodename);
  printf("release = %s\n", name->release);
  printf("version = %s\n", name->version);
  printf("machine = %s\n", name->machine);
  exit (0);
}