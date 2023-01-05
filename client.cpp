#include <stdio.h>
#include <iostream>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    // 1.创建套接字
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cfd == -1)
        std::cerr << "client fail to create socket";

    // 2.connect server
    struct sockaddr_in serveaddr;
    serveaddr.sin_family = AF_INET;
    serveaddr.sin_port = htons(9999);
    inet_pton(AF_INET, "172.17.0.1", &serveaddr.sin_addr.s_addr);

    int ret = connect(cfd, (sockaddr *)&serveaddr, sizeof(serveaddr));
    if (ret == -1)
        std::cerr << "client connect fail" << std::endl;

    char recvbuf[1024];
    while (1)
    {

        // 3. 发送数据
        const char *data = "hello , im client";
        write(cfd, data, strlen(data));
        sleep(1);
        // 4. 接收数据

        int len = read(cfd, recvbuf, sizeof(recvbuf));
        if (len == -1)
        {
            perror("server fail to read");
            exit(-1);
        }
        else if (len > 0)
            printf("receive server data: %s\n", recvbuf);
        else if (len == 0)
        {
            // 表示服务器端断开连接
            printf("server closed");
            break;
        }
    }

    // 5. 关闭连接
    close(cfd);
}