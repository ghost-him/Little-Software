#pragma once

#include <string>
#include <vector>
#include <mysql/mysql.h>
#include <memory>
#include <iostream>


// 用于连接mariadb
class Sql_Connector {
public:
    Sql_Connector();
    ~Sql_Connector();
    /*
     * 设置数据库的连接参数
     */
    Sql_Connector(const std::string & host,
                  const std::string & user,
                  const std::string & password,
                  const std::string & database,
                  unsigned int port = 3306,
                  const std::string & unix_socket = "0",
                  unsigned long flags = 0);

    /*
     * 初始化数据库的连接器
     */
    void startInit();

    /*
     * 设置数据库的属性
     * 连接数据库
     */
    bool connect();

    /*
     * 获取上一次操作的错误信息
     */
    std::string get_error();

    /*
     * 获取是否存在错误
     */
    bool is_error();

    /*
     * 测试数据库的连接
     */
    bool test_connect();

    /*
     * 获取 mysql 的指针
     */
    std::shared_ptr<MYSQL> get_sql_ptr();

    /*
     * 设置属性
     */
    void set_host(const std::string &);
    void set_user(const std::string &);
    void set_password(const std::string &);
    void set_database(const std::string &);
    void set_port(unsigned int);
    void set_unix_socket(const std::string &);
    void set_flags(unsigned long);

    void set_connect_timeout_time(int);
    void set_reconnect_time(int);

    /*
     * 获取属性
     */
    std::string get_host();
    std::string get_user();
    std::string get_password();
    std::string get_database();
    unsigned int get_port();
    std::string get_unix_socket();
    unsigned long get_flags();

    int get_connect_timeout_time();
    int get_reconnect_time();

private:
    // mysql的指针
    std::shared_ptr<MYSQL> _mysql;
    // 错误信息
    std::string error;
    // 是否存在错误
    bool _is_error;

    std::string _host;          // 地址
    std::string _user;          // 用户
    std::string _password;      // 密码
    std::string _database;      // 连接的数据库
    unsigned int _port;         // 连接的端口
    std::string _unix_socket;   // 连接的socket
    unsigned long _flags;       // 连接的附加值

    int _connect_timeout_time;  // 连接超时的时间
    int _reconnect_time;        // 重新连接的时间

};