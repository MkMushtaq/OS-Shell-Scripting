#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>

void main(int argc,char *argv[])
{
FILE *f;
int i;
char s[256],a[256];
int fd[2];
f=fopen(argv[1],"r");

while(fgets(s,sizeof(s),f)!=NULL)
	{
	pipe(fd);	
	pid_t p;
	p=fork();
	
	if(p<0)
	     	{
		printf("Process creation failed!");
		}
	else if(p==0)
		{
		close(fd[1]);
		read(fd[0],a,sizeof(s));
		
		char *token;
		int i=0;
		token=strtok(a," ");
		char *c[5];
		while(token!=NULL)
			{
			c[i]=token;
			i++;
			token=strtok(NULL," ");			
			}
		c[i-1]=NULL;
			
		execvp(c[0],c);		
		
		}

	else if(p>0)
		{
		
		close(fd[0]);	
		write(fd[1],s,sizeof(s));		
		
		wait(NULL);	
		}

	}

}
