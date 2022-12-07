#include "Sql_Controller.h"

Sql_Controller::Sql_Controller() {
    _connector = std::make_shared<Sql_Connector>();
    _mysql = _connector->get_sql_ptr();
}

Sql_Controller::Sql_Controller(const std::string & host,
                               const std::string & user,
                               const std::string & password,
                               const std::string & database,
                               unsigned int port,
                               const std::string & unix_socket,
                               unsigned long flags) {
    _connector = std::make_shared<Sql_Connector>(
            host,
            user,
            password,
            database,
            port,
            unix_socket,
            flags
            );
    _mysql = _connector->get_sql_ptr();
    _result = nullptr;
    _row = nullptr;
}

std::shared_ptr<Sql_Connector> Sql_Controller::get_connector() {
    return _connector;
}

bool Sql_Controller::execute_sql(const std::string & sql) {
    return execute_sql(sql.c_str(),sql.size());
}

bool Sql_Controller::execute_sql(const char * sql, unsigned int length) {
    _sql = sql;
    _sql_length = length;

    // 清理上一次的结果集
    if (_result != nullptr)
        mysql_free_result(_result);

    int ret = mysql_real_query(_mysql.get(), sql, length);
    if (ret != 0) {
        // 失败
        _is_error = true;
        _error = mysql_error(_mysql.get());

        return false;
    } else {

        // 成功
        _is_error = false;

        // 获取这一次的结果集
        _result = mysql_use_result(_mysql.get());

        if (_result == nullptr) {
            _is_error = true;
            _error = mysql_error(_mysql.get());
            return false;
        } else {
            _is_error = false;
        }

        return true;
    }
}

std::vector<std::pair<std::string, unsigned long>> Sql_Controller::get_row() {
    std::vector<std::pair<std::string, unsigned long>> res;
    if (_result == nullptr) {
        _is_error = true;
        return res;
    } else {
        _is_error = false;
    }


    _row = mysql_fetch_row(_result);
    if (_row == nullptr) {
        _is_error = true;
        _error = mysql_error(_mysql.get());
        return res;
    } else {
        _is_error = false;
    }

    // 获取当前的长度
    auto size = get_field_size();
    for (int i = 0; i < size; i++) {
        if (_row[i])
            res.push_back(std::make_pair(_row[i], mysql_fetch_lengths(_result)[i]));
        else
            res.push_back(std::make_pair("NULL", 0));
    }
    return res;
}

bool Sql_Controller::is_end() {
    if (_row == nullptr | _is_error)
        return true;
    else
        return false;
}

unsigned int Sql_Controller::get_affected_rows_size() {
    if (_mysql.get() == nullptr) {
        return -1;
    }

    return mysql_affected_rows(_mysql.get());
}

unsigned int Sql_Controller::get_insert_id() {
    return mysql_insert_id(_mysql.get());
}


std::string Sql_Controller::get_sql() {
    return _sql;
}

std::vector<std::string> Sql_Controller::get_field() {
    std::vector<std::string> res;
    if (_result == nullptr)
        return res;

    MYSQL_FIELD* field;
    while ((field = mysql_fetch_field(_result), field)) {
        res.push_back(field->name);
    }

    return res;
}

unsigned int Sql_Controller::get_field_size() {
    if (_result == nullptr) return -1;
    return mysql_num_fields(_result);
}

std::string Sql_Controller::get_error() {
    if (_is_error)
        return _error;
    else
        return "success";
}

bool Sql_Controller::is_error() {
    return _is_error;
}