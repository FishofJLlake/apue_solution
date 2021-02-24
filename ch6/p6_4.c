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