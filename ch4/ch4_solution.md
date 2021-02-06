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

注意空格符和空字符的区别，空格符是' '，其asc码是0x20，而空字符为'\0'，其asc码为0。实现代码如下：

```c
/*
 * @Author: lxy
 * @Date: 2021-01-27 09:49:22
 * @LastEditors: lxy
 * @LastEditTime: 2021-01-27 10:31:52
 */
#include <sys/stat.h>
#include <stdlib.h>		/* for convenience */
#include <stdio.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
#include <fcntl.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP |S_IWGRP |S_IROTH)
#define BUFFERSIZE 4096

char	buf1[] = "abcdefghij";
char	buf2[] = "ABCDEFGHIJ";

int main(void)
{
	/* creat hole file, and write 20 bytes */
	int fd;
	if ((fd = open("file.txt", O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0)  /* creat hole file */
		fprintf(stderr, "fd open error");

	if (write(fd, buf1, 10) != 10)
		fprintf(stderr, "buf1 write error");
	/* offset now = 10 */

	if (lseek(fd, 6384, SEEK_SET) == -1)
		fprintf(stderr, "lseek error");
	/* offset now = 6384 */

	if (write(fd, buf2, 10) != 10)
		fprintf(stderr, "buf2 write error");
	/* offset now = 6394 */

	if (lseek(fd, 0, SEEK_SET) == -1)
		fprintf(stderr, "lseek error");

	// int fd1;
	// if ((fd1 = open("file.txt", O_RDWR, FILE_MODE)) < 0)  /* creat hole file */
	// 	fprintf(stderr, "fd open error");

	/* copy hole file */
  int ofd;
	if((ofd = open("filecopyhole.txt", O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0)
		fprintf(stderr, "ofd open error");

	printf("fd1:%d\n", ofd);
	int n = 0;
	char buf[BUFFERSIZE];
	while((n = read(fd, buf, BUFFERSIZE)) > 0)
	{
		// printf("n: %d\n", n);	
		if(write(ofd, buf, n) != n)
		{
			fprintf(stderr,"write error!\n");
		}
	}
	if(n < 0)
	{
		fprintf(stderr, "read error!\n");
	}

	/* copy hole file without '\0' character */
	if (lseek(fd, 0, SEEK_SET) == -1)
		fprintf(stderr, "lseek error");

  int ofd1;
	if((ofd1 = open("filecopynohole.txt", O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0)
		fprintf(stderr, "ofd1 open error");

	printf("fd1:%d\n", ofd1);
	int l = 0;
	char buf1;
	while((l = read(fd, &buf1, 1)) > 0)
	{
		// printf("n: %d\n", n);
		printf("buf1:%c\n", buf1);
		if(buf1 == '\0')
		{
			continue;
		}
		if(write(ofd1, &buf1, l) != l)
		{
			fprintf(stderr,"write error!\n");
		}
	}

	if(l < 0)
	{
		fprintf(stderr, "read error!\n");
	}

	close(fd);
	close(ofd);
	close(ofd1);

	exit(0);
}
```

#### p4.7 在4.12节ls命令的输出中,core和core.copy的访问权限不同,如果创建两个文件时umask没有变,说明为什么会发生这种差别。

在ubuntu下， du命令报告的是1024字节块的块数，并且其字节数为4096字节的整数倍（因为磁盘上的数据块设置为4096）。
当创建新的core文件时(程序运行过程中异常终止或崩溃，操作系统会将程序当时的内存状态记录下来，保存在core dump文件中，一般命名为core),内核对其访问权限有一个默认设置,在本例中为rw-r--r--。shell对创建的重定向的新文件也有一个默认的访问权限,本例中为rw-rw-rw-,这个值总是被当前的umask修改,在本例中umask为02。

#### p4.8 在运行图 4-16 的程序时,使用了 df(1)命令来检查空闲的磁盘空间。为什么不使用 du(1)命令?

当使用unlink函数时，其目录项被立即删除，但是当该文件仍旧是打开状态时，其内容不会立即删除，也就是所占用的磁盘空间不会立即释放，只有当关闭该文件或进程终止时，内容才会删除。df命令用来检查实际使用的磁盘空间，而du命令则用来检查文件或目录所占用的磁盘空间，在本题中，unlink 的文件目录项已经删除，所以不包含在du命令的统计中，故使用du无意义。

