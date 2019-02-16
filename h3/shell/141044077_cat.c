/*	MUSTAFA BINGUL 141044077	*/
#include <stdio.h>
#include <stdlib.h>

#define BUF 1024
int main(int argc, char const *argv[])
{
	
	
    if(argc==1)
    {
    	FILE * file=stdin;
    	char buf[1024];
    	while(fgets(buf,BUF,file)!=NULL){    		
    		printf("%s",buf);
    		fflush(stdout);
    	}
    }else{

		FILE* file;
		char *buffer;
		int fileSize;

		if((file = fopen(argv[1],"r"))!=NULL){

			fseek(file,0,SEEK_END);
			fileSize = ftell(file);
			fseek(file,0,SEEK_SET);

			buffer = (char *)calloc(fileSize,sizeof(char));

			fread(buffer,fileSize,1,file);
			printf("%s",buffer);
			fclose(file);
			free(buffer);
		}
		else
		{	
			fprintf(stderr,"cat: %s: No such file or directory\n",argv[1]);
			
		}
	}

	return 0;
}