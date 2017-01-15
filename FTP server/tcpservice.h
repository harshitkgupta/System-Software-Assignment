
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <netinet/in.h>
#include <time.h>
#include <dirent.h>

#ifndef BSIZE
#define BSIZE 1024
#endif

typedef struct Command
{
	char command[8];
	char arg[40];
	char arg1[40];
} Command;
char * path_prefix(char * pwd,char *rpath);
typedef enum cmdlist
{
	LS,CD,CHMOD,LLS,LCD,LCHMOD,PUT,GET,CLOSE
} cmdlist;

void parse_command(char *, Command *);

int lookup_cmd(char *cmd);
int lookup(char *needle, const char **haystack, int count);

void ftp_ls(Command *, int );
void ftp_cd(Command *, int );
void ftp_chmod(Command *, int );
void ftp_lls(Command *, int );
void ftp_lcd(Command *, int );
void ftp_close(Command *, int );
void ftp_lchmod(Command *, int );
void ftp_sget(Command *, int );
void ftp_sput(Command *, int );
void ftp_cget(Command *, int );
void ftp_cput(Command *, int );

void error(char *msg);
int open_listener_socket();
int catch_signal(int signal,void (*handler)(int));
void handle_shutdown(int signal);
void bind_to_port(int socket,int port);
void connect_to_port(int socket,char* addres,int port);
int say(int sock,char *s);
int hear(int socket ,char *buf ,int len);
int strnicmp(const char *pStr1, const char *pStr2, size_t Count);
