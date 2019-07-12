/*
 *udp demo
 *usage:
 *./client serv_ip serv_port
 * */
#include "net.h"

void usage(char *s)
{
    printf("\nThis is udp demo!\n");
    printf("\nUsage:\n\t %s serv_ip serv_port",s);
    printf("\n\t serv_ip: udp server ip adress!");
    printf("\n\t serv_port: udp server port(serv_port > 5000)\n\n");
}

int main(int argc,char *argv[])
{
    int sfd = -1;
    int port = SERV_PORT;
    port = atoi(argv[2]);
    if(port < 5000)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in sin;
    if (argc != 3)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    bzero(&sin, sizeof(sin)); //填空地址清零
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);//本地字节序转化为网络字节序端口号
#if 0
    sin.sin_addr.s_addr = inet_addr(SERV_IP_ADDR); //IP地址转换函数
#else
    if (inet_pton(AF_INET, argv[1], &sin.sin_addr.s_addr) != 1) //IP地址转化函数
    {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

#endif
    printf("UDP client starting....OK!\n");    
    char buf[BUFSIZ];
    while(1)
    {
        fprintf(stderr,"Please input the string to server:");
        bzero(buf,BUFSIZ);
        if (fgets(buf, BUFSIZ-1, stdin) == NULL)
        {
            perror("fgets");
            continue;
        }
        sendto(sfd, buf, strlen(buf), 0, (struct sockaddr *)&sin, sizeof(sin));
        if (!strncasecmp(buf, QUIT_STR, strlen(QUIT_STR)))
        {
            printf("Client is exited!\n");
            break;
        }
    }
    close(sfd);
    return 0;

}
