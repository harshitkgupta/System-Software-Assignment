#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<signal.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<sys/sendfile.h>
#include<ctype.h>
#include<unistd.h>
#include<math.h>
#include"tcpservice.h"
extern char **environ;
#ifndef BSIZE
#define BSIZE 5000
#endif

static const char *cmdlist_str[] =
{
	"ls","cd","chmod","lls","lcd","lchmod","put","get","close"
};
void error(char *msg)
{
	fprintf(stderr,"%s %s\n",msg,strerror(errno));
	exit(EXIT_FAILURE);
}


int open_listener_socket()
{
	int s=socket(AF_INET,SOCK_STREAM,0);
	if(s==-1)
	{
		error("can't open socket");
	}
	return s;
}


void bind_to_port(int socket,int port)
{
	struct sockaddr_in saddress;
	bzero(&saddress,sizeof(saddress));
	saddress.sin_family=AF_INET;
	saddress.sin_port=(in_port_t)htons(port);
	saddress.sin_addr.s_addr=htonl(INADDR_ANY);
	int reuse =1;
	int r=setsockopt(socket,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
	if(r==-1)
	{
		error("can't use reuse option");
	}
	int b=bind(socket,(struct sockaddr*)&saddress,sizeof(saddress));
	if(b==-1)
	{
		error("can't bind to socket");
	}
}


void connect_to_port(int socket,char* address,int port)
{
	struct sockaddr_in server;
	bzero(&server,sizeof(server));
	server.sin_addr.s_addr=inet_addr(address);
	server.sin_family=AF_INET;
	server.sin_port=(in_port_t)htons(port);
	if(connect(socket,(struct sockaddr*)&server,sizeof(server))<0)
	{
		error("could not connect");
	}
}


int say(int socket,char *s)
{
	int r=write(socket,s,strlen(s));
	if(r==-1)
	{
		error("error talking to client");
	}
	return r;
}


int hear(int socket ,char *buf ,int len)
{
	int slen=len;
	int c=recv(socket,buf,len,0);
	while(c>0 && buf[c-1]!='\n')
	{
		buf+=c;
		len-=c;
		c=recv(socket,buf,len,0);
	}
	if(c<0)
	{
		return c;
	}
	else if(c==0)
	{
		buf[0]='\0';
	}
	else
	{
		buf[c-1]='\0';
	}
	return slen-len;
}


int lookup_cmd(char *cmd)
{
	const int cmdlist_count = sizeof(cmdlist_str)/sizeof(char *);
	return lookup(cmd, cmdlist_str, cmdlist_count);
}


int lookup(char *needle, const char **haystack, int count)
{
	int i;
	for(i=0;i<count; i++)
	{
		if(strnicmp(needle,haystack[i],sizeof(needle))==0)return i;
	}
	return -1;
}


void parse_command(char *cmdstring, Command *cmd)
{
	sscanf(cmdstring,"%s %s %s",cmd->command,cmd->arg,cmd->arg1);
}


int strnicmp(const char *pStr1, const char *pStr2, size_t Count)
{
	char c1, c2;
	int  v=0;

	if (Count == 0)
		return 0;
	do
	{
		c1 = *pStr1++;
		c2 = *pStr2++;

		v = tolower(c1) - tolower(c2);
	} while ((v == 0) && (c1 != '\0') && (--Count > 0));

	return v;
}


void ftp_ls(Command *cmd, int s)
{

	pid_t pid=fork();
	if(pid<0)
		{error("error in lls fork");}
		else if(pid==0)
	{
		dup2(s, 0);
		dup2(s, 1);
		dup2(s, 2);
		if(cmd->arg[0]=='-')
		{
			if(strlen(cmd->arg1)==0)
				strcpy(cmd->arg1,getenv("PWD"));
			execle("/bin/ls","ls",cmd->arg,cmd->arg1,NULL,environ);
		}
		else
		{
			if(strlen(cmd->arg)==0)
				strcpy(cmd->arg,getenv("PWD"));
			execle("/bin/ls","ls",cmd->arg,NULL,environ);
		}
	}
	else
	{

		wait(0);
	}
}


int decimal_octal(int n)
{
	int decimal=0, i=0, rem;
	while (n!=0)
	{
		rem = n%10;
		n/=10;
		decimal += rem*pow(8,i);
		++i;
	}
	return decimal;
}


void ftp_chmod(Command *cmd, int s)
{

	int i = decimal_octal(atoi(cmd->arg));

	int j=chmod(cmd->arg1,i);
	if(j==0)
		say(s,"Permission of file changed sucessfully\n");
	else
		say(s,"error in executing file permision\n");
}


void ftp_lls(Command *cmd, int s)
{
	pid_t pid=fork();
	if(pid<0)
		{error("error in lls fork");}
		else if(pid==0)
	{
		//puts(cmd->arg);
		if(cmd->arg[0]=='-')
		{
			if(strlen(cmd->arg1)==0)
				strcpy(cmd->arg1,getenv("PWD"));
			execle("/bin/ls","ls",cmd->arg,cmd->arg1,NULL,environ);
		}
		else
		{
			if(strlen(cmd->arg)==0)
				strcpy(cmd->arg,getenv("PWD"));
			execle("/bin/ls","ls",cmd->arg,NULL,environ);
		}
	}
	else
	{
		wait(0);
	}

}


void ftp_cd(Command *cmd, int s)
{
	if(chdir(cmd->arg)==0)
	{
		setenv("PWD",cmd->arg,1);
		say(s,"Directory successfully changed.\n");
	}
	else
	{
		say(s," Failed to change directory.\n");
	}
}


void ftp_lcd(Command *cmd, int s)
{
	if(chdir(cmd->arg)==0)
	{
		setenv("PWD",cmd->arg,1);
		puts("Directory successfully changed.\n");
	}
	else
	{
		puts(" Failed to change directory.\n");
	}
}


void ftp_close(Command *cmd, int s)
{
	puts("Good Bye Client");
	sleep(1);
	close(s);
	exit(1);

}


void ftp_lchmod(Command *cmd, int s)
{
	int i = decimal_octal(atoi(cmd->arg));
	int j=chmod(cmd->arg1,i);
	if(j==0)
		puts("Permission of file changed sucessfully\n");
	else
		puts("error in executing file permision\n");

}


void ftp_sput(Command *cmd, int s)
{
	FILE *fp=fopen(cmd->arg1,"w");
	long int filesize;
	int i=0;
	char buffer[5000];
	bzero(buffer,5000);
	read(s,buffer,5000);
	puts(buffer);
	if(strncmp(buffer,"pass",4)==0)
	{
		say(s,"size");
		bzero(buffer,5000);
		puts("reading size\n");
		read(s,buffer,100);
		puts(buffer);
		filesize=atoi(buffer);
		printf("size=%ld\n",filesize);
		say(s,"CTS");
		puts("reading file\n");
		while(filesize>0)
		{
			printf("in loop %d rem=%ld\n",i++,filesize);
			bzero(buffer,5000);
			read(s,buffer,5000);
			//size=strlen(buffer);
			fwrite(buffer,1,(filesize<5000)?filesize:5000,fp);
			filesize-=5000;
		}
		bzero(buffer,5000);
		fclose(fp);
		say(s,"recv");
		puts("transfer done");
	}
	else
	{
		puts("error in opening file\n");
	}
	//while(hear(s,buffer,5000)>0)

	//hear(s,buffer,5000);

	//puts(buffer);
}


void ftp_cput(Command *cmd, int s)
{
	int fd;
	struct stat stat_buf;
	off_t offset = 0;
	int sent_total = 0;
	fd = open(cmd->arg,O_RDONLY);
	char buff[5000];
	char sizestr[100];
	if(fd>0)
	{
		fstat(fd,&stat_buf);
		puts("before pass\n");
		say(s, "pass");
		read(s,buff,5000);
		puts(buff);
		bzero(sizestr,100);
		sprintf(sizestr,"%ld",(long int)stat_buf.st_size);
		puts(sizestr);

		say(s,sizestr);
		bzero(buff,5000);
		read(s,buff,50);
		bzero(buff,5000);
		sent_total = sendfile(s, fd, &offset, stat_buf.st_size);
		printf("sent_total= %d stat_size=%d\n",sent_total,(int)stat_buf.st_size);
		puts(strerror(errno));
		read(s,buff,5000);
		if(strncmp(buff,"recv",4)==0)
		{
			if(sent_total != stat_buf.st_size)
			{
				puts("error in file read");
				say(s,"error");
				exit(EXIT_SUCCESS);
			}
			else
				puts("file sent");
			//say(s,"sent");
		}
		else
		{
			puts("file not received\n");
		}
	}
	else
	{
		puts("fail");
		say(s," Failed to read file.\n");
	}

	close(fd);
}


void ftp_sget(Command *cmd, int s)
{
	int fd;
	struct stat stat_buf;
	off_t offset = 0;
	int sent_total = 0;
	fd = open(cmd->arg,O_RDONLY);
	char buff[5000];
	char sizestr[100];

	if(fd>0)
	{
		fstat(fd,&stat_buf);

		say(s, "pass");

		read(s,buff,5000);
		puts(buff);
		bzero(sizestr,100);
		sprintf(sizestr,"%ld",(long int)stat_buf.st_size);
		puts(sizestr);

		say(s,sizestr);
		bzero(buff,5000);
		read(s,buff,50);
		bzero(buff,5000);
		sent_total = sendfile(s, fd, &offset, stat_buf.st_size);
		printf("sent_total= %d stat_size=%d\n",sent_total,(int)stat_buf.st_size);
		puts(strerror(errno));
		read(s,buff,5000);
		if(strncmp(buff,"recv",4)==0)
		{
			if(sent_total != stat_buf.st_size)
			{
				puts("error in file read");
				say(s,"error");
				exit(EXIT_SUCCESS);
			}
			else
				puts("file sent");
			say(s,"sent");
		}
		else
		{
			puts("file not received\n");
		}
	}
	else
	{
		puts("fail");
		say(s," Failed to read file.\n");
	}

	close(fd);

}


void ftp_cget(Command *cmd, int s)
{
	FILE *fp=fopen(cmd->arg1,"wb");
	int i=0;
	long int filesize;
	char buffer[5000];
	bzero(buffer,5000);
	read(s,buffer,5000);
	puts(buffer);
	if(strncmp(buffer,"pass",4)==0)
	{
		say(s,"size");
		bzero(buffer,5000);
		puts("reading size\n");
		read(s,buffer,100);
		puts(buffer);
		filesize=atoi(buffer);
		printf("size=%ld\n",filesize);
		say(s,"CTS");
		puts("reading file\n");
		while(filesize>0)
		{
			printf("in loop %d rem=%ld\n",i++,filesize);
			bzero(buffer,5000);
			read(s,buffer,(filesize<5000)?filesize:5000);
			//size=strlen(buffer);
			fwrite(buffer,1,(filesize<5000)?filesize:5000,fp);
			filesize-=5000;
		}
		bzero(buffer,5000);
		fclose(fp);
		say(s,"recv");
		puts("transfer done");

	}
	else
	{
		puts("error in opening file\n");
	}
	//while(hear(s,buffer,5000)>0)

	//hear(s,buffer,5000);
	//puts(buffer);
}


char * path_prefix(char * pwd,char *rpath)
{
	if(rpath[0]!='/')
		return (strcat(strcat(pwd,"/"),rpath));
	else
		return rpath;
}