#### p4.9 图4-20中显示unlink函数会修改文件状态更改时间,这是怎样发生的?

unlink函数会进行目录项删除及链接数计数减1, 这里不区分是否时符号链接，因为如果是符号链接，符号链接本身会有一个inode节点，其链接数计数仍然会减1。而文件状态更改时间是指该文件的i节点最后一次被修改的时间，如更改文件的访问权限、更改用户ID、更改链接数等。

#### p4.10 4.22节中,系统对可打开文件数的限制对myftw函数会产生什么影响?

用opendir打开一个目录后,递归调用函数dopath。假设opendir使用一个文件描述符,并且只有在处理完目录后才调用closedir释放描述符,这就意味着目录每次降级就要使用另外一个描述符。所以进程可打开的最大描述符数就限制了我们可以遍历的文件系统树的深度。

#### p4.11 在4.22节中的myftw从不改变其目录,对这种处理方法进行改动:每次遇到一个目录就用其调用chdir,这样每次调用lstat时就可以使用文件名而非路径名,处理完所有的目录项后执行chdir("..")。比较这种版本的程序和书中程序的运行时间。

代码见p4_11.c代码，运行后发现其运行时间并没有明显的变化，具体原因待分析。
<font color=red>TODO</font> #1

#### p4.12 每个进程都有一个根目录用于解析绝对路径名,可以通过 chroot 函数改变根目录。在手册中查阅此函数。说明这个函数什么时候有用。

chroot函数被因特网文件传输协议(Internet File Transfer Protocol,FTP)程序用于辅助安全性。系统中没有账户的用户(也称为匿名 FTP)放在一个单独的目录下,利用 chroot将此目录当作新的根目录,就可以阻止用户访问此目录以外的文件。
chroot也用于在另一台机器上构造一个文件系统层次结构的副本,然后修改此副本,不会更改原来的文件系统。这可用于测试新软件包的安装。
chroot只能由超级用户执行,一旦更改了一个进程的根,该进程及其后代进程就再也不能恢复至原先的根。

#### p4.13 如何只设置两个时间值中的一个来使用utimes函数?

代码见下。utime函数使用时出现问题，使用utimensat函数代替，待解决。
```c
#include <sys/stat.h>
#include <stdlib.h>		/* for convenience */
#include <stdio.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <utime.h>

int main(int argc, char *argv[])
{
  struct stat statbuf;
  if (lstat(argv[1], &statbuf) < 0)	/* stat error */
  {
    perror("can not stat");
  }
  
  printf("访问时间： %ld\n", statbuf.st_atim.tv_sec);
  printf("修改时间： %ld\n", statbuf.st_mtime);

  /*********** TODO #2 **************/
  struct timeval atimes[2];
  atimes[0].tv_sec = statbuf.st_atime;
  //atimes[0].tv_usec = 330009451;

  atimes[1].tv_sec = statbuf.st_mtime;
  //atimes[1].tv_usec = 330009451;
  
  if(utimes(argv[1], &atimes) < 0)
  {
    perror("get utimes error");
  }

  struct stat statbuf1;
  if (lstat(argv[1], &statbuf1) < 0)	/* stat error */
  {
    perror("can not stat");
  }

  printf("use utimes 访问时间： %ld\n", statbuf1.st_atime);
  printf("use utimes 修改时间： %ld\n", statbuf1.st_mtime);

  /**********************************/

  struct timespec times[2];

  times[0].tv_sec = 1612494030;
  times[0].tv_nsec = UTIME_OMIT;    //keep the current value unchanged
  times[1].tv_sec = 1612494045;
  times[1].tv_nsec = 330009451;
  if (utimensat(AT_FDCWD, argv[1], times, 0) < 0)		/* reset times */
    perror("futimens error");

  // close(fd);

  struct stat statbuf2;
  if (lstat(argv[1], &statbuf2) < 0)	/* stat error */
  {
    perror("can not stat");
  }

  printf("use utimensat 访问时间： %ld\n", statbuf2.st_atime);
  printf("use utimensat 修改时间： %ld\n", statbuf2.st_mtime);


  exit(0);
}
```
#### p4.14 有些版本的finger(1)命令输出“New mail received ...”和“unread since ...”,其中...表示相应的日期和时间。程序是如何决定这些日期和时间的?

