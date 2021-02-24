#include <stdio.h>
#include <stdlib.h>

#define MAXLENTH 4

int main(void)
{
  char buf[MAXLENTH];
  while(fgets(buf,MAXLENTH,stdin) != NULL)
  {
    if(fputs(buf,stdout) == EOF)
      fprintf(stderr,"output error\n");
  if(ferror(stdin))
    fprintf(stderr, "imput error\n");
  }

  exit(0);
}