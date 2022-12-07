#include "Sql_Connector.h"

class Sql_Controller {
public:
    /*
     * 构造器
     */
    Sql_Controller();
    /*
     * 带有参数的构造器
     */
    Sql_Controller(const std::string & host,
                   const std::string & user,
                   const std::string & password,
                   const std::string & database,
                   unsigned int port = 3306,
                   const std::string & unix_socket = "0",
                   unsigned long flags = 0);

    /*
     * 获取连接器
     */
    std::shared_ptr<Sql_Connector> get_connector();

    /*
     * 执行sql语句,字符串
     */
    bool execute_sql(const std::string &);

    /*
     * 执行sql语句
     */
    bool execute_sql(const char *, unsigned int);

    /*
     * 获取一条数据，再次执行则输出下一行数据
     * <数据内容，长度>
     */
    std::vector<std::pair<std::string, unsigned long>> get_row();

    /*
     * 判断是否到了结尾
     */
    bool is_end();

    /*
     * 获取影响的行数
     */
    unsigned int get_affected_rows_size();

    /*
     * 获取插入的id
     */
    unsigned int get_insert_id();

    /*
     * 获取表字段
     */
    std::vector<std::string> get_field();

    /*
     * 获取表字段的长度
     */
    unsigned int get_field_size();

    /*
     * 获取上一次执行的sql
     */
    std::string get_sql();

    /*
     * 获取执行时产生的错误
     */
    std::string get_error();

    /*
     * 是否发生了错误
     */
    bool is_error();
private:
    // sql连接器
    std::shared_ptr<Sql_Connector> _connector;
    // sql指针
    std::shared_ptr<MYSQL> _mysql;
    // 上一次执行的sql语句
    std::string _sql;
    // 上一次执行的sql语句的长度
    unsigned int _sql_length;
    // 结果集
    MYSQL_RES* _result;
    // 一行的结果
    MYSQL_ROW _row;
    // 错误
    std::string _error;
    // 是否存在错误
    bool _is_error;
};