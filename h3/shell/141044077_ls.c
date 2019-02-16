/*	MUSTAFA BINGUL 141044077	*/
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#define BUFSIZE 1024
int main(int argc, char const *argv[])
{
	
	struct dirent *lsFile;
	struct stat fileStatus;
	DIR *lsDir;

	//char cwd[BUFSIZE];
	char buffer[BUFSIZE];
	//getcwd(cwd,sizeof(cwd));
	//printf("CURRENT ->  %s\n",cwd);

	lsDir = opendir(".");
	while((lsFile = readdir(lsDir))!=NULL){
		stat(lsFile->d_name,&fileStatus);
		
		printf((fileStatus.st_mode & S_IRUSR) ? "r":"-");
		printf((fileStatus.st_mode & S_IWUSR) ? "w":"-");
		printf((fileStatus.st_mode & S_IXUSR) ? "x":"-");

		printf((fileStatus.st_mode & S_IRGRP) ? "r":"-");
		printf((fileStatus.st_mode & S_IWGRP) ? "w":"-");
		printf((fileStatus.st_mode & S_IXGRP) ? "x":"-");
		
		printf((fileStatus.st_mode & S_IROTH) ? "r":"-");
		printf((fileStatus.st_mode & S_IWOTH) ? "w":"-");
		printf((fileStatus.st_mode & S_IXOTH) ? "x":"-");
		printf("  <");
		printf((S_ISDIR(fileStatus.st_mode) ? "DIRECTORY":""));
		printf((S_ISREG(fileStatus.st_mode) ? "REGULAR FILE":""));
		printf((S_ISCHR(fileStatus.st_mode) ? "CHARACTER DEVICE":""));
		printf((S_ISBLK(fileStatus.st_mode) ? "BLOCK DEVICE":""));
		printf((S_ISFIFO(fileStatus.st_mode) ? "FIFO":""));
		printf((S_ISLNK(fileStatus.st_mode) ? "SYMBOLIC LINK":""));
		printf((S_ISSOCK(fileStatus.st_mode) ? "SOCKET":""));
		printf(">  ");
		printf("     //%lld bytes//",(long long)fileStatus.st_size);
		printf("     %s\n",lsFile->d_name);

	}

	closedir(lsDir);
	

	return 0;
}