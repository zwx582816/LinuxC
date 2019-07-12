#include "net.h"
int main(int argc, char *argv[])
{
    int sfd = -1;
    int bfd = -1;
    struct sockaddr_in sin;
    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    /*优化4：允许绑定地址快速重用*/
    int b_reuse = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &b_reuse, sizeof(int));
    bzero(&sin, sizeof(sin));
    bzero(&sin, sizeof(sin)); //填空地址清零
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SERV_PORT);//本地字节序转化为网络字节序端口号
    sin.sin_addr.s_addr = htonl(INADDR_ANY);  //能绑定在任意的IP地址上
    bfd = bind(sfd, (struct sockaddr *)&sin, sizeof(sin));
    if (bfd < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    printf("Server starting......OK\n");
    char buf[BUFSIZ];
    struct sockaddr_in cin;
    socklen_t addrlen = sizeof(cin);
    while(1)
    {
        bzero(buf, BUFSIZ);
        if (recvfrom(sfd, buf, BUFSIZ-1, 0, (struct sockaddr *)&cin, &addrlen) < 0)
        {
            perror("recvfrom");
            continue;
        }
        char ipv4_addr[16];
        if (!inet_ntop(AF_INET, (void *)&cin.sin_addr, ipv4_addr, sizeof(cin)))
        {
            perror("inet_ntop");
            exit(EXIT_FAILURE);
        }
        printf("Recived from(%s:%d), data %s",ipv4_addr,ntohs(cin.sin_port), buf);
        
        if (!strncasecmp(buf, QUIT_STR, strlen(QUIT_STR)))
        {
            printf("Client(%s:%d) is exiting!\n",ipv4_addr, ntohs(sin.sin_port));
        }
    }
    close(sfd);
    return 0;
}
