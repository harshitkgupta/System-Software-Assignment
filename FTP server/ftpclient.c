#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<signal.h>
#include"tcpservice.h"
#include<sys/socket.h>
#include<arpa/inet.h>

#include <sys/types.h>

#include <netinet/in.h>
#include <netdb.h>
#ifndef BSIZE
#define BSIZE 5000
#endif

int main(int argc,char* argv[])
{
	int sockfd, portno, n;

	struct sockaddr_in serv_addr;
	struct hostent *server;
	Command *cmd = malloc(sizeof(Command));

	char buffer[5000];
	if (argc < 3)
	{
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	server = gethostbyname(argv[1]);
	if (server == NULL)
	{
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
		error("ERROR connecting");

	while(1)
	{
		//memset(server_reply,0,BSIZE);
		sleep(1);
		bzero(buffer,5000);
		bzero(cmd->command,8);
		bzero(cmd->arg,40);
		bzero(cmd->arg1,40);
		//write(1,"waiting for prompt \n",19);
		n = read(sockfd,buffer,5000);
		if (n < 0)
			error("ERROR reading from socket");
		printf("%s",buffer);
		bzero(buffer,256);
		fgets(buffer,255,stdin);
		parse_command(buffer,cmd);
		//puts("I am here\n");
		switch(lookup_cmd(cmd->command))
		{
			case LS:			 //puts("In ls\n");
				say(sockfd,buffer);
				bzero(buffer,5000);
				n = read(sockfd,buffer,5000);
				if (n < 0)
					error("ERROR reading from socket");
				puts(buffer);
				break;
			case CD:say(sockfd,buffer);
			bzero(buffer,5000);
			/*n = read(sockfd,buffer,5000);
			if (n < 0)
				error("ERROR reading from socket");
			printf("%s\n",buffer);
			break;*/
			break;
			case CHMOD:say(sockfd,buffer);
			bzero(buffer,5000);
			/*n = read(sockfd,buffer,5000);
			if (n < 0)
				error("ERROR reading from socket");
			puts(buffer);
			*/
			break;

			case LLS:if(cmd->arg[0]!='-')
			strcpy(cmd->arg,path_prefix(getenv("PWD"),cmd->arg));
			else
				strcpy(cmd->arg1,path_prefix(getenv("PWD"),cmd->arg1));
			ftp_lls(cmd,sockfd);
			say(sockfd,"\n");
			break;
			case LCD:strcpy(cmd->arg,path_prefix(getenv("PWD"),cmd->arg));
			ftp_lcd(cmd,sockfd);
			say(sockfd,"\n");
			break;
			case LCHMOD:strcpy(cmd->arg1,path_prefix(getenv("PWD"),cmd->arg1));
			ftp_lchmod(cmd,sockfd);
			say(sockfd,"\n");
			break;
			case GET:strcpy(cmd->arg1,path_prefix(getenv("PWD"),cmd->arg1));
			//puts("in get\n");
			say(sockfd,buffer);
			sleep(1);
			ftp_cget(cmd,sockfd);

			break;
			case PUT:strcpy(cmd->arg,path_prefix(getenv("PWD"),cmd->arg));
			//puts("in put\n");
			say(sockfd,buffer);
			sleep(1);
			ftp_cput(cmd,sockfd);

			break;
			case CLOSE:
				//puts("in close\n");
				ftp_close(cmd,sockfd);

				break;
			default:
				say(sockfd,"\n");
				puts( "wrong command\n");
				break;
		}
		//sleep(1);

	}
	return 0;
}
