#include <stdio.h>
#include <unistd.h>
 #include <fcntl.h>
#include <string.h>
int main(int argc, char const *argv[])
{
	
	char *a[2];
	
	char bb[1024];
	getcwd(bb,sizeof(bb));
	printf("%s >>>  \n",bb);
	strcat(bb,"/");
	strcat(bb,"myls");
	printf("%s >>>  \n",bb);
	char *kk="ls";
	if (fork() == 0)
	{	
		
	    // child
	    int fd = open("fileE.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

	    dup2(fd, 1);   // make stdout go to file
	    

	    close(fd);     // fd no longer needed - the dup'ed handles are sufficient

	    execlp("./ls",kk,NULL);
	}



	return 0;
}


