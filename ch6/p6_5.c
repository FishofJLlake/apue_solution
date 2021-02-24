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