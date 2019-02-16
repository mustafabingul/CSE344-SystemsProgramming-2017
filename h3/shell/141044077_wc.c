/*	MUSTAFA BINGUL 141044077	*/
#include <stdio.h>
#include <fcntl.h>

#include <stdio.h> 
#include <string.h>
#include <unistd.h> 

int main(int argc, char const *argv[])
{
	
	FILE * file;
	int lineCount=0;
	char c;
	if(argc==1)
		file=stdin;
	else
		file=fopen(argv[1],"r");

	while((c=getc(file))!=EOF){
		if(c=='\n'){
			lineCount++;
		}

	}
	printf("%d\n",lineCount );
	fclose(file);

	return 0;
}