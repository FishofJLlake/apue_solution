#include<dirent.h>
#include<fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>		/* for convenience */
#include <stdio.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
#include <fcntl.h>
#include <sys/types.h>
#include <stddef.h>
#include <string.h>

#define NAME "./alonglonglonglonglonglonglonglongname"
#define TRUE 1
int main(){

	//用来获取当前目录名
	DIR *dir;
	struct dirent *dr;

	int i=0;
	
	if(chdir("/home/lxy")<0)
		perror("chdir /home/lxy error");

	while(TRUE){
		
		printf("level: %d\n", i);
		
		//创建目录
		if(mkdir(NAME, 0775)!=0)
			perror("mkdir error");
		
		if(chdir(NAME)<0)
			perror("chdir error");

		if((dir=opendir("."))==NULL)
			perror("opendir error");
		
		if((dr=readdir(dir))==NULL)
			perror("readdir error");

		char *dirName;
    strcpy(dirName, dr->d_name);
		if(strlen(dirName) > pathconf("/", _PC_PATH_MAX))
				break;
		
		if(closedir(dir)<0)
			perror("closedir error");

		i++;
	}

	// //尝试获得目录的路径名
	// long size=pathconf("/", _PC_PATH_MAX);
	// char *buf=(char *)malloc(size+1);//加上终止null字符
	// char *path_name;

	// while(TRUE){
	// 	if((path_name=getcwd(buf,size)) > 0)
	// 		break;
	// 	else{
	// 		perror("get cwd failed");
	// 		size+=100;
	// 		if((buf=(char *)realloc(buf,size))==NULL)
	// 			perror("realloc error");	
	// 	}
	// }
	// printf("length= %ld, %ld\n",strlen(buf), buf);
	exit(0);
}