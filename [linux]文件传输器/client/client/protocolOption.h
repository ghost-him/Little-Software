#pragma once
#include <string>
#include <deque>
#include <bitset>
#include <cmath>
#include <cstring>

enum protocolType {
	UINT,
	INT,
	SIZE
};

class _Option {
public:
	protocolType _type;
	int _length;
	std::string _description;
};


class ProtocolOption {
	friend class ProtocolConverter;
	friend class ProtocolBuilder;
public:
	ProtocolOption();
	// 获取单次发送的最大长度
	unsigned int size();

	// 添加，
	// 占用的字节长度，信息类型，描述
	bool append(unsigned int length, protocolType type, std::string description = "");

private:
	// 控制信息
	std::bitset<1> _control;

	std::deque<_Option> _optionList;
	// 表明指定消息长度的下标
	unsigned int _size_index;
	// 表明单个消息的最大长度
	unsigned int _size;
};
