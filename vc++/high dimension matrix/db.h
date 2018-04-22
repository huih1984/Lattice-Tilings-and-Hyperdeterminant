
#ifndef _DB_H_
#define _DB_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <vector>
using namespace std;




// sql语句的最大长度, 2M
#define MAX_SQL_LEN (2 * MB)

// 转换为单个字段查询的sql语句的最大长度
#define MAX_CHANGE_SELECT_SQL_LEN 2047

// 单行最大字节数, 这里不包括二进制数据.
#define RECORD_MAX_LEN 1023

// 数据库重连次数
#define DB_RE_CONNECT_COUNT 3

// where条件字段的最大长度
#define MAX_WHERE_LEN 1024




// 数据库类型
enum EnumDbType
{
	DB_ORACLE = 1, 	    // oracle 数据库
	DB_SYSBASE = 2,	    // sysbase 数据库
	DB_DB2 = 3,			// db2 数据库
	DB_SQLSERVER = 4,   // sqlserver 数据库
	DB_MYSQL = 5        // mysql 数据库
};


// sql的DML操作
enum EnumDml
{
	DML_UNKNOWN = 1,	// 未定义的操作, 例如Call, 在当前环境下不能运行的命令
	DML_INSERT = 2, 	// insert 操作
	DML_UPDATE = 3, 	// update 操作
	DML_DELETE = 4, 	// delete 操作
	DML_SELECT = 5  	// select 操作
};


// 使用select语句取出的单条记录值
typedef struct
{
	char m_value[RECORD_MAX_LEN + 1];
}ONE_RECORD_STRUCT;


// 数据库抽象类
class CDataBase
{
public:
	CDataBase();
	virtual ~CDataBase();
	
	// 打开数据库连接
	virtual bool db_open(const char* username, const char* password, const char* server_name, const char* dbname) = 0;
	
	// 打开数据库连接
	virtual bool db_open() = 0;
	
	// 关闭数据库连接
	virtual void db_close(bool commit = true) = 0;
	
	// 开始事务
	virtual int db_begintran() = 0;
	
	// 提交当前事务
	virtual void db_commit() = 0;
	
	// 回滚事务
	virtual void db_rollback() = 0;
	
	// 重连数据库
	virtual int db_re_connect() = 0;

	// 判断数据库是否连接正常
	virtual int db_test_conn() = 0;
	
	// 执行无返回结果集的SQL语句(insert, delete, update)
	virtual int execute_sql(const char* sql) = 0;
	
	// 执行返回一条记录的select语句
	virtual int execute_select_sql(const char* sql, ONE_RECORD_STRUCT &one_record) = 0;
	
	// 执行返回一条或多条记录的select语句
	virtual int execute_select_sql(const char* sql, vector<ONE_RECORD_STRUCT> &data_vec) = 0;

	// 执行存储过程
	virtual void exec_store_proc(const char* sp_name, const map<char*, char*> &input_param_map) = 0;
	  
	// 数据库连接的用户名
    void get_username(char* username);
    void set_username(const char* username);
    
    // 密码
    void set_password(const char* password);
    void get_password(char* password);
  
    // 数据库服务名称
    void get_server_name(char* server_name);    
    void set_server_name(const char* server_name);
  
    // 数据库名称
    void get_dbname(char* dbname);    
    void set_dbname(const char* dbname);
  
    // 返回SQLCODE 
    int get_sqlcode();
  
    // SQL语句执行结果
    void set_sql_result(int sqlcode, int row = 0);
  
    // 数据库连接状态
	bool IsConnected()
	{
		return m_connected;
	}
   
    // 数据库类型
	EnumDbType get_dbtype()
	{
		return m_dbtype;
	}

    
protected:
	// 将select语句转换为单个输出结果的sql语句
	int get_sql(const char* oldsql, char* newsql);

	// 获取sql操作类型
	EnumDml get_sql_dml(const char* sql);

	// 检查当前的语句是否已经到了列名结尾(" from "或" as ")
	virtual bool is_end_field(const char* sql);

	// 数据库连接信息
	char m_username[32]; 		// 用户名
	char m_password[32]; 		// 密码
	char m_server_name[32]; 	// 数据库服务名
	char m_dbname[32]; 			// 数据库名
	EnumDbType m_dbtype;        // 数据库类型
		
	// SQL语句执行信息
	int m_sqlcode; 			        // SQLCODE 
	int m_row;						// SQL 语句影响的行数
	char m_sqlerr[1024];	 // 执行的SQL语句错误信息

	bool m_connected; 	     // 数据库连接状态: 数据库是否连接  
 
    char m_split_symbol[10];     // 字段分隔符, 可以使用能区分字段名和字段值的字符
	char m_union_symbol[10];     // 把多个字段进行连接的连接符

	// 存放sql语句的缓冲区, 2M
	char* m_sql_buff;

	// 存放字段值的缓冲区, 1M
	char* m_field_value;
};



#endif

