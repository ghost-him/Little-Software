/**
 * @author ghost_him
 */

#include "Sql_Connector.h"
#include "Sql_Controller.h"
#include <iostream>

using namespace std;

int main() {
    // 生成一个控制器
    Sql_Controller controller(
            "127.0.0.1",
            "ghosthim",
            "123456",
            "study"
            );

    // 获取控制器的连接器
    auto connector = controller.get_connector();

    //初始化连接器
    connector->startInit();
    // 连接并判断连接的情况
    if (!connector->connect()) {
        cout << "连接失败" << endl;
        cout << connector->get_error() << endl;
    }

    // 执行并判断连接的情况
    if (!controller.execute_sql("select * from test_table")) {
        cout << controller.get_error() << endl;
    }

    // 遍历执行的结果
    while (1) {
        auto row = controller.get_row();
        for (auto i : row) {
            // first 是 字符串的内容， second 是字符串的长度
            std::cout << i.first << " ";
        }

        // 获取表字段
        auto field = controller.get_field();
        for (auto i : field) {
            cout << i << " ";
        }
        cout << endl;

        if (controller.is_end()) break;
    }

    // 查看影响了多少行
    cout << controller.get_affected_rows_size() << " rows" << endl;

    // 查看表字段的数量
    cout << controller.get_field_size() << " fields" << endl;

    // 插入2个数据
    for (int i = 0; i < 2; i ++) {
        if(!controller.execute_sql("insert `test_table` (`name`,`path`) values('test_jpg', '/usr/ghosthim')")){
            cout << "error " << controller.get_error() << endl;
        }

        cout << controller.get_affected_rows_size() << " all rows" << endl;
    }

    cout << controller.get_field_size() << " fields" << endl;


}