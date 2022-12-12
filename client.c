#include <stdio.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#define BUFLEN 256

int main(void) {
	int 	n, bytes_to_read;
	int 	sd, port;
	struct 	hostent *hp;
	struct 	sockaddr_in server;
	char 	*host, *bp, rbuf[BUFLEN], sbuf[BUFLEN];
	int 	sockfd = 0;
	char 	buf[1024];
    int     listenfd, connfd;
    struct  sockaddr_in6 s_addr, c_addr; 
    char    sendBuff[100];
    int     len;
    char    ipv6_addr[16];
    int     pipefd[2];
    char    readPipe[145];

	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "Can't create a socket\n");
		exit(1);
	} 

    host = "3.17.53.130";
    port = atoi("50000");

	bzero((char *)&server, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	if ((hp = gethostbyname(host)) == NULL) {
		fprintf(stderr, "Can't get server's address\n");
		exit(1);
	}
	bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);

	if (connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1) {
		fprintf(stderr, "Can't connect\n");
		exit(1);
	} 

	// ID
	memset(buf,0x00,sizeof(buf));
    read(sd,buf,sizeof(buf));
    printf("%s",buf);
    memset(buf,0x00,sizeof(buf));
    read(sd,buf,sizeof(buf));
    printf("%s",buf);
    memset(buf,0x00,sizeof(buf));
    read(0,buf,sizeof(buf));
    write(sd,buf,strlen(buf)); 

	// IP address
    memset(buf,0x00,sizeof(buf));
    read(sd,buf,sizeof(buf));
    printf("%s",buf);
    memset(buf,0x00,sizeof(buf));
    read(sd,buf,sizeof(buf));
    printf("%s",buf);
    memset(buf,0x00,sizeof(buf));
    read(0,buf,sizeof(buf));
    write(sd,buf,strlen(buf)); 

	// PORT address
    memset(buf,0x00,sizeof(buf));
    read(sd,buf,sizeof(buf));
    printf("%s",buf);
    memset(buf,0x00,sizeof(buf));
    read(sd,buf,sizeof(buf));
    printf("%s",buf);
    memset(buf,0x00,sizeof(buf));
    read(0,buf,sizeof(buf));
    write(sd,buf,strlen(buf));

	// confirm
    memset(buf,0x00,sizeof(buf));
    read(sd,buf,sizeof(buf));
    printf("%s",buf);
    memset(buf,0x00,sizeof(buf));
    read(sd,buf,sizeof(buf));
    printf("%s",buf);
    memset(buf,0x00,sizeof(buf));
    read(0,buf,sizeof(buf));
    write(sd,buf,strlen(buf)); 

	// concurrent
    memset(buf,0x00,sizeof(buf));
    read(sd,buf,sizeof(buf));
    printf("%s",buf);
    memset(buf,0x00,sizeof(buf));
    read(sd,buf,sizeof(buf));
    printf("%s",buf);
    memset(buf,0x00,sizeof(buf));
    read(0,buf,sizeof(buf));
    write(sd,buf,strlen(buf)); 

	// connect
	memset(buf,0x00,sizeof(buf));
    read(sd,buf,sizeof(buf));
    printf("%s",buf);
    memset(buf,0x00,sizeof(buf));
    read(sd,buf,sizeof(buf));
    printf("%s",buf);
    memset(buf,0x00,sizeof(buf));
    read(0,buf,sizeof(buf));
    write(sd,buf,strlen(buf));
    memset(buf,0x00,sizeof(buf));

    if(pipe(pipefd) == -1)
    {
        printf("pipe error\n");
    }
    int pid = fork();
    if(pid == 0)
    {
        char token[144];
        listenfd = socket(AF_INET6, SOCK_STREAM, 0);
        memset(&s_addr, '0', sizeof(s_addr));
        memset(sendBuff, '\0', sizeof(sendBuff)); 

        s_addr.sin6_family = AF_INET6;
        s_addr.sin6_port = htons(50500); 

        inet_pton(AF_INET6, "2001:0:c38c:c38c:2c38:65d:2104:6935", (void *)&ipv6_addr);
        memcpy((void *)&s_addr.sin6_addr.s6_addr, (void *)&ipv6_addr, 16);


        if(bind(listenfd, (struct sockaddr_in6*)&s_addr, sizeof(s_addr)) == -1){
            printf("Can not Bind\n");
            return -1;
        } 

        if(listen(listenfd, 10) == -1){
            printf("listen Fail\n");
            return -1;
        } 
        len = sizeof(c_addr);
        connfd = accept(listenfd, (struct sockaddr*)&c_addr, &len); 
        memset(token,0x00,sizeof(token));
        read(connfd,token,sizeof(token));
        
        close(pipefd[0]);
        write(pipefd[1], token, strlen(token));
       
        close(connfd);
        close(listenfd);
        exit(0);
    }
    else if(pid > 0)
    {
        int returnStatus = 0;
        waitpid(pid, &returnStatus, 0);
        if(returnStatus != 0)
        {
            printf("child process error");
        }
        close(pipefd[1]);
        read(pipefd[0], readPipe, sizeof(readPipe));
    }
    
    readPipe[144] = 10;
    write(sd,readPipe,strlen(readPipe));
    memset(buf,0x00,sizeof(buf));
    read(sd,buf,sizeof(buf));
    printf("%s",buf);
    memset(buf,0x00,sizeof(buf));
    read(sd,buf,sizeof(buf));
    printf("%s",buf);
    memset(buf,0x00,sizeof(buf));
    read(sd,buf,sizeof(buf));
    printf("%s",buf);

	close(sd);
	return(0);
}
