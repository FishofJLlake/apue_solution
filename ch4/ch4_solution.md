## 第四章习题答案

---


#### 4.1 用stat函数替换图4-3程序中的lstat函数， 如若命令行参数之一是符号链接， 会发生什么变化？

lstat函数与stat函数的区别在于，当函数的第一个参数即文件路径名指向的是一个符号链接时，lstat返回该符号链接的有关信息， 而不是由该符号链接引用的文件的信息。也就是说stat函数返回的文件类型也不是该符号链接本身的文件类型，而是由该符号链接引用的文件的文件类型，若该符号链接指向一个不存在的文件时，stat会报错返回。

#### 4.2 如果文件模式创建屏蔽字是777（八进制） ， 结果会怎样？ 用 shell 的 umask 命令验证该结果。

如果屏蔽字为777，代表不管是文件的所有者，还是组用户或者是其他用户，其读、写、执行权限都被屏蔽，但是umask返回前一次的结果不变，默认为2，代码附下：

```c
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

```

#### 4.3 关闭一个你所拥有文件的用户读权限， 将导致拒绝你访问自己的文件， 对此进行验证。

编写代码进行验证，首先使用stat函数获取当前用户所有的某个文件的权限，并使用chmod关闭其读权限，最后使用access函数进行读权限测试。代码附下：

```C
/*
 * @Author: lxy
 * @Date: 2021-01-26 20:06:03
 * @LastEditors: lxy
 * @LastEditTime: 2021-01-26 20:32:25
 */
#include <sys/stat.h>
#include <stdlib.h>		/* for convenience */
#include <stdio.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
#include <fcntl.h>

int main(int argc, char *argv[])
{
  struct stat statbuf;

  if(argc < 2)
  {
    fprintf(stderr, "please input a file!\n");
  }

  if(stat(argv[1], &statbuf) < 0)
  {
    fprintf(stderr, "stat error!\n");
  }

  if(chmod(argv[1], statbuf.st_mode & ~S_IRUSR) < 0)
  {
    fprintf(stderr, "change mode failed!\n");
  }

  if(access(argv[1], R_OK) < 0)
  {
    fprintf(stderr, "can not read this file!\n");
  }

  return(0);
}

```

#### 4.4 创建文件foo和bar后， 运行图4-9的程序， 将发生什么情况？

如果用 open 或 creat 创建已经存在的文件，则该文件的访问权限位不变。

```shell
$ ls -l bar foo
-rw-rw-r-- 1 lxy lxy 0 1月  27 10:12 bar
-rw-rw-r-- 1 lxy lxy 3 1月  27 10:13 foo
```
```c
#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

int
main(void)
{
	umask(0);
	if (open("foo", RWRWRW) < 0)
		err_sys("creat error for foo");
	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (open("bar", RWRWRW) < 0)
		err_sys("creat error for bar");
	exit(0);
}

```
当使用open函数打开一个已存在的文件时，其访问权限位不变，文件长度也不变（除非添加O_TRUNC）。
```shell
$ ./umask 
$ ls -l bar foo
-rw-rw-r-- 1 lxy lxy 0 1月  27 10:12 bar
-rw-rw-r-- 1 lxy lxy 3 1月  27 10:13 foo
```
```c
#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

int
main(void)
{
	umask(0);
	if (creat("foo", RWRWRW) < 0)
		err_sys("creat error for foo");
	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (creat("bar", RWRWRW) < 0)
		err_sys("creat error for bar");
	exit(0);
}
```

```shell
$ ./umask 
$ ls -l bar foo
-rw-rw-r-- 1 lxy lxy 0 1月  27 10:14 bar
-rw-rw-r-- 1 lxy lxy 0 1月  27 10:14 foo
```
当使用creat创建一个已存在的文件时，其访问权限位不变，但文件长度截断为0.

#### 4.5  4.12节中讲到一个普通文件的大小可以为0， 同时我们又知道st_size字段是为目录或符号链接定义的， 那么目录和符号链接的长度是否可以为0？

对于所有的文件类型，st_size成员对其中的普通文件、目录以及符号链接有实在的意义。其中，对于普通文件而言，st_size记录了该文件的实际大小；对于目录而言，st_size的值为目录项的大小（包含inode编号及对应文件名列表），lunix默认为4096（一个数据块的大小）(对于空目录即只包含.和..的目录来说，其si_size也为4096)；对于符号链接而言，st_size的值为符号链接所指的路径文件名的长度。例如lib->usr/lib，则st_size的值即为7，(不跟随符号链接)，验证代码如下：
```C
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
```
shell命令行如下：
```shell
$ ls -l ./test
 lrwxrwxrwx 1 lxy lxy 9 1月  27 09:04 ./test -> test1.txt
$ ./p45 ./test
 ./test length: 9
$ ./p45 ./
 ./ length: 4096
```
#### p4.6 编写一个类似cp(1)的程序， 它复制包含空洞的文件， 但不将字节0写到输出文件中去。

