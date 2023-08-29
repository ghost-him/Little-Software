#include "protocolConverter.h"

ProtocolConverter::ProtocolConverter() {
	_length = 0;
	_option = nullptr;
	_data = nullptr;
}

// 加载要解码的数据
void ProtocolConverter::convert(unsigned char* data) {
	_data = data;
}

// 加载协议规则
void ProtocolConverter::load(std::shared_ptr<ProtocolOption> option) {
	_option = option;
	// 初始化前缀和数组
	_length = _option->_optionList.size();

	_sum.resize(_length + 1);
	// 计算前缀和数组
	for (int i = 1; i <= _length; i++) {
		_sum[i] = _sum[i - 1] + _option->_optionList[i - 1]._length;
	}
}


void ProtocolConverter::get_data(void* destination) {
	// 计算数据部分的长度：总长度 - 首部的长度
	unsigned int data_size = get_head<unsigned int>(_option->_size_index) - _sum[_length];
	// 将内容复制出去
	memcpy(destination,  _data + _sum[_length], data_size);
}

unsigned int ProtocolConverter::get_data_size() {
	// 计算数据部分的长度：总长度 - 首部的长度
	unsigned int data_size = get_head<unsigned int>(_option->_size_index) - _sum[_length];
	return data_size;
}