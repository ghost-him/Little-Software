///
///	最低c++标准：17
/// 
/// 功能：查找一个文件夹下所有文件中包含了指定关键字的文件。
/// 输入：目标文件夹， 目标关键字
/// 输出：所有包含了这个关键字的文件路径
/// 
/// 

#include <iostream>
#include <string>
#include <algorithm>
#include <filesystem>
#include <fstream>

using namespace std;
namespace fs = std::filesystem;

string filePath;
wstring keyWord;

bool check(const fs::path& textPath) {
    wstring line;
    wifstream wifs(textPath);

    if (!wifs.is_open())
        return false;

    while (getline(wifs, line)) {
        if (line.find(keyWord) != wstring::npos) {
            return true;
        }
    }
    return false;
}

void find_files_with_keyword(const fs::path& directoryPath) {
    // 遍历子文件夹及其子文件，忽略权限不足
    for (const auto& entry : fs::recursive_directory_iterator(directoryPath, fs::directory_options::skip_permission_denied)) {
        if (!fs::is_directory(entry)) {
            // 检查文件是否包含关键字
            if (check(entry.path())) {
                cout << entry.path().string() << endl; // 需要将路径转换为string进行输出
            }
        }
    }
}

int main() {
    cout << "输入文件夹路径" << endl;
    getline(cin, filePath);
    cout << "输入目标关键字" << endl;
    wcin.ignore(); // 忽略前一个输入后的换行符
    getline(wcin, keyWord);

    find_files_with_keyword(filePath);

    return 0;
}