finger(1)对邮箱调用stat函数,最近一次的修改时间是上一次接收邮件的时间,最近访问时间是上一次读邮件的时间。

#### p4.15 用cpio(1)和tar(1)命令检查档案文件的格式(请参阅《UNIX程序员手册》第5部分中的说明)。3 个可能的时间值中哪几个是为每一个文件保存的?你认为文件复原时,文件的访问时间是什么?为什么?

cpio和tar存储的只是归档文件的修改时间(st_mtim)。因为文件归档时一定会读它,所以该文件的访问时间对应于创建归档文件的时间,因此没有存储其访问时间。cpio的-a选项可以在读输入文件后重新设置该文件的访问时间,于是创建归档文件不改变文件的访问时间。(但是,重置文件的访问时间确实改变了状态更改时间。)状态更改时间没有存储在文挡上,因为即使它曾被归档,在抽取时也不能设置其值。(utimes 函数极其相关的futimens和utimensta函数可以更改的仅仅是访问时间和修改时间。)对tar来说,在抽取文件时,其默认方式是复原归档时的修改时间值,但是tar的-m选项则将修改时间设置为抽取文件时的时间,而不是复原归档时的修改时间值。对于tar,无论何种情况,在抽取后,文件的访问时间均是抽取文件时的时间。另一方面,cpio将访问时间和修改时间设置为抽取文件时的时间。默认情况下,它并不试图将修改时间设置为归档时的值。cpio 的-m 选项将文件的修改时间和访问时间设置为归档时的值。

#### p4.16 UNIX系统对目录树的深度有限制吗?编写一个程序循环,在每次循环中,创建目录,并将该目录更改为工作目录。确保叶节点的绝对路径名的长度大于系统的 PATH_MAX 限制。可以调用getcwd得到目录的路径名吗?标准UNIX系统工具是如何处理长路径名的?对目录可以使用tar或cpio命令归档吗?

代码如下:
```c
#include <sys/stat.h>
#include <stdlib.h>		/* for convenience */
#include <stdio.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

/*
 * Default file access permissions for new files.
 */
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/*
 * Default permissions for new directories.
 */
#define	DIR_MODE	(FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

int main(int argc, char *argv[])
{
    const char* dirname = "mydir";

    long pathmax = pathconf(".", _PC_PATH_MAX);
    printf("pathmax: %ld\n", pathmax);
    char path[pathmax];
    long curr_len;
    // We keep looping until we exceed pathmax.
    while ((curr_len = strlen(getcwd(path, pathmax))) < pathmax) {
        // DIR_MODE is defined in apue.h
        // Note, mkdir returns 0 on success, not the fd of the dir.
        if (mkdir(dirname, DIR_MODE) < 0) {
            perror("mkdir error, did you make sure mydir didn't exist before running");
            return -1;
        }
        if(chdir(dirname) < 0)
        {
          perror("can not chdir");
        }
        printf("Current absolute path length: %ld\n", curr_len);
    }
    
    return 0;
}
```

当路径名超过最大限制时，getcwd可以获得路径名,但是需要多次调用realloc得到一个足够大的缓冲区。本linux程序会引发段错误，可以参考官方答案。tar可以对目录进行压缩，但是对压缩文件无法进行解压。

#### p4.17 3.16 节中描述了/dev/fd 特征。如果每个用户都可以访问这些文件,则其访问权限必须为rw-rw-rw-。有些程序创建输出文件时,先删除该文件以确保该文件名不存在,忽略返回码。
```c
  unlink (path);
  if ( (fd = creat(path, FILE_MODE)) < 0)
    err_sys(...);
```
如果path是/dev/fd/1,会出现什么情况?

/dev目录关闭了一般用户的写访问权限，以防止普通用户删除目录中的文件名。这就意味着unlink失败。
