#pragma once
#include "protocolOption.h"
#include "protocol.h"

class ProtocolBuilder {
public:
	ProtocolBuilder();
	// 设置协议格式
	void load(std::shared_ptr<ProtocolOption> option);
	// 设置头标记
	// 下标从0开始
	template<class Data>
	void set_head(unsigned int index, Data data) {
		_Option& option = _option->_optionList[index];

		// 获取当前标记开始的位置
		unsigned int start = _sum[index];
		for (int i = 0; i < option._length; i++) {
			_head[start + i] = (data >> (8 * i)) & 0xff;
		}
	}
	// 设置数据内容
	std::shared_ptr<Protocol> set_data(void* data, unsigned int size);


private:
	std::shared_ptr<ProtocolOption> _option;

	// 头标记的个数
	unsigned int _length;
	// 头标记的启始位置
	std::deque<unsigned int> _sum;
	// 头标记缓冲区
	std::unique_ptr<unsigned char[]> _head;
};