#ifndef _SQLSERVER_DB_H_
#define _SQLSERVER_DB_H_


#define WHERE "where "



// sqlserver数据库操作类
class CDbSqlServer: public CDataBase
{
public:
	CDbSqlServer();
	CDbSqlServer(const char* username, const char* password, const char* server_name, const char* dbname);
	virtual ~CDbSqlServer();
	
	// 打开数据库连接
	virtual bool db_open(const char* username, const char* password, const char* server_name, const char* dbname);
	
	// 打开数据库连接
	virtual bool db_open();
	
	// 关闭数据库连接
	virtual void db_close(bool commit = true);
	
	// 开始事务
	virtual int db_begintran();
	
	// 提交当前事务
	virtual void db_commit();
	
	// 回滚事务
	virtual void db_rollback();
	
    // 判断数据库是否连接正常
	virtual int db_test_conn();

	// 重连数据库
    virtual int db_re_connect();
	
	// 执行无返回结果集的SQL语句(insert, delete, update)
	virtual int execute_sql(const char* sql);
	
	// 执行返回一条记录的select语句
	virtual int execute_select_sql(const char* sql, ONE_RECORD_STRUCT &one_record);
	
	// 执行返回一条或多条记录的select语句
	virtual int execute_select_sql(const char* sql, vector<ONE_RECORD_STRUCT> &data_vec);

	// 执行存储过程
	void exec_store_proc(const char* sp_name, const map<char*, char*> &input_param_map);

	TADOConnection* get_ado_conn()
	{
        return m_conn;
	}

	TADOQuery* get_ado_qry()
	{
		return m_ado_qry;
	}

	void set_msg(const MSG_HEAD &msg_head, const MSG_BODY &msg_body)
	{
        m_req_msg_head = msg_head;
		m_req_msg_body = msg_body;
	}

protected:
	// 检查当前的语句是否已经到了" as "(sqlserver多个字段查询使用as name格式)
	virtual bool is_end_field(const char* sql);

	// 解析select查询语句
    int parse_select(int type, char* where, int len);

    // 构造where条件, 如果有and_or就放在field_list第一个字段中.
	int parse_where(char* where, int len);

    // 根据表名和字段名/字段值, 构造insert sql语句.
	int parse_insert_sql(int type, const char* table_name, char* sql, int len);

	// 根据表名和字段名/字段值, 构造update sql语句
	int parse_update_sql(int type, const char* table_name, char* sql, int len);

	// 根据表名和字段名/字段值, 构造delete sql语句
	int parse_delete_sql(int type, const char* table_name, char* sql, int len);

    // 根据表名和字段名/字段值, 构造insert, delete或update sql语句.
	int parse_add_del_update_sql(int type, const char* table_name, char* sql, int len);

    // 更新表信息(包括增加/修改/删除)
	int update_table_info(const char* table_name, MSG_HEAD &head, MSG_BODY &res_body, CMem &xml_mem);

    // 给where查询条件的字段指定具体表名
	void change_where_option( char* where, char* new_where, int new_where_len, const map<char*, char*> &field_map);

	// 根据数组的值, 依次给报文体field list赋值
	int assign_field(FIELD_STRUCT field_arr[], int size, MSG_BODY &res_body);

	// 根据数组的内容, 依次给报文体value赋值
	int get_assign_value(FIELD_STRUCT field_arr[], int size, TADOQuery* ado_qry, MSG_BODY &res_body, CMem &xml_mem);
	
    //根据数据的类型组装SQL 报文
    int assign_accept_value(int  field_type, char* field_value,unsigned int field_len, char* field_memory);
       
	// 对TBytes类型的二进制数据进行base64编码
	int base64_code(TBytes bin_data, int bin_len, char *base64_str, int base64_len);

	// 对TStream类型的二进制数据进行base64编码
	int base64_code(TStream *stream, long stream_size, char* base64_str, int base64_len);




	TADOConnection* m_conn;  // 数据库连接
	TADOQuery* m_ado_qry;
	TADOStoredProc *m_store_proc;   // 存储过程


	// 请求报文头
	MSG_HEAD m_req_msg_head;

    // 请求报文体
	MSG_BODY m_req_msg_body;

	
private:
    // 初始化
	void init();

		
};


#endif
