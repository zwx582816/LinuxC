#include "net.h"

int main(int argc, char *argv[])
{
    int sfd = -1;
    int cfd = -1;
    struct sockaddr_in sin;
    //1. 创建socket
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    // 2. 链接服务器
    // 2.1 填充struct sockaddr_in结构体变量
    
    bzero(&sin, sizeof(sin)); //填空地址清零
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SERV_PORT);//本地字节序转化为网络字节序端口号
#if 0
    sin.sin_addr.s_addr = inet_addr(SERV_IP_ADDR); //IP地址转换函数
#else
    if (inet_pton(AF_INET, SERV_IP_ADDR, &sin.sin_addr.s_addr) != 1) //IP地址转化函数
    {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

#endif

    cfd = connect(sfd, (struct sockaddr *)&sin, sizeof(sin));
    if (cfd < 0)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    
    //3. 写数据
    char buf[BUFSIZ];
    while(1) {
        bzero(buf,BUFSIZ);
        if (fgets(buf, BUFSIZ-1, stdin) == NULL)
        {
            continue;
        }
    
        write(sfd, buf, strlen(buf));
        if (!strncasecmp(buf, QUIT_STR, strlen(QUIT_STR)))
        {   
            printf("Client is exiting!\n");
            break;
        }
    }
    close(sfd);
    return 0;
}


