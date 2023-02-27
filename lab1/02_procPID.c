#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
	DIR *dp;
	DIR *subdp;
	struct dirent *dirp;
	struct dirent *proContent;
	char dirname[256] = "/proc";

	if((dp = opendir(dirname)) == NULL)
		printf("Can't open %s\n", dirname);

	

	while((dirp = readdir(dp)) != NULL) {
        if(dirp -> d_name[0] >= '0' && dirp -> d_name[0] <= '9') {

			char processDirname[256] = dirname;
			strcat(processDirname, "/");
			strcat(processDirname, dirp ->d_name);
			strcat(processDirname, "/status");

			FILE* fp = fopen(processDirname, O_RDONLY);
			if(fp == -1) {
		 		perror("open");
			}
            printf("%s\n", dirp->d_name);
			char ch = fgetc(fp);        // 从文件中读取每个字符
       		while (ch != EOF)    // 只要文件没读到结尾，就执行下面的代码
		       {
		            printf("%c", ch);
 		            ch = fgetc(fp);
		       }
 	       	printf("\n");

        }
    }


		

	closedir(dp);
	return 0;
}