#include "protocol.h"

Protocol::Protocol() {
	_size = 0;
	_data.release();
}


Protocol::Protocol(const Protocol& pro) {
	init(pro);
}

const Protocol& Protocol::operator=(const Protocol& pro) {
	init(pro);
	return *this;
}

// 获取数据指针
void* Protocol::data() {
	return _data.get();
}
// 获取总体的长度
unsigned int Protocol::size() {
	return _size;
}

void Protocol::init(const Protocol& pro) {
	_size = pro._size;

	_data.reset(new unsigned char[_size]);
	// 复制数据
	for (int i = 0, siz = _size; i < siz; i++) {
		_data[i] = pro._data[i];
	}
}
