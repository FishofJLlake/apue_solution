## 第五章知识点

#### 1.标准IO封装
- 操作对象-FILE* : 封装了不同OS对文件的实现
- 封装了缓存支持和优化

---

## 第五章习题答案
#### 5.1 用setvbuf实现setbuf。

setbuf函数主要实现缓冲区的开闭，为了带缓冲进行 I/O，参数buf必须指向一个长度为BUFSIZ的缓冲区（该常量定义在<stdio.h>中）。通常在此之后该流就是全缓冲的，但是如果该流与一个终端设备相关，那么某些系统也可将其设置为行缓冲的。为了关闭缓冲，将buf设置为NULL。setvbuf函数则可以精确地说明所需的缓冲类型。如果指定一个不带缓冲的流，则忽略buf和size参数。如果指定全缓冲或行缓冲，则buf和size可选择地指定一个缓冲区及其长度。如果该流是带缓冲的，而buf是NULL，则标准I/O库将自动地为该流分配适当长度的缓冲区。适当长度指的是由常量BUFSIZ所指定的值。

```c
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

```

#### 5.2 图5-5中的程序利用每次一行I/O（fgets和fputs函数）复制文件。若将程序中的MAXLENTH改为4， 当复制的行超过该最大值时会出现什么情况？ 对此进行解释。

对于fgets，必须指定缓冲的长度n。此函数一直读到下一个换行符为止，但是不超过n−1个字符，读入的字符被送入缓冲区。该缓冲区以null字节结尾。如若该行包括最后一个换行符的字符数超过n−1，则fgets只返回一个不完整的行，但是，缓冲区总是以null字节结尾。对fgets的下一次调用会继续读该行。
函数fputs将一个以null字节终止的字符串写到指定的流，尾端的终止符null不写出。注意，这并不一定是每次输出一行，因为字符串不需要换行符作为最后一个非null字节。通常，在null字节之前是一个换行符，但并不要求总是如此。

#### 5.3 printf返回0值表示什么？

当printf没有输出任何字符时， 如printf("");， 函数调用返回0。

#### 5.4 下面的代码在一些机器上运行正确， 而在另外一些机器运行时出错， 解释问题所在。
```c
#include <stdio.h>
int main(void)
{
  char c;
  while ((c = getchar()) != EOF)
    putchar(c);
}
```
这是一个比较常见的错误。getc以及getchar的返回值是int类型，而不是char类型。由于EOF经常定义为-1，那么如果系统使用的是有符号的字符类型，程序还可以正常工作。但如果使用的是无符号字符类型， 那么返回的EOF被保存到字符c后将不再是-1，所以，程序会进入死循环。本书说明的4种平台都使用带符号字符，所以实例代码都能工作。

#### 5.5 对标准I/O流如何使用fsync函数（见3.13节）？

fflush此函数使该流所有未写的数据都被从用户缓冲区传送至内核。作为一种特殊情形，如若fp是NULL，则此函数将导致所有输出流被冲洗。
故使用方法为: 先调用fflush后调用fsync。fsync所使用的参数由fileno函数获得。如果不调用fflush，所有的数据仍然在内存缓冲区中，此时调用fsync将没有任何效果。

#### 5.6 在图1-7和图1-10程序中， 打印的提示信息没有包含换行符， 程序也没有调用fflush函数， 请解释输出提示信息的原因是什么？

当程序交互运行时， 标准输入和标准输出均为行缓冲方式。 每次调用fgets时标准输出设备将自动冲洗。
任何时候只要通过标准I/O 库要求从（a）一个不带缓冲的流，或者（b）一个行缓冲的流（它从内核请求需要数据）得到输入数据，那么就会冲洗所有行缓冲输出流。在（b）中带了一个在括号中的说明，其理由是，所需的数据可能已在该缓冲区中，它并不要求一定从内核读数据。很明显，从一个不带缓冲的流中输入（即（a）项）需要从内核获得数据。