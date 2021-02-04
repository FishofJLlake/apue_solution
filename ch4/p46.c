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
