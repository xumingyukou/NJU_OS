#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define PPID_LINE 7

int main()
{
	DIR *dp;
	DIR *subdp;
	struct dirent *dirp;
	struct dirent *proContent;
	char dirname[6] = "/proc";
	int pid;
	char name[64];
	char status;
	int ppid;
	char buf[128];

	if((dp = opendir(dirname)) == NULL)
		printf("Can't open %s\n", dirname);

	while((dirp = readdir(dp)) != NULL) {
        if(dirp -> d_name[0] >= '0' && dirp -> d_name[0] <= '9') {

			char processDirname[256] = "/proc";
			strcat(processDirname, "/");
			strcat(processDirname, dirp ->d_name);
			strcat(processDirname, "/stat");

			// printf("%s ---- %s\n", dirp->d_name, processDirname);

			FILE* fp = fopen(processDirname, "r");
			if(fp == NULL) {
		 		perror("open");
			}
			fgets(buf, sizeof(buf), fp);
			// printf("%s\n", buf);
			sscanf(buf, "%d %s %c %d", &pid, name, &status, &ppid);
			// printf("pid: %d, name: %s, status: %c, ppid: %d", pid, name, status, ppid);
			printf("name: %s, pid: %d, ppid: %d\n", name, pid, ppid);

			
 	       	printf("\n--------------------------------\n");

        }
    }

	closedir(dp);
	return 0;
}