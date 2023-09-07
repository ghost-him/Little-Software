#include <trantor/net/TcpServer.h>
#include <trantor/utils/Logger.h>
#include <trantor/net/EventLoopThread.h>
#include <string>
#include <iostream>
#include <atomic>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include "protocolConverter.h"
#include "protocolBuilder.h"
#include <memory>
#include <fstream>

std::shared_ptr<ProtocolOption> option = std::make_shared<ProtocolOption>();

// 解析信息
ProtocolConverter converter;

unsigned char* data = new unsigned char[option->size()];

// 这里要手动输入接收文件的名字和类型
std::ofstream ofs("receive.mp4");

void init() {
    option->append(1, UINT, "版本号");
    option->append(1, UINT, "消息类型");
    option->append(4, UINT, "总长度");
    option->append(2, UINT, "消息组");
    option->append(4, UINT, "偏移量");
    option->append(3, SIZE, "消息长度");
    converter.load(option);
}

using namespace trantor;


int main() {
    init();

    LOG_DEBUG << "test start";
    Logger::setLogLevel(Logger::kTrace);
    EventLoopThread loopThread;
    loopThread.run();

    InetAddress addr(8888);

    TcpServer server(loopThread.getLoop(), addr, "test");
    // 新连接到一个fd
    server.setBeforeListenSockOptCallback([](int fd) {
        std::cout << "setBeforeListenSockOptCallback:" << fd << std::endl;
        });
    //  连到一个
    server.setAfterAcceptSockOptCallback([](int fd) {
        std::cout << "afterAcceptSockOptCallback:" << fd << std::endl;
        });

    // 如果一个信息发送过来了
    server.setRecvMessageCallback(
        [](const TcpConnectionPtr& connectionPtr, MsgBuffer* buffer) {
            
            LOG_DEBUG<<"recv callback!";
            // std::cout.write(buffer->peek(), buffer->readableBytes());
            std::cout << buffer->readableBytes() << std::endl;
            // 解析收到的数据
            converter.convert((unsigned char*)buffer->peek());
            // 将数据送到data数组中
            converter.get_data(data);

            // 获取偏移量
            int offset = converter.get_head<unsigned int>(4);
            // 如果是一个新的文件
            if (offset == 0) {
                // 执行新文件的初始化
            }

            std::cout << "已收到 " << converter.get_data_size() << " 长度的数据" << std::endl;
            ofs.seekp(offset);
            ofs.write(reinterpret_cast<char*>(data), converter.get_data_size());
            
            buffer->retrieveAll();
            
            connectionPtr->send("1", 1);
            // connectionPtr->forceClose();
        });

    server.setConnectionCallback([](const TcpConnectionPtr& connPtr) {
        if (connPtr->connected())
        {
            LOG_DEBUG << "New connection";
            connPtr->send("1", 1);
        }
        else if (connPtr->disconnected())
        {
            LOG_DEBUG << "connection disconnected";
        }
        });


    server.setIoLoopNum(3);
    server.start();
    loopThread.wait();

    return 0;
}