#include<apue.h>
#include<stdio.h>

void my_setbuf(FILE *restrict fp,char *restrict buf){
  if(buf){//buf非空

    //如果流指向终端设备
    if(fp==stdin||fp==stdout)
    {
      setvbuf(fp,buf,_IOLBF,BUFSIZ);
    }
    else if(fp==stderr)
    {
      setvbuf(fp,buf,_IONBF,0);
    }
    else
    {
      setvbuf(fp,buf,_IOFBF,BUFSIZ);
    }

  }
  else
  {//buf空
    setvbuf(fp,buf,_IONBF,0);
  }

}

int buffer_size(FILE *fp)
{
	return(fp->_IO_buf_end - fp->_IO_buf_base);
}

int main(){
  FILE *fp=fopen("input.txt","r+");
  fputs("one line to standard output\n", fp);
  char buf[BUFSIZ];
  printf("%d\n",BUFSIZ);
  printf("buffer size: %d\n", buffer_size(fp));
  my_setbuf(fp,buf);
  printf("after set buffer size: %d\n", buffer_size(fp));
  return(0);
}