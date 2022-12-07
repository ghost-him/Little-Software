# C++ 操作 mysql数据库

## 初始化和清理

### mysql_server_init()

```
作用：在调用其他函数之前调用，以初始化库。
int mysql_server_init(int argc __attribute__((unused)),
                       char **argv __attribute__((unused)),
                       char **groups __attribute__((unused)))
                                     
参数：无（所有的参数都已经弃用了，传入nullptr即可）

单线程模式，mysql_init会自动调用，线程不安全
```

### mysql_server_end()

```
作用：当使用完库时，例如与服务器断开连接后，调用。 对于一个客户程序，只通过执行以下的清理工作，内存管理任务
void mysql_server_end(void);
```

### mysql_init()

```
作用：准备并初始化一个MYSQL结构，以便与mysql_real_connect()一起使用。 如果一个MYSQL结构的地址作为参数被传递，该结构将被初始化，如果传递的是NULL，一个新的结构将被分配和初始化。
MYSQL *mysql_init(MYSQL *mysql);
参数：
	mysql: 将被初始化的空间
```

### mysql_end()

```
作用：关闭之前打开的连接，释放地址空间
void mysql_close(MYSQL *mysql);
参数：
	mysql: 将被关闭的空间
```

## 连接数据库

### mysql_real_connect()

```
作用：建立一个连接数据库的连接, 若未连接，则会阻塞
MYSQL * mysql_real_connect(MYSQL *mysql,
                           const char *host,
                           const char *user,
                           const char *passwd,
                           const char *db,
                           unsigned int port,
                           const char *unix_socket,
                           unsigned long flags);
参数：
	mysql：一个mysql句柄，它之前被mysql_init()分配。
    host：可以是一个主机名或一个IP地址。 如果向该参数传递NULL值或字符串 "localhost"，则假定本地主机会被假定。 如果可能的话，将使用管道代替TCP/IP协议。 从3.3.0版本开始，也可以提供一个逗号分隔的主机列表，以便在一个或多个主机不可用的情况下进行简单的故障转移。
    user： 用户名称。
    passwd： 如果提供或为空，服务器将尝试根据那些没有密码的用户记录来验证用户。这允许一个用户名使用不同的权限（取决于是否提供密码）。
    db： 如果提供，将指定执行查询时使用的默认数据库。
    port： 指定尝试连接到服务器的端口号。
    unix_socket： 指定应该使用的套接字或命名管道。（若为0，则会自己创建）
    flags： 允许设置各种连接选项的flags
返回值：
	成功：连接柄
	失败：0
```

### mysql_ping()

```
作用：检查与服务器的连接是否在工作。 如果它已经中断，并且全局选项reconnect被启用，那么将尝试自动重新连接。启用后，将尝试自动重新连接。这个功能可以被长期闲置的客户使用，以检查服务器是否已经关闭了连接，并在必要时重新连接。检查服务器是否关闭了连接，并在必要时重新连接。

int mysql_ping(MYSQL * mysql);
参数：
	mysql:需要检查的数据库
返回值：
	成功：0
	失败: 1
```

## 数据库设置

### mysql_options()

```
作用：用于设置额外的连接选项并影响连接的行为。 这个函数可以被多次调用以设置多个选项。
int mysql_options(MYSQL * mysql,
                  enum mysql_option,
                  const void * arg);
参数：
	mysql:设置的数据库
	mysql_option: 设置的属性
		MYSQL_OPT_CONNECT_TIMEOUT: 连接超时的时间
		MYSQL_OPT_RECONNECT：自动重连的时间
	arg: 设置的参数
返回值：
	成功：0
	失败：非0
```

## 错误分析

### mysql_error()

```
作用：返回最近一次函数调用的最后一条错误信息，可以成功也可以失败。 一个空的字符串意味着没有发生错误。

const char * mysql_error(MYSQL * mysql);
参数：
	mysql: 查询的数据库
```

## sql查询

### mysql_real_query()

````
作用：执行sql语句
int mysql_real_query(MYSQL * mysql,
                     const char * query,
                     unsigned long length);
参数：
	mysql:要查询的数据库
	query: 要执行的sql语句	
	length:sql语句的长度
返回值：
	成功：0
	失败：非0
````

### mysql_affected_rows()

