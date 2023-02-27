#include<stdio.h>
#include<dirent.h>

int main()
{
	DIR *dp;
	struct dirent *dirp;
	char dirname[256] = "/proc";

	if((dp = opendir(dirname)) == NULL)
		printf("Can't open %s\n", dirname);

	while((dirp = readdir(dp)) != NULL) {
        if(dirp -> d_name[0] >= '0' && dirp -> d_name[0] <= '9') {
            printf("%s\n", dirp->d_name);
        }
    }
		

	closedir(dp);
	return 0;
}