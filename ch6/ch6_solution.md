## 第六章习题答案
---

#### 6.1 如果系统使用阴影文件， 那么如何取得加密口令？

在linux系统中，使用struct spwd *getspnam(const char *name)函数访问加密口令。

#### 6.2 假设你有超级用户权限， 并且系统使用了阴影口令， 重新考虑上一道习题。

除非有超级用户权限， 否则调用getspnam将返回EACCES错误。示例如下：
```c
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
```
#### 6.3 编写一程序， 它调用uname并输出utsname结构中的所有字段，将该输出与uname(1)命令的输出结果进行比较。

与uname(1)命令的输出结果相比少三个输出：处理器类型、硬件平台、操作系统。因为UNIX系统并不负责定义硬件及具体实现，UNIX只定义标准规范。代码如下：
```c
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
```
#### 6.4 计算可由time_t数据类型表示的最近时间。 如果超出了这一时间将会如何?

在我的计算机上，系统使用long int来实现time_t，实际使用64位来表示long类型，因此其取值值范围为 -9223372036854775808～9223372036854775807，由于该值特别大，2900亿年后才会溢出，此时宇宙可能都不存在了。对于某些32位系统或者旧的程序，它们的time_t类型是使用32位int来实现的，而int取值范围为-2147483648～2147483647，我们可以利用localtime( )函数来分解该值，并用strftime( )函数来打印，程序如下：

```c
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>

int main (int argc, char *argv[])
{
    time_t tm_t = INT_MAX;
    struct tm* tmp = NULL;
    char buf[64] = {0};
    if (tmp = localtime(&tm_t))
    {
      strftime(buf, 63, "%Y/%m/%e %H:%M:%S \n", tmp);
    }
    printf("time: %s\n", buf);

    return 0;
}
```
```shell
$ ./p6_4
time: 2038/01/19 11:14:07 
```
由于我的本地北京时间有8个小时的时差提前，因此真正的溢出时间为2038年1月19日03:14:07。如果到这一天将会溢出，由于int是有符号数，而C和C++标准都对有符号数溢出行为未定义，因此程序的行为无法预计，可能崩溃，可能复位，可能溢出后是个无法估计的值。

#### 6.5 编写一程序，获取当前时间，并使用 strftime 将输出结果转换为类似于 date(1)命令的默认输出。将环境变量TZ设置为不同值，观察输出结果。

```c
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[])
{
    time_t tm_t = time(NULL);
    struct tm* tmp = NULL;
    char buf[64] = {0};
    printf("%s\n", argv[1]);
    if(strcmp(argv[1],"usa") == 0)
      setenv("TZ", "PST8PDT", 1);
    else 
      setenv("TZ", "GMT-8", 1);
    if (tmp = localtime(&tm_t))
    {
      strftime(buf, 63, "%Y年 %m月 %e日 星期%u  %H:%M:%S %Z\n", tmp);
    }
    printf("%s\n", buf);

    return 0;
}
```

