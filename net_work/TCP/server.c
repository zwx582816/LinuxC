#include "net.h"

int main(int argc, char *argv[])
{
    int sfd = -1;
    int bfd = -1;
    int lfd = -1;
    int afd = -1;
    struct sockaddr_in sin;
    //1. 创建socket
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    /*
    *2. 绑定
    *2.1 填充struct sockaddr_in结构体变量 
    *2.2 绑定
    * */
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
    bfd = bind(sfd, (struct sockaddr *)&sin, sizeof(sin));
    if (bfd < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    
    //3. 调用listen()把主动套接字变成被动套接字
    lfd = listen(sfd, BACKLOG);
    if (lfd == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    //4. 阻塞等待客户端连接请求
    afd = accept(sfd, NULL, NULL);
    if (afd == -1)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    //5. 读写
    //和afd进行数据的读写
    int ret = -1;
    char buf[BUFSIZ];
    while(1)
    {
         bzero(buf, BUFSIZ);
        do
        {
            ret = read(afd, buf, BUFSIZ-1);       
        }while(ret < 0 && EINTR == errno);
        if(ret < 0)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if(!ret)
        {
            break; //对方关闭
        }
        printf("Receive data: %s\n",buf);
        if (!strncasecmp(buf, QUIT_STR, strlen(QUIT_STR)))
        {
            printf("Client is exiting!\n");
            break;
        }
    }
    close(afd);
    close(sfd);
    return 0;
}
