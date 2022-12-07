#include <Sql_Connector.h>

Sql_Connector::Sql_Connector() {
    _mysql = std::make_shared<MYSQL>();
    mysql_init(_mysql.get());
    _is_error = false;

    _connect_timeout_time = 5;

    _reconnect_time = 5;
}

Sql_Connector::~Sql_Connector() {
    mysql_close(_mysql.get());
}

Sql_Connector::Sql_Connector(const std::string & host,
                             const std::string & user,
                             const std::string & password,
                             const std::string & database,
                             unsigned int port,
                             const std::string & unix_socket,
                             unsigned long flags) {
    _host = host;
    _user = user;
    _password = password;
    _database = database;
    _port = port;
    _unix_socket = unix_socket;
    _flags = flags;

    _mysql = std::make_shared<MYSQL>();
    mysql_init(_mysql.get());
    _is_error = false;

    _connect_timeout_time = 5;

    _reconnect_time = 5;
}

void Sql_Connector::startInit() {
}

bool Sql_Connector::connect() {
    // const char *host,
    // const char *user,
    // const char *passwd,
    // const char *db,
    // unsigned int port,
    // const char *unix_socket,
    // unsigned long flags);
    // 设置连接失败的时间
    mysql_options(_mysql.get(), MYSQL_OPT_CONNECT_TIMEOUT, &_connect_timeout_time);
    // 设置重新连接的时间
    mysql_options(_mysql.get(), MYSQL_OPT_RECONNECT, &_reconnect_time);

    if (!mysql_real_connect(_mysql.get(),
                                 _host.c_str(),
                                 _user.c_str(),
                                 _password.c_str(),
                                 _database.c_str(),
                                 _port,
                                 _unix_socket == "0" ? 0 : _unix_socket.c_str(),
                                 _flags)) {
        // 连接失败
        error = mysql_error(_mysql.get());
        _is_error = true;
        return false;
    }
    _is_error = false;
    return true;
}

std::string Sql_Connector::get_error() {
    if (_is_error) {
        return error;
    }
    return "success";
}

bool Sql_Connector::is_error() {
    return _is_error;
}

bool Sql_Connector::test_connect() {
    int ret = mysql_ping(_mysql.get());
    // 成功
    if (ret == 0) {
        _is_error = true;
        return true;
    } else {
        // 失败
        _is_error = false;
        error = mysql_error(_mysql.get());
        return false;
    }
}

std::shared_ptr<MYSQL> Sql_Connector::get_sql_ptr() {
    return _mysql;
}

void Sql_Connector::set_host(const std::string & host) {
    _host = host;
}

void Sql_Connector::set_user(const std::string & user) {
    _user = user;
}

void Sql_Connector::set_password(const std::string & password) {
    _password = password;
}

void Sql_Connector::set_database(const std::string & database) {
    _database = database;
}

void Sql_Connector::set_port(unsigned int port) {
    _port = port;
}

void Sql_Connector::set_unix_socket(const std::string & unix_socket) {
    _unix_socket = unix_socket;
}

void Sql_Connector::set_flags(unsigned long flags) {
    _flags = flags;
}

void Sql_Connector::set_connect_timeout_time(int time) {
    _connect_timeout_time = time;
}

void Sql_Connector::set_reconnect_time(int time) {
    _reconnect_time = time;
}

std::string Sql_Connector::get_host() {
    return _host;
}

std::string Sql_Connector::get_user() {
    return _user;
}

std::string Sql_Connector::get_password() {
    return _password;
}

std::string Sql_Connector::get_database() {
    return _database;
}

unsigned int Sql_Connector::get_port() {
    return _port;
}

std::string Sql_Connector::get_unix_socket() {
    return _unix_socket;
}

unsigned long Sql_Connector::get_flags() {
    return _flags;
}

int Sql_Connector::get_connect_timeout_time() {
    return _connect_timeout_time;
}

int Sql_Connector::get_reconnect_time() {
    return _reconnect_time;
}