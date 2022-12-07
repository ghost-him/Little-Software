/*
密码生成器
可以自定义密码的长度, 密码的种类, 密码的数量以及密码的输出的位置
*/

#include <iostream>
#include <random>
#include <string>
#include <fstream>
#include <chrono>

using namespace std;

// 密码的数量
int password_number = 1;
// 密码的长度
int password_length = 10;
// 密码的种类
// 大写, 小写, 数字
int password_type = 1;
// 输出文件的路径
string file_path;

ofstream ofs;

int main()
{
	cout << "请输入密码的长度" << endl;
	cin >> password_length;

	cout << "请输入密码的种类\n";
	cout << "1. 数字\n2. 小写字母\n3. 数字,小写字母混合\n4. 大写字母\n5. 大写字母,数字混合\n6. 小写字母,大写字母混合\n7. 数字,小写字母,大写字母混合" << endl;
	cin >> password_type;

	cout << "生成的密码的数量" << endl;
	cin >> password_number;

	cout << "是否输出到文件中(1为是)" << endl;
	int choice;
	cin >> choice;
	if (choice == 1) {
		cout << "文件路径: ";
		cin >> file_path;
		ofs.open(file_path);
		if (!ofs.is_open()) {
			cout << "文件打开失败, 当前的路径为: " << file_path << endl;
			return 0;
		}
	}

	default_random_engine e;
	e.seed(time(nullptr));

	int max_num = 0;
	// 如果选了数字
	if (password_type & 1)
		max_num += 9;
	// 如果选了字母
	if (password_type >> 1 & 1 || password_type >> 2 & 1)
		max_num += 23;

	uniform_int_distribution<int> pswd(0, max_num);
	uniform_int_distribution<int> upper(0, 1);

	string password;
	for (int i = 0; i < password_number; i++) {
		password.clear();
		for (int j = 0; j < password_length; j++) {
			int num = pswd(e);
			// 如果有数字
			if (password_type & 1) {
				if (num <= 9) {
					password += to_string(num);
					continue;
				}
				else {
					num -= 9;
				}
			}
			char add = num + 'a';
			// 如果启用了大写, 并且是大写
			if (password_type >> 2 & 1 && upper(e) == 1) {
				password.push_back(toupper(add));
			}
			else {
				password.push_back(add);
			}
		}
		if (choice == 1) {
			ofs << password << "\n";
		}
		else {
			cout << password << "\n";
		}
	}
	return 0;
}
