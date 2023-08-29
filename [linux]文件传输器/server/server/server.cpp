#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "protocolConverter.h"
#include <fstream>

std::shared_ptr<ProtocolOption> option = std::make_shared<ProtocolOption>();

void init() {
    option->append(1, UINT, "版本号");
    option->append(1, UINT, "消息类型");
    option->append(4, UINT, "总长度");
    option->append(2, UINT, "消息组");
    option->append(4, UINT, "偏移量");
    option->append(3, SIZE, "消息长度");
}

int main() {
    init();

    //创建套接字
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //将套接字和IP、端口绑定
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    serv_addr.sin_port = htons(1234);  //端口
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    //进入监听状态，等待用户发起请求
    listen(serv_sock, 20);
    //接收客户端请求
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);

    int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    //向客户端发送数据
    //处理客户端的数据
    unsigned char* recv = new unsigned char[option->size()];
    
    ProtocolConverter converter;
    converter.load(option);

    unsigned char* data = new unsigned char[option->size()];


    std::ofstream ofs;
    ofs.open("receive.mp4");
    while (1) {
        // 读一下从客户端发来的数据
        int ret = read(clnt_sock, recv, option->size());
        if (ret == 0) {
            //关闭套接字
            break;
        }
        // 解析收到的数据
        converter.convert(recv);
        // 将数据送到data数组中
        converter.get_data(data);
        
        // 获取偏移量
        int offset = converter.get_head<unsigned int>(4);
        std::cout << offset << std::endl;
        // 如果是一个新的文件
        if (offset == 0) {
            // 执行新文件的初始化
        }

        std::cout << "已收到 " << converter.get_data_size() << " 长度的数据" << std::endl;

        ofs.seekp(offset);

        ofs.write(reinterpret_cast<char*>(data), converter.get_data_size());

    }


    //关闭套接字
    close(clnt_sock);
    close(serv_sock);
    return 0;
}