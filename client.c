#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define ONLINE "online"
#define BYE "bye"

int main()
{
    //创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    //向服务器（特定的IP和端口）发起请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    serv_addr.sin_port = htons(1234);  //端口
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    char buffer[40];
    //读取服务器传回的数据
    while(1)
    {
        read(sock, buffer, sizeof(buffer));
        printf("Message form server: %s\n", buffer);
        if(NULL != strstr(buffer, ONLINE))
        {
            break;
        }
        write(sock, buffer, sizeof(buffer));
    }

    pid_t pid = fork();
    if(pid < 0)
    {
        printf("Error in fork()!!!\n");
        exit(EXIT_FAILURE);
    }
    //while(1)
    {
    if(0 == pid)
    {
        while(1)
        {
            printf("Message send:");
            scanf("%s", buffer);
            write(sock, buffer, sizeof(buffer));
        }
    }
    else
    {
        while(1)
        {
            read(sock, buffer, sizeof(buffer));
            printf("Message receive:%s\n", buffer);
        }
    }
    }
    //关闭套接字
    close(sock);
    return 0;
}
