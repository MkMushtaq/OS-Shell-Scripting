#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>

void main(int argc,char *argv[])
{

FILE *f;
int i;
char s[20];
f=fopen(argv[1],"r");

while(fgets(s,sizeof(s),f)!=NULL)
	{

	pid_t p;
	p=fork();
	if(p<0)
	     	{
		printf("Process creation failed!");
		}
	else if(p==0)
		{
		char *token;
		token=strtok(s," ");
		i=0;
		char *c[3];
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
		wait(NULL);
		}
	}
}
