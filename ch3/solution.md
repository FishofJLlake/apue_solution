## 第三章习题答案 

---

#### 3.1 当读/写磁盘文件时,本章中描述的函数确实是不带缓冲机制的吗? 请说明原因。

实际上书中所说的不带缓冲机制是指在用户空间中不用缓冲区，但是在调用系统调用在内核中运行时，write函数不是立即就将数据写入磁盘，而是会将数据复制到缓冲区高速缓存中，满足一定条件后再写入磁盘。read函数在大多数文件系统中，采用预读技术，当正在进行顺序读取时，系统会读入比应用要求更多的数据存在缓存中。

#### 3.2 编写一个与 3.12 节中 dup2 功能相同的函数,要求不调用 fcntl 函数,并且要有正确的出错处理。

程序实现链接```/home/lxy/exec_apue/ch3/p32.c```

```c
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

```

#### 3.3 假设一个进程执行下面3个函数调用:
```c
fd1 = open(path, oflags);
fd2 = dup(fd1);
fd3 = open(path, oflags);
```
#### 画出类似于图3-9的结果图。对 fcntl 作用于 fd1 来说, F_SETFD 命令会影响哪一个文件描述符? F_SETFL 呢?

图略。当一个进程调用open打开一个文件时，进程会在进程表中维护一个文件描述符表，每个文件描述符占用一项，包括两个数据结构：文件描述符标志、指向一个文件表项的指针。而该文件表又包含文件状态标志、文件偏移量、i节点指针（i节点包含了文件的所有者、文件长度、指向文件实际数据块在磁盘上所在位置的指针等）。当接着调用dup函数时，将fd1对应的文件表项指针复制给fd2，这样返回的fd2与fd1共享同一个打开文件表，如果接着调用open函数打开同一个文件，则会返回一个不同的描述符，指向不同的打开文件表，但是其中i节点指针指向同一个文件。根据上述分析，如果fcntl作用于fd1，则 F_SETFD 通过设置文件描述符标志，影响fd1描述符，F_SETFL 通过设置共享的文件状态标志，影响fd1,fd2。

#### 3.4 许多程序中都包含下面一段代码:
```c
dup2(fd, 0);
dup2(fd, 1);
dup2(fd, 2);
if (fd > 2)
close(fd);
```
#### 为了说明if语句的必要性,假设fd是1,画出每次调用dup2时3个描述符项及相应的文件表项的变化情况。然后再画出fd为3的情况。

图略。当fd为1时，调用3次dup2函数后，相当于将0，1，2都重定向至1（标准输出），fd不会close。当fd为3时，相当于将0，1，2都重定向至3所指向的文件表项，故3无存在必要。close()函数只是delete fd对应的指向文件项指针，同时在打开文件项中引用计数-1。

#### 3.5 在 Bourne shell 、Bourne-again shell 和 Korn shell 中, digit1>&digit2表示要将描述符digit1重定向至描述符digit2的同一文件。请说明下面两条命令的区别。
```
./a.out > outfile 2>&1
./a.out 2>&1 > outfile
```
#### (提示: shell 从左到右处理命令行。)

第一条命令表示 该可执行文件标准输出重定向至outfile，同时标准错误重定向至标准输出，其本质上是进行文件描述符的复制，首先将标准输出指向outfile，标准错误指向标准输出，这样都指向了同一个文件表项。第二条命令首先将标准错误的文件表项指向标准输出的文件表项，这一步之后两者都指向终端的标准输出，后面再将标准输出指向outfile文件的表项，其结果就是两者指向不同的文件表项。类似于执行以下操作：
```c++
//第一条命令
dup2(fd, 1);
dup2(1, 2);

//第二条命令
dup2(1, 2);
dup2(fd, 1); /* 这一部操作后标准错误和fd就指向不同的文件表项 */
```
#### 3.6 如果使用追加标志打开一个文件以便读、写,能否仍用 lseek 在任一位置开始读?能否用 lseek 更新文件中任一部分的数据? 请编写一段程序验证。

程序实现链接```/home/lxy/exec_apue/ch3/p36.c```

```c
/*
 * @Author: lxy
 * @Date: 2021-01-22 12:45:21
 * @LastEditors: lxy
 * @LastEditTime: 2021-01-22 13:53:43
 */
#include <apue.h>
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

```

从程序结果上，可以发现read可以任意用lseek函数指定，但是write函数即使使用lseek重新定位，但是仍然会自动定位到文件的末尾将只能的内容写到文件中。设计 O_APPEND 的目的其实还是为了多进程考虑，当出现竞争条件的时候，不至于出现相互覆盖的情况。