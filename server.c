#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <signal.h>

#define BUFLEN 256

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0, len;
    struct sockaddr_in6 s_addr, c_addr; 
    char sendBuff[100];
    int sockfd = 0, n = 0;
    struct sockaddr_in6 serv_addr; 
    char recvBuff[100];
    char ipv6_addr[16];
    int new_sd;
    char *bp, buf[BUFLEN];
    int	bytes_to_read;
    int pipefd[2];
    char readPipe[100];
    char readPipe2[100];
    char readPipe3[100];
    char readPipe4[100];
    char readPipe5[100];
    char token[145];
    int cnt = 0;

    listenfd = socket(AF_INET6, SOCK_STREAM, 0);
    memset(&s_addr, '0', sizeof(s_addr));
    memset(sendBuff, '\0', sizeof(sendBuff)); 

    s_addr.sin6_family = AF_INET6;
    s_addr.sin6_port = htons(50100); 

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

    signal(SIGCHLD, SIG_IGN);

    while(cnt != 5)
    {
        if(pipe(pipefd) == -1)
        {
            printf("pipe error\n");
        }
        char ch;
        len = sizeof(c_addr);
        connfd = accept(listenfd, (struct sockaddr_in6*)&c_addr, &len); 
        int pid = fork();
        if(pid == 0)
        {
            char Buff[100];
            close(listenfd);
            read(connfd,Buff,sizeof(Buff));
            sendBuff[strlen(Buff)-1] = '\0';
            printf("%s",Buff);

            close(pipefd[0]);
            write(pipefd[1], Buff, strlen(Buff));

            close(connfd);
            exit(0);
        }
        else{
            close(connfd);
            if(cnt == 0)
            {
                close(pipefd[1]);
                read(pipefd[0], readPipe, sizeof(readPipe));
            }
            else if(cnt == 1)
            {
                close(pipefd[1]);
                read(pipefd[0], readPipe2, sizeof(readPipe2));
            }
            else if(cnt == 2)
            {
                close(pipefd[1]);
                read(pipefd[0], readPipe3, sizeof(readPipe3));
            }
            else if(cnt == 3)
            {
                close(pipefd[1]);
                read(pipefd[0], readPipe4, sizeof(readPipe4));
            }
            else if(cnt == 4)
            {
                close(pipefd[1]);
                read(pipefd[0], readPipe5, sizeof(readPipe5));
            }
        }
        cnt++;
    }
    close(listenfd);

    for(int i=0; i<28; i++)
    {
        token[i] = readPipe[i];
    } 
    token[28]=',';

    for(int i=29; i<57; i++)
    {
        token[i] = readPipe2[i-29];
    }
    token[57]=',';

    for(int i=58; i<86; i++)
    {
        token[i] = readPipe3[i-58];
    }
    token[86]=',';

    for(int i=87; i<115; i++)
    {
        token[i] = readPipe4[i-87];
    }
    token[115]=',';

    for(int i=116; i<144; i++)
    {
        token[i] = readPipe5[i-116];
    }
    token[144]='\0';
    
    printf("token = %s\n", token);

    // pass token to client
    struct sockaddr_in6 svrsin6;
    sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd < 0){
        perror("socket create error:");
        exit(0);
    }

    svrsin6.sin6_family   = AF_INET6;
    svrsin6.sin6_flowinfo = 0;
    svrsin6.sin6_port     = htons(50500);

    inet_pton(AF_INET6, "2001:0:c38c:c38c:2c38:65d:2104:6935", (void *)&ipv6_addr);
    memcpy((void *)&svrsin6.sin6_addr, (void *)&ipv6_addr, 16);

    if(connect(sockfd, (struct sockaddr *)&svrsin6, sizeof(svrsin6)) < 0){
        perror("connect error:");
        exit(0);
    }
    write(sockfd, token, strlen(token));
    close(sockfd);
    
    return 0;
}