```
作用：返回与mysql相关的最后一次操作所影响的行数，如果该操作是一个 "upsert"(INSERT, UPDATE, DELETE or REPLACE)语句，如果最后一次操作失败，则返回-1。
my_ulonglong mysql_affected_rows(MYSQL * mysql);
参数：
	mysql:被查询的mysql
```

### mysql_insert_id()

```
作用：返回给定的 connection 中上一步 INSERT 查询中产生的 AUTO_INCREMENT 的 ID 号。
my_ulonglong mysql_insert_id(st_mysql *mysql)
参数：
	mysql:被查询的mysql
```







## sql读取结果集

### mysql_store_result()

```
作用：从最后的一次查询中返回一个结果集，里面有查询的内容
MYSQL_RES * mysql_store_result(MYSQL * mysql);
参数：
	mysql: 待查询的数据库
返回值：
	成功：返回查询出来的结果集（有数据，要注意缓存大小）
	失败：nullptr
```

### mysql_use_result()

```
作用：用于启动检索数据库连接上使用mysql_real_query()函数执行的最后一次查询的结果集。 里面没有查询的内容，在遍历的时候才会有
MYSQL_RES * mysql_use_result(MYSQL * mysql);
参数：
	mysql:待查询的数据库
返回值：
	成功：返回查询出来的结果集（无数据）
	失败：nullptr
```

## 遍历结果集

### mysql_fetch_row()

```
作用： 从结果集中获取一行数据，并以char指针数组（MYSQL_ROW）的形式返回，其中每个列被存储在一个从0（零）开始的偏移量中。 对该函数的每次后续调用将返回结果集中的下一个行，如果没有更多的行，则返回NULL。

MYSQL_ROW mysql_fetch_row(MYSQL_RES * result);
参数：
	result:要被遍历的结果集
返回值
	成功：下一行的数据
	失败：nullptr（到达了末尾）
```

### mysql_fetch_lengths()

```
作用：返回一个数组，其中包含当前行的每一列的长度（不包括结束的零字符），如果发生错误，则返回NULL。

unsigned long * mysql_fetch_lengths(MYSQL_RES * result);
参数：
	result:要查询的结果集
返回值：
	成功：当前行的长度的数组
	失败：nullptr
	
注意：mysql_fetch_lengths()只对结果集的当前行有效。 如果你在调用mysql_fetch_row(3)之前或在检索了结果中的所有行之后调用它，它将返回NULL。
```

## 清空结果集

### mysql_free_result()

```
作用：释放结果集的空间
void mysql_free_result(MYSQL_RES * result);
参数：
	要被释放的结果集
返回值：
	无返回值
	
注意：之前由 mysql_fetch_row() 出来的返回值会变得无效
```

## 获取表字段

### mysql_fetch_field()

```
作用：返回一个结果集的一个列的定义，作为一个指向MYSQL_FIELD结构的指针。 重复调用这个函数 来检索结果集中所有列的信息。

MYSQL_FIELD * mysql_fetch_field(MYSQL_RES * result);
参数：
	result: 要查询的结果集
返回值：
	成功：一个指向MYSQL_FIELD结构体的指针
	失败：nullptr（没有更多的字段的时候）
```

### mysql_num_fields

```
作用：返回表字段的数量
unsigned int mysql_num_fields(MYSQL_RES * result);
参数：
	result:要查询的结果集
返回值：
	结果集中表字段的数量
```

### mysql_fetch_field_direct

```
作用：返回一个指向MYSQL_FIELD结构的指针，该结构包含指定结果集的字段信息。

MYSQL_FIELD * mysql_fetch_field_direct(MYSQL_RES * res,
                                       unsigned int fieldnr);
参数：
	res: 要查询的结果集
	fieldnr：要查询的字段的下标（从0开始）
返回值：
	成功：指向MYSQL_FIELD的结构体的指针
	失败：nullptr
```





# 示例代码

