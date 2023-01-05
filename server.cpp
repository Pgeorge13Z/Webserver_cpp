#include <stdio.h>
#include <iostream>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>

int main()
{
    // 1.创建socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
    {
        perror("fail to creat socket");
        exit(-1);
    }

    // 2.绑定bind,地址和端口
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    inet_pton(AF_INET,"172.17.0.1",&saddr.sin_addr.s_addr);
    //saddr.sin_addr.s_addr = 0;
    int ret = bind(lfd, (sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1)
    {
        perror("fail to bind");
        exit(-1);
    }

    // 3.监听listen
    ret = listen(lfd, 20);
    if (ret == -1)
    {
        perror("fail to listen");
        exit(-1);
    }

    // 4.accept,接收客户端信息
    struct sockaddr_in clientaddr;
    socklen_t lens = sizeof(clientaddr);
    int cfd = accept(lfd, (sockaddr *)&clientaddr, &lens);
    if (cfd == -1)
    {
        perror("fail to accept");
        exit(-1);
    }

    // 5.输出客户端的信息
    char clientIP[16];
    inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
    uint16_t clientPort = ntohs(clientaddr.sin_port);
    printf("client ip is %s,port is %d\n", clientIP, clientPort);

    char recvbuf[1024];

    while (1)
    {
        // 6.获取客户端的数据
        int len = read(cfd, recvbuf, sizeof(recvbuf));
        if (len == -1)
        {
            perror("server fail to read");
            exit(-1);
        }
        else if (len > 0)
            std::cout << recvbuf << std::endl;
        else if (len == 0)
        {
            // 表示客户端断开连接
            printf("client closed");
            break;
        }

        // 7.给客户端返回数据
        const char *data = "hello , im server";
        write(cfd, data, strlen(data));
    }

    // 关闭文件描述符
    close(cfd);
    close(lfd);
}