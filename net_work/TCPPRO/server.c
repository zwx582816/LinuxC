#include <pthread.h>
#include <signal.h>
#include "net.h"

void cli_data_handle(void *arg);

void sig_child_handle(int signo)
{
    if(SIGCHLD == signo)
	{
	    waitpid(-1, NULL, WNOHANG);
	}
}

int main(int argc, char *argv[])
{
    int sfd = -1;
    int bfd = -1;
    int lfd = -1;
    int afd = -1;
    struct sockaddr_in sin;
	signal(SIGCHLD, sig_child_handle);
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

    /* 
	 * 优化1： 让服务器程序能绑定在任意的ip地址上
     * */

    sin.sin_addr.s_addr = htonl(INADDR_ANY);  //能绑定在任意的IP地址上
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
    printf("Server starting......OK\n");
    //4. 阻塞等待客户端连接请求
    struct sockaddr_in cin;
    socklen_t addrlen = sizeof(cin);
	while(1)
	{
		 pid_t pid = -1;
	     afd = accept(sfd, (struct sockaddr *)&cin, &addrlen);
		 if (afd < 0)
		 {
			 perror("accept");
			 break;
		 }
		 /*
		  * 创建一个子进程用于处理已建立连接的客户端的交互数据
		  * */
		 if ((pid = fork()) < 0)
		 {
		     perror("fork");
		     break;
		 }

		 if(0 == pid) //子进程
		 {
              close(sfd);
			  char ipv4_addr[16];
			  if (!inet_ntop(AF_INET, (void *)&cin.sin_addr, ipv4_addr, sizeof(cin)))
			  {
			       perror("inet_ntop");
				   exit(EXIT_FAILURE);
			  }
			  printf("Client(%s:%d) is connected!",ipv4_addr,ntohs(cin.sin_port));
			  cli_data_handle(&afd);
			  return 0;
		 }
		 else //父进程
		 {
             close(afd);

		 }
	}
    close(sfd);
    return 0;
}

void cli_data_handle(void *arg)
{
    int afd = *(int *)arg;
	printf("Child handling process:afd=%d\n", afd);
	int ret = -1;
	char buf[BUFSIZ];
	while(1)
	{
	    bzero(buf, BUFSIZ);
	    do
	    {
	        ret = read(afd, buf, BUFSIZ-1);
	    }while(ret < 0 && EINTR == errno);
	    if (ret < 0)
	    {
	        perror("read");
		    exit(EXIT_FAILURE);
	    }
	    if (!ret)
	    {
	        break;
	    }
	    printf("Receive data: %s\n",buf);
	    if (!strncasecmp(buf, QUIT_STR, strlen(QUIT_STR)))
	    {
	        printf("Client(sfd=%d) is exiting!\n",afd);
	        break;
	    }
	}
	close(afd);
}
