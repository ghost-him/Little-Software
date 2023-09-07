#pragma once
#include <bitset>
#include "protocolOption.h"
#include <memory>


class Protocol {
    friend class ProtocolConverter;
    friend class ProtocolBuilder;
public:
    Protocol();
    Protocol(const Protocol& pro);

    const Protocol& operator=(const Protocol& pro);

    // 获取数据指针
    void* data();
    // 获取总体的长度
    unsigned int size();

private:
    // 用于存储当前的数据
    std::unique_ptr<unsigned char[]> _data;
    // 当前的长度
    unsigned int _size;

    void init(const Protocol& pro);
};