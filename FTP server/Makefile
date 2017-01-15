all :ftpserver ftpclient
ftpserver:ftpserver.c tcpservice.c tcpservice.h
	gcc -Wall -o ftpserver ftpserver.c tcpservice.c -lm
ftpclient:ftpclient.c tcpservice.c tcpservice.h
	gcc -Wall -o ftpclient ftpclient.c tcpservice.c -lm

