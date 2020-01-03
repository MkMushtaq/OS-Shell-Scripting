#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<dirent.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>

int find(char a[],char x)
	{
	int i=0;
	for(i=0;i<strlen(a);i++)
		{
		if(a[i]==x)
			{
			return 0;
			}
		}
	return 1;
	}

void pipeline(char ***cmd)
	{
	int p[2],fdd=0;
	while(*(cmd)!=NULL)
		{
		pipe(p);
		if(fork()==0)
			{
			dup2(fdd,0);
			if(*(cmd+1)!=NULL)		
				{
				dup2(p[1],1);
				}
			close(p[0]);
			execvp((*cmd)[0],*cmd);
			}
		else
			{
			wait(NULL);
			close(p[1]);
			fdd=p[0];
			cmd++;
			}	
		}
	}

void main(int argc,char *argv[])
{
char c[256],d[256];
char s[256],a='>';
char cwd[256];
int c1=-1,i=0,o,pass=0,pass1=0;
FILE *f;
char s1[256],s2[256];
char s11[256],s12[256];
	



getlogin_r(c,sizeof(c));
gethostname(d,sizeof(d));
while(1){

if(c1!=1)
	{
	if(getcwd(cwd, sizeof(cwd))!=NULL)
			{
			printf("<%s>",cwd);    /*Every time calling cwd because 						to get the updated directory*/ 
			}
	gets(s);
	
	
	}
else
	{
	gets(s);
	c1=-1;            //This will make to print the directory every time 				  //as the user is done with using cd command   
	}	


if(strncmp(s,"cd",2)==0)
	{
	if(chdir((s+3))==0)  
		{
		if(getcwd(cwd, sizeof(cwd))!=NULL)
			{
			printf("<%s>",cwd);
			c1=1;       //This will make the user ask only the input
				    //and not displaying the directory again
			}
		else
			{
			printf("Error"); 
			}
		}
	else
		{
		printf("Unable to open file\n");
		}
	}
else if(strcmp(s,"ls")==0)
	{
	if(fork()==0)
		{
		pid_t p;
		p=getpid();
		f=fopen("process.txt","a+");
		fprintf(f,"%s",s);
		fprintf(f,"\t");
		fprintf(f,"Process id:%d",p);	
		fprintf(f,"\n");
		fclose(f);
		char *cmd1[5];
		char *token;
		token=strtok(s," ");
		
		cmd1[0]=token;
		cmd1[1]=NULL;
		execvp(cmd1[0],cmd1);
		}
	else
		{
		wait(NULL);
		}	
	}


else if(strcmp(s,"pid")==0)  //If user enters pid command 
	{
	if(fork()==0)
		{
		pid_t p;
		p=getpid();
		printf("Process id:%d\n",p);
		f=fopen("process.txt","a+");
		fprintf(f,"%s",s);
		fprintf(f,"\t");
		fprintf(f,"Process id:%d",p);	
		fprintf(f,"\n");
		fclose(f);
		}
	else
		{
		wait(NULL);
		}	
	}
else if(strcmp(s,"hist")==0)
	{
	printf("Ok");
	f=fopen("process.txt","r");
	char inp[256];
	while( fgets(inp,sizeof(inp),f) != NULL)
		{
		printf("%s",inp);
		}
	fclose(f);
	}
else if(strncmp(s,"hist",4)==0 && (atoi(s+4)-1)>=0)
	{
	f=fopen("process.txt","r");
	char inp[256];
	for(i=0;i<atoi(s+4);i++)
		{
		fgets(inp,sizeof(inp),f);
		printf("%s",inp);
		}
	fclose(f);
	}
else if(strncmp(s,"!hist",5)==0 && (atoi(s+5)-1)>=0)
	{
	f=fopen("process.txt","r");
	char inp[256];
	for(i=0;i<atoi(s+5);i++)
		{
		fgets(inp,sizeof(inp),f);
		}
	printf("%s",inp);		
	fclose(f);
	}
else if(find(s,'|')==0 && (find(s,'<')==0 || find(s,'>')==0))
	{
	i=0;
	int inc=0,k1=0,k2=0;
	while(s[i]!='\0')
		{
		if(s[i]=='|')     //find the input and output directors
			{	//and storing in s1,s2 arrays
			inc++;
		    	}
		if(s[i]=='>' || s[i]=='<')
			{
			inc++;	
			}
		if(inc<2)
			{
			s11[k1]=s[i];
			k1++;
			}
		if(inc>=1)
			{
			s12[k2]=s[i+2];
			k2++;
			}
		i++;
		}
	pass=1;
	s11[k1-1]='\0';
	s12[k2-2]='\0';
	
	pass1=1;
	}
if(find(s,'|')==0 || pass1==1)
	{
	if(pass1==1)
		{
		strcpy(s,s11);
		}
	
	int cnt=0;
	char *s3[256],*s4[256],*s5[256];
	char *token;
	token=strtok(s," ");
	while(token!=NULL)
		{
		s3[cnt]=token;
		token=strtok(NULL," ");
		cnt++;		
		}
		
	i=0;
	while(*s3[i]!='|')
		{
		s4[i]=s3[i];
		i++;
		}
	s4[i]=NULL;
	int j=0;
	i++;
	while(*(s3+i)!=NULL)
		{
		s5[j]=s3[i];
		i++;
		j++;
		}
	s5[j]=NULL;
	
	char **a[]={s4,s5,NULL};
	pass1=0;
	pipeline(a);
	}

if(find(s,'<')==0 || find(s,'>')==0 || (find(s,'<')==0 && find(s,'>')==0) || pass1==1)
	{
	if(pass1==1)
		{
		strcpy(s,s2);
		}
	i=0;
	int rec=0,flag=0;
	while(s[i]!='\0')	    //Finding whether it is input redirection
		{		    // or output redirection,and a=">"
		if(s[i]==a)
			{
			rec=1;
			flag++;
			}
		if(s[i]=='<')
			{
			flag++;
			
			}
		i++;
		}
	
	if(flag==2)
		{
		i=0;
		int inc=0,k1=0,k2=3;
		s2[0]='w';s2[1]='c',s2[2]=' ';
		while(s[i]!='\0')
			{
			if(s[i]=='<')     //find the input and output directors
				{	//and storing in s1,s2 arrays
				inc++;
			    	}
			if(s[i]=='>')
				{
				inc++;	
				}
			if(inc<2)
				{
				s1[k1]=s[i];
				k1++;
				}
			if(inc>=1)
				{
				s2[k2]=s[i+2];
				k2++;
				}
			i++;
			}
		pass=1;
		s1[k1-1]='\0';
		s2[k2-2]='\0';
		
		printf("s1:%s s2:%s\n",s1,s2);
		}
	if(rec!=1 || pass==1)
		{
		if(pass==1)
			{
			strcpy(s,s1);
			}
		if(fork()==0)
			{
			char *token;
			token=strtok(s," ");
			char *c1[10],*cmp="<";
			int j=0;
			while(token!=NULL)
				{
				if(strcmp(token,cmp)==0)
					{
					token=strtok(NULL," ");
					continue;
					}
				c1[j]=token;
				j++;
				token=strtok(NULL," ");
				}
			c1[j]=token;
			j++;
			c1[j-1]=NULL;
			execvp(c1[0],c1);
			}
		else
			{
			wait(NULL);
			
			}
				
		}
	if(rec==1 || pass==1)
		{
		if(pass==1)
			{
			strcpy(s,s2);
			}
		if(fork()==0)
			{
			char *token;
			token=strtok(s," ");
			char *c1[10],*cmp=">";
			int j=0;
			while(token!=NULL)
				{
				if(strcmp(token,cmp)==0)
					{
					token=strtok(NULL," ");
					continue;
					}
				printf("t:%s\n ",token);
				
				c1[j]=token;
				j++;
				token=strtok(NULL," ");
				}
			c1[j]=token;
			j++;
			c1[j-1]=NULL;
			o=open(c1[j-2],O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
			dup2(o,1);
			execvp(c1[0],c1);
			}
		else
			{
			wait(NULL);
			close(o);
			}
		}
	}

}
}
