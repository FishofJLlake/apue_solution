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

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | )

char	buf1[] = "abcdefghij";
char	buf2[] = "ABCDEFGHIJ";

int main(void)
{
	int fd;
	if ((fd = creat("file.txt", FILE_MODE)) < 0)  /* creat hole file */
		fprintf(stderr, "creat error");

	if (write(fd, buf1, 10) != 10)
		fprintf(stderr, "buf1 write error");
	/* offset now = 10 */

	if (lseek(fd, 16384, SEEK_SET) == -1)
		fprintf(stderr, "lseek error");
	/* offset now = 16384 */

	if (write(fd, buf2, 10) != 10)
		fprintf(stderr, "buf2 write error");
	/* offset now = 16394 */
  close(fd);

  int ofd;
  if((ofd = open("file.txt", O_RDWR, FILE_MODE) < 0))
  {
    fprintf(stderr, "open error");
  }

  


}
