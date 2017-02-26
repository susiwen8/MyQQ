#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BYE "bye"

int main(){
    int pfd_c_f[2], pfd_f_c[2];
    if(-1 == pipe(pfd_c_f))
    {
        perror("Error in pipe(c_f)\n");
    }
    if(-1 == pipe(pfd_f_c))
    {
        perror("Error in pipe(f_c)\n");
    }
    const char online[] = "Friend online!";
    //创建套接字
    int serv_sock, clnt_sock; 
    const char child[] = "Child server!";
    const char father[] = "Father server!";
    char buffer_child[100];
    char buffer_father[100];
    //将套接字和IP、端口绑定
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    serv_addr.sin_port = htons(1234);  //端口
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    pid_t pid = fork();
    if(pid < 0)
    {
        printf("Error in fork()!!!\n");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        if(0 == pid)//child server
        {
            //进入监听状态，等待用户发起请求
            listen(serv_sock, 20);
            //接收客户端请求
            clnt_addr_size = sizeof(clnt_addr);
            clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);//有客户端连接
            printf("\nChild server has a client!\n");
            write(clnt_sock, child, sizeof(child));//把服务器代码发给连接客户
            read(clnt_sock, buffer_child, sizeof(buffer_child));//避免出BUG
            close(pfd_c_f[0]);
            write(pfd_c_f[1], online, sizeof(online));//告知另一个服务器，此服务器有一个客户端连接
            close(pfd_f_c[1]);
            read(pfd_f_c[0], buffer_child, sizeof(buffer_child));
            write(clnt_sock, buffer_child, sizeof(buffer_child));
            read(clnt_sock, buffer_child, sizeof(buffer_child));

            while(1)
            {
                read(clnt_sock, buffer_child, sizeof(buffer_child));
                close(pfd_c_f[0]);
                write(pfd_c_f[1], buffer_child, sizeof(buffer_child));
                close(pfd_f_c[1]);
                read(pfd_f_c[0], buffer_child, sizeof(buffer_child));
                write(clnt_sock, buffer_child, sizeof(buffer_child));
            }

        }
        else//father server
        {
            //进入监听状态，等待用户发起请求
            listen(serv_sock, 20);
            //接收客户端请求
            clnt_addr_size = sizeof(clnt_addr);
            clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
            printf("\nFather server has a client!\n");
            write(clnt_sock, father, sizeof(father));
            close(pfd_c_f[1]);
            read(pfd_c_f[0], buffer_father, sizeof(buffer_father));
            write(clnt_sock, buffer_father, sizeof(buffer_father));
            read(clnt_sock, buffer_father, sizeof(buffer_father));
            close(pfd_f_c[0]);
            write(pfd_f_c[1], online, sizeof(online));
        }

        while(1)
        {
            close(pfd_c_f[1]);
            read(pfd_c_f[0], buffer_father, sizeof(buffer_father));
            write(clnt_sock, buffer_father, sizeof(buffer_father));
            read(clnt_sock, buffer_father, sizeof(buffer_father));
            close(pfd_f_c[0]);
            write(pfd_f_c[1], buffer_father, sizeof(buffer_father));
        }
    }
    //关闭套接字
    close(clnt_sock);
    close(serv_sock);
    return 0;
}
