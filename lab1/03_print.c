#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include<stdlib.h>

#define PPID_LINE 7

struct process {
    char* name;
    int pid;
    int ppid;
}processes[100];

int getPID()
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

    int i = 0;
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

            processes[i].name = (char*)malloc(sizeof(name)+1);
            strcpy(processes[i].name, name);
            processes[i].pid = pid;
            processes[i].ppid = ppid;

            i++;
        }
    }

	closedir(dp);
	return i;
}

void print() {
    
}

int main() {
    int totalNum = getPID();
    printf("totalNum = %d\n", totalNum);

    for(int i = 0; i < totalNum; i++)
    {
        printf("name: %s, pid: %d, ppid: %d\n", processes[i].name, processes[i].pid, processes[i].ppid);
        printf("============================\n");
    }



    return 0;
}