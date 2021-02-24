#include <stdio.h>
#include <shadow.h>
#include <stdlib.h>

int main(void)
{
  struct spwd *ptr;

  if((ptr = getspnam("lxy")) == NULL)
  {
    // fprintf(stderr,"getsnam error\n");
    perror("getsnam error");
  }
  printf("sp_pwdp = %s\n", ptr->sp_pwdp == NULL || ptr->sp_pwdp[0] == 0 ? "(null)": ptr->sp_pwdp);
  exit(0);
}
