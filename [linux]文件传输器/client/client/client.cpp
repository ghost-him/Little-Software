#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include "protocolBuilder.h"
#include <filesystem>
#include <fstream>
#include "protocolConverter.h"
#include <thread>

std::shared_ptr<ProtocolOption> option = std::make_shared<ProtocolOption>();

void init() {
    option->append(1, UINT, "版本号");
    option->append(1, UINT, "消息类型");
    option->append(4, UINT, "总长度");
    option->append(2, UINT, "消息组");
    option->append(4, UINT, "偏移量");
    option->append(3, SIZE, "消息长度");
}

#define READ_COUNT 1024 * 32

int main() {
    std::cout << std::filesystem::current_path() << std::endl;
    
    init();
    //创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    //向服务器（特定的IP和端口）发起请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    serv_addr.sin_port = htons(1234);  //端口
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    std::cout << "连接成功" << std::endl;
    ////////////////////////////////////////
    ProtocolBuilder builder;
    builder.load(option);

    // 设置头标记
    builder.set_head(0, 1);     // 版本号          1
    builder.set_head(1, 1);     // 消息类型        1
    builder.set_head(3, 0);     // 消息组          0

    unsigned char* data = new unsigned char[option->size()];

    // 输入路径
    std::string path;
    std::cin >> path;

    std::ifstream ifs;

    ifs.open(path, std::ios::binary);
    if (!ifs.is_open()) {
        std::cerr << "打开失败" << std::endl;
        return 0;
    }
    // 获取文件大小
    ifs.seekg(0, std::ios::end);
    std::streampos fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    std::cout << "文件总长度:" << fileSize << std::endl;
    // 设置消息的总长度
    builder.set_head(2, fileSize);

    // 已经读了的长度(下一次发送时的偏移量)
    unsigned int total_read_size = 0;

    auto start = std::chrono::system_clock::now();
    

    while (1) {
        
        ifs.read(reinterpret_cast<char*>(data), READ_COUNT);
        // 本次读了的长度
        int read_size = ifs.gcount();
        // 生成协议
        builder.set_head(4, total_read_size);
        auto proto = builder.set_data(data, read_size);

        // 发送消息
        int ret = write(sock, proto->data(), proto->size());
        std::cout << "发送 " << proto->size() << " 的数据" << std::endl;

        if (ret == 0) {
            std::cerr << "已关闭连接" << std::endl;
            break;
        }
        // 更新已经读取的长度
        total_read_size += read_size;
        if (total_read_size == fileSize) {
            std::cout << "已成功发送文件" << std::endl;
            break;
        }
        else if (total_read_size > fileSize){
            std::cerr << "发送的文件大于原文件，程序错误" << std::endl;
            break;
        }

        auto now = std::chrono::system_clock::now();
        std::cout << "当前的速率(B/s): " << (double)total_read_size / std::chrono::duration_cast<std::chrono::seconds>(now - start).count() << std::endl;
        std::cout << "已完成: " << ((double)total_read_size / fileSize) * 100 << "%" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    //关闭套接字
    close(sock);
    return 0;
}