```cpp
/**
 * @author ghost_him
 */

#include <iostream>
#include <mysql/mysql.h>
#include <memory>
#include <string>
#include <cstring>

using namespace std;

int main() {
    // 定义一个mysql的指针，可以使用智能指针
    shared_ptr<MYSQL> mysql = make_shared<MYSQL>();
    // 初始化mysql
    mysql_init(mysql.get());

    // 定义连接的地址，用户，密码，数据库

    /*
     * 记得更改此条信息
     *
     */
    const char *host = "127.0.0.1";
    const char *user = "ghosthim";
    const char *pass = "123456";
    const char *db = "study";

    // 定义超时连接的时间
    int time = 3;
    // 设置连接超时的时间
    mysql_options(mysql.get(), MYSQL_OPT_CONNECT_TIMEOUT, &time);
    // 设置重新连接的时间
    mysql_options(mysql.get(), MYSQL_OPT_RECONNECT, &time);

    // 连接数据库
    if (!mysql_real_connect(mysql.get(), host, user, pass,db, 3306, 0, 0)) {
        cout << "连接失败"<< endl;
    } else {
        cout << "连接成功" << endl;
    }

    // 执行sql语句
    /*
     * 前提：数据库中有user这个表
     */
    const char * sql = "select * from user";
    // 查询sql语句
    int ret = mysql_real_query(mysql.get(), sql, strlen(sql));
    // 判断查询的返回值
    if (ret != 0) {
        cout << "mysql_real_query failed !" << mysql_error(mysql.get()) << endl;
    } else {
        cout << "执行成功" << endl;
    }

    // 获取结果集
    MYSQL_RES* result = mysql_store_result(mysql.get());

    if (!result) {
        cout << "mysql_use_result failed!" << mysql_error(mysql.get()) << endl;
    } else {
        cout << "mysql_use_result success" << endl;
    }

    // 获取表字段
    MYSQL_FIELD * field = 0;
    while (field = mysql_fetch_field(result)) {
        cout << "key:" << field->name << endl;
    }

    // 获取表字段数量
    int fnum = mysql_num_fields(result);

    // 遍历结果集
    MYSQL_ROW row;

    while (row = mysql_fetch_row(result)) {
        // cout << "[" << row[0] << "," << row[1] << "]" << endl;
        // cout << "length: " <<mysql_fetch_lengths(result)[0] <<","  << mysql_fetch_lengths(result)[1] << endl;
        for (int i = 0; i < fnum; i++) {
            cout << mysql_fetch_field_direct(result,i)->name << ":" ;
            if (row[i])
                cout << row[i];
            else
                cout << "NULL";
            cout << ",";
        }
        cout << endl;
    }
    // 清理结果集
    mysql_free_result(result);

    //=============================================================
    /*
     * 前提：数据库中有test_table这个表
     */

    string sql2 = "";
    // 1. 创建表
    sql2 = R"(create table  if not exists test_table(
            `id` int AUTO_INCREMENT,
            `name` varchar(1024),
            `path` varchar(2048),
            primary key(`id`))
            )";

    int re = mysql_query(mysql.get(), sql2.c_str());
    if (re != 0) {
        cout << "mysql create table failed!" << mysql_error(mysql.get()) << endl;
    }
    // 2. 插入100条数据

    for (int i = 0; i < 100; i++) {
        sql2 = R"(insert `test_table` (`name`,`path`) values('test_jpg', '/usr/ghosthim'))";
        re = mysql_query(mysql.get(), sql2.c_str());
        if (re == 0) {
            auto count = mysql_affected_rows(mysql.get());
            cout << "mysql_affected_rows" << count << endl;
            cout << mysql_insert_id(mysql.get()) << endl;
        } else {
            cout << "mysql_query failed" << endl;
            cout << mysql_error(mysql.get()) << endl;
        }

    }


    // 3. 修改数据
    sql2 = R"(update test_table set `name` = 'change' where id = 5)";
    re = mysql_query(mysql.get(), sql2.c_str());
    if (re == 0) {
        cout << "update success" << endl;
    } else {
        cout << "update failed!" << mysql_error(mysql.get()) << endl;
    }

    // 4. 删除数据
    /*
     * delete 以后不会真的删除空间，只做标识，需要用optimize来优化
     * delete 时，自增id号不会变成初始的样子，
     * 若需要使得id从1开始，则需要使用truncate
     */

    sql2 = R"(delete from test_table)";
    re = mysql_query(mysql.get(), sql2.c_str());
    if (re == 0) {
        cout << "delete success" << endl;
    } else {
        cout << "delete failed!" << mysql_error(mysql.get()) << endl;
    }

    sql2 = R"(optimize table test_table)";
    re = mysql_query(mysql.get(), sql2.c_str());
    if (re == 0) {
        cout << "optimize success" << endl;
    } else {
        cout << "optimize failed!" << mysql_error(mysql.get()) << endl;
    }

    // 关闭数据库
    mysql_close(mysql.get());
}
```



