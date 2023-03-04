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
};

struct pNode
{
	char* name;
	int pid;
	struct process sons[50];
	int sonSize;
}pNodes[100];

int getParent(int ppid)
{
	for(int i = 0; i < 100; i++)
	{
		if(pNodes[i].pid == ppid)
		{
			return i;
		}
	}
	return 0;
}

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

            struct process* pro = (struct process*)malloc(sizeof(struct process));
			pro -> pid = pid;
			pro -> ppid = ppid;
			pro -> name = (char *)malloc(sizeof(name) + 1);
			strcpy(pro -> name, name);

			struct pNode* node = (struct pNode*)malloc(sizeof(struct pNode));
			node -> pid = pid;
			node -> name = (char*)malloc(sizeof(name) + 1);
			strcpy(node -> name, name);
			node -> sonSize = 0;
			pNodes[i] = *node;

			int parentNum = getParent(ppid);
			pNodes[parentNum].sons[pNodes[parentNum].sonSize] = *pro;
			pNodes[parentNum].sonSize++;

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
		printf("pid: %d, name: %s, sonSize: %d\n", pNodes[i].pid, pNodes[i].name, pNodes[i].sonSize);
	}

	

    return 0;
}