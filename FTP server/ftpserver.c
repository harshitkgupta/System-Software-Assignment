#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<signal.h>
#include"tcpservice.h"
#include<sys/socket.h>
#include<arpa/inet.h>
#define PORT 30000
#define QUEUE 1000
#ifndef BSIZE
#define BSIZE 5000
#endif

int listener_id;

int catch_signal(int signal,void (*handler)(int))
{
	struct sigaction action;
	action.sa_handler=handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags=0;
	return sigaction(signal,&action,NULL);
}


void handle_shutdown(int signal)
{
	if(listener_id)
		shutdown(listener_id,2);
	fprintf(stderr,"Bye...Shutting Down FTP Server");
	exit(EXIT_SUCCESS);
}


int main()
{

	int serr=catch_signal(SIGINT,handle_shutdown);
	if(serr==-1)
	{
		error("can't register Handle");
	}
	listener_id=open_listener_socket();

	bind_to_port(listener_id,PORT);
	int lerr=listen(listener_id,QUEUE);
	if(lerr==-1)
	{
		error("can't listen on socket");
	}

	struct sockaddr_storage client_address;
	unsigned int address_size=sizeof(client_address);
	fprintf(stderr,"Waiting for connection");

	while(1)
	{
		int connect_id=accept(listener_id,(struct sockaddr*)&client_address,&address_size);
		if(connect_id==-1)
		{
			error("can't open secondary socket");
		}
		pid_t pid=fork();
		if(pid==-1)
		{
			error("can't fork child process");
		}
		else if(pid==0)
		{
			char buffer[BSIZE];
			Command *cmd = malloc(sizeof(Command));

			close(listener_id);
			char welcome[] = " Welcome to FTP SERVER v1.0\r\n";
			say(connect_id,welcome);

			while (1)
			{
				say(connect_id,"ftp$ ");
				bzero(buffer,BSIZE);
				bzero(cmd->command,8);
				bzero(cmd->arg,40);
				bzero(cmd->arg1,40);
				hear(connect_id,buffer,BSIZE);
				puts(buffer);
				parse_command(buffer,cmd);

				switch(lookup_cmd(cmd->command))
				{
					case LS:
						if(cmd->arg[0]!='-')
							strcpy(cmd->arg,path_prefix(getenv("PWD"),cmd->arg));
						else
							strcpy(cmd->arg1,path_prefix(getenv("PWD"),cmd->arg1));
						ftp_ls(cmd,connect_id);
						break;
					case CD:strcpy(cmd->arg,path_prefix(getenv("PWD"),cmd->arg));
					ftp_cd(cmd,connect_id);
					break;
					case CHMOD:strcpy(cmd->arg1,path_prefix(getenv("PWD"),cmd->arg1));
					ftp_chmod(cmd,connect_id);
					break;
					case GET:strcpy(cmd->arg,path_prefix(getenv("PWD"),cmd->arg));
					puts("server in get\n");
					ftp_sget(cmd,connect_id);
					break;
					case PUT:strcpy(cmd->arg1,path_prefix(getenv("PWD"),cmd->arg1));
					puts("server in put\n");
					ftp_sput(cmd,connect_id);
					break;
					case CLOSE:ftp_close(cmd,connect_id);
					break;
					default:     printf( "wrong command\n");
					break;
				}

			}
			printf("Client disconnected.\n");
			exit(EXIT_SUCCESS);

		}
		else
		{
			close(connect_id);
		}
	}
	return 0;
}
