#include <trantor/net/TcpClient.h>
#include <trantor/utils/Logger.h>
#include <trantor/net/EventLoopThread.h>
#include <string>
#include <iostream>
#include <fstream>
#include <atomic>
#include "protocolBuilder.h"
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <chrono>
#include <filesystem>

using namespace trantor;

std::shared_ptr<ProtocolOption> option = std::make_shared<ProtocolOption>();


void init() {
    option->append(1, UINT, "版本号");
    option->append(1, UINT, "消息类型");
    option->append(4, UINT, "总长度");
    option->append(2, UINT, "消息组");
    option->append(4, UINT, "偏移量");
    option->append(3, SIZE, "消息长度");
}

// trantor框架限制，一次最多可以传2KB的长度，所以这里的数据大小不可以超过 2KB - 协议首部的长度（这里为15B） = 2033B
#define READ_COUNT 1024

int main() {
    std::cout << std::filesystem::current_path() << std::endl;
    
    init();

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
////////////////////////////////////////////////////////////////

    trantor::Logger::setLogLevel(trantor::Logger::kTrace);
    LOG_DEBUG << "TcpClient class test!";
    EventLoop loop;

    InetAddress serverAddr("127.0.0.1", 8888);

    std::shared_ptr<trantor::TcpClient> client;

    client = std::make_shared<trantor::TcpClient>(&loop,
        serverAddr,
        "tcpclienttest");

    client->setSockOptCallback([](int fd) {
        LOG_DEBUG << "setSockOptCallback!";

        int optval = 10;
        ::setsockopt(fd,
            SOL_TCP,
            TCP_KEEPCNT,
            &optval,
            static_cast<socklen_t>(sizeof optval));
        ::setsockopt(fd,
            SOL_TCP,
            TCP_KEEPIDLE,
            &optval,
            static_cast<socklen_t>(sizeof optval));
        ::setsockopt(fd,
            SOL_TCP,
            TCP_KEEPINTVL,
            &optval,
            static_cast<socklen_t>(sizeof optval));
        });

    client->setConnectionCallback(
        [&loop](const TcpConnectionPtr& conn) {
            if (conn->connected())
            {
                LOG_DEBUG << " connected!";

            }
            else
            {
                LOG_DEBUG << " disconnected";
                loop.quit();
            }
        });
    client->setMessageCallback(
        [&ifs, &data, &total_read_size,&builder, &fileSize, &start](const TcpConnectionPtr& conn, MsgBuffer* buf) {
            buf->retrieveAll();
            ifs.read(reinterpret_cast<char*>(data), READ_COUNT);
            // 本次读了的长度
            int read_size = ifs.gcount();
            // 生成协议
            builder.set_head(4, total_read_size);
            auto proto = builder.set_data(data, read_size);

            conn->send(proto->data(), proto->size());

            // 更新已经读取的长度
            total_read_size += read_size;
            if (total_read_size == fileSize) {
                std::cout << "已成功发送文件" << std::endl;
                conn->shutdown();
            }
            else if (total_read_size > fileSize) {
                std::cerr << "发送的文件大于原文件，程序错误" << std::endl;
                conn->shutdown();
            }

            auto now = std::chrono::system_clock::now();
            std::cout << "当前的速率(KB/s): " << (double)total_read_size / 1024 / (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() / 1000.0) << std::endl;
            std::cout << "已完成: " << ((double)total_read_size / fileSize) * 100 << "%" << std::endl;

        });
    client->connect();

    loop.loop();
    return 0;
}