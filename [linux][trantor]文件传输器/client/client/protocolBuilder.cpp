#include "protocolBuilder.h"

ProtocolBuilder::ProtocolBuilder() {
	_length = 0;
	_option = nullptr;
}

void ProtocolBuilder::load(std::shared_ptr<ProtocolOption> option) {
	_option = option;

	// 初始化前缀和数组
	_length = _option->_optionList.size();
	_sum.resize(_length + 1);
	// 计算前缀和数组
	for (int i = 1; i <= _length; i++) {
		_sum[i] = _sum[i - 1] + _option->_optionList[i - 1]._length;
	}
	// 开辟一个头标记长度的空间，给头标记缓冲区
	// _sum[_length]表示头标记的长度
	_head.reset(new unsigned char[_sum[_length]]);
}
// 设置数据内容
std::shared_ptr<Protocol> ProtocolBuilder::set_data(void* data, unsigned int size) {
	std::shared_ptr<Protocol> res = std::make_shared<Protocol>();
	// 计算总长度 总长度 = 首部长度 + 数据长度
	unsigned int total_size = _sum[_length] + size;
	// 初始化协议对象，赋予长度和空间
	res->_size = total_size;
	res->_data.reset(new unsigned char[total_size]);
	// 设置数据部分的长度
	set_head(_option->_size_index, total_size);
	// 复制头标记的内容
	memcpy(res->_data.get(), _head.get(), _sum[_length]);
	// 复制数据部分的内容
	memcpy(res->_data.get() + _sum[_length], data, size);
	return res;
}

