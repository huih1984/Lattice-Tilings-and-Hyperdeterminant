/****************************************************************
* Copyright(c) 2009-2012 Keybridge Communication Technology Co.,Ltd.
* All rights reserved.
*
* 文件名称：sqlserver_db.cpp
* 摘    要：sqlserver数据库处理
*
* 作    者:  杨雨衡 2011-02-28
****************************************************************/

#include "sqlserver_db.h"
#include <OleAuto.h>



CDbSqlServer::CDbSqlServer()
{
	init();
}


CDbSqlServer::CDbSqlServer(const char* username, const char* password, const char* server_name, const char* dbname)
{
	strcpy(m_username, username);
	strcpy(m_password, password);
	strcpy(m_server_name, server_name);
	strcpy(m_dbname, dbname);
	
	init();
}


void CDbSqlServer::init()
{
	m_dbtype = DB_SQLSERVER;
	strcpy(m_split_symbol, "'^#'");  // 为避免和字段值区分不开, 这里使用多个字符
	strcpy(m_union_symbol, "+");     // sqlserver连接符
	m_connected = false;
	m_conn = new TADOConnection(NULL);
	m_ado_qry = new TADOQuery(NULL);
	m_store_proc = new TADOStoredProc(NULL);
}

// 析构函数: 关闭数据库连接
CDbSqlServer::~CDbSqlServer()
{
    db_close();
}


// 打开数据库连接
// 自动重连这个参数暂时没用.
bool CDbSqlServer::db_open()
{
	// 数据库已连接
	if (m_connected)
	{
		return true;
	}
	
	try
	{  
        m_conn->ConnectionString = AnsiString(g_cfg.m_sql_server_conn_str);
		m_conn->Connected = true;
		m_ado_qry->Connection = m_conn;    // 指定数据库连接
		m_store_proc->Connection = m_conn;
	}
	catch (...)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "连接数据库[user:%s, password:%s, srvname:%s, dbname:%s]失败!", m_username, m_password, m_server_name, m_dbname);
		return false;
	}

	m_connected = true;
	return true;	
}


// 打开数据库连接
bool CDbSqlServer::db_open(const char* username, const char* password, const char* server_name, const char * dbname)
{
	strcpy(m_username, username);
	strcpy(m_password, password);
	strcpy(m_server_name, server_name);
	strcpy(m_dbname, dbname);
	return db_open();
}

// 关闭数据库连接
// bool commit: 是否自动提交事务
void CDbSqlServer::db_close(bool commit)
{
	// 检查数据库连接是否断开
	if (!m_connected)
	{
		return;
	}

	// 断开连接, 释放资源
	if (m_conn)
	{
		delete m_conn;
		m_conn = NULL;
	}
	
	if (m_ado_qry)
	{
		delete m_ado_qry;
		m_ado_qry = NULL;
	}

	if (m_store_proc)
	{
		delete m_store_proc;
		m_store_proc = NULL;
	}
	
	m_connected = false;
}

// 开始事务
int CDbSqlServer::db_begintran()
{
	return m_conn->BeginTrans();
}

// 提交当前事务
void CDbSqlServer::db_commit()
{
	m_conn->CommitTrans();
}

// 回滚事务
void CDbSqlServer::db_rollback()
{
	m_conn->RollbackTrans();
}

// 判断数据库是否连接正常
int CDbSqlServer::db_test_conn()
{
	ONE_RECORD_STRUCT one_record;
	int ret;
	try
	{
		ret = execute_select_sql("select getdate()", one_record);
	}
	catch (Oleauto::EOleException &dbe)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "数据库异常:%s%s, error_code:%d", dbe.ClassName().t_str(), dbe.Message.t_str(), dbe.ErrorCode);
		if (dbe.ErrorCode == SQLSERVER_DISCONNECT_ERRCODE)
		{
        	return db_re_connect();  // 重连DB
		}
	}
    catch (Exception& e)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "异常:%s%s", e.ClassName().t_str(), e.Message.t_str());
		return DB_ACTION_EXECUTE_FAILED;
	}

    return ret;
}

// 重连数据库
int CDbSqlServer::db_re_connect()
{
	// DB连接断开, 重连, 尝试连接3次, 3次都不成功就返回连接失败
	int i;
	for (i = 0; i < DB_RE_CONNECT_COUNT; i++)
	{
		try
		{
			m_conn->Connected = false;
			m_conn->ConnectionString = AnsiString(g_cfg.m_sql_server_conn_str);
			m_conn->Connected = true;
		}
		catch(Exception& e)
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "%s%s, 连接数据库[user:%s, password:%s, srvname:%s, dbname:%s]失败!", e.ClassName().t_str(), e.Message.t_str(), m_username, m_password, m_server_name, m_dbname);
			continue;
		}

		if (m_conn->Connected)
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_INFO, "连接数据库[user:%s, password:%s, srvname:%s, dbname:%s]成功.", m_username, m_password, m_server_name, m_dbname);
			return SUCCESS;
        }
	}

	if (i == DB_RE_CONNECT_COUNT)
	{
        g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "重连数据库[user:%s, password:%s, srvname:%s, dbname:%s]3次失败!", m_username, m_password, m_server_name, m_dbname);
		return DB_CONNECT_FAILED;
	}

	return SUCCESS;
}

// 执行无返回结果集的SQL语句(insert, delete, update)
// 异常在调用函数中处理
int CDbSqlServer::execute_sql(const char* sql)
{
	g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_DEBUG, "sql:[%s]", sql);
	m_ado_qry->SQL->Clear();   // 清空sql
	m_ado_qry->SQL->Add(sql);
	m_ado_qry->ParamCheck = true;
	int ret_code = m_ado_qry->ExecSQL();
	g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_DEBUG, "ret_code: %d.", ret_code);
	return 0;
}

// 执行返回一条记录的select语句
int CDbSqlServer::execute_select_sql(const char* sql, ONE_RECORD_STRUCT &one_record)
{
	vector<ONE_RECORD_STRUCT> data_vec;
	execute_select_sql(sql, data_vec);

	// 查不到记录
	if (data_vec.size() == 0)
	{
		return DB_NOT_FOUND_DATA;
	}

	// 查到多条记录
	if (data_vec.size() > 1)
	{
		return DB_MORE_DATAS;
	}

	memset(one_record.m_value, 0, sizeof(one_record.m_value));
	strcpy(one_record.m_value, data_vec[0].m_value);
	return 0;
}


/* 执行返回一条或多条记录的select语句.
 * sqlserver中要把所有字段转换为字符型. ----oralce好像能自动转换,如整型和字符型?
 * select语句格式: select name, convert(varchar(10), id), ... as dbname from table_name ...
 *	 select之后的字段都转换为字符型, sqlserver查询多个字段一定要加上as name, 方便ado取值.
 */
int CDbSqlServer::execute_select_sql(const char* sql, vector<ONE_RECORD_STRUCT> &data_vec)
{
	char newsql[MAX_CHANGE_SELECT_SQL_LEN] = {0};
    int ret;
	ret = get_sql(sql, newsql); // 转换sql语句
	if (0 != ret)
	{
		return ret;
	}

    ONE_RECORD_STRUCT one_record;
	m_ado_qry->SQL->Clear();   // 清空sql
	m_ado_qry->SQL->Add(newsql);
	m_ado_qry->ParamCheck = true;
	m_ado_qry->Open();
	m_ado_qry->First();
	while (!m_ado_qry->Eof)
	{
        // 取索引号为1的记录
		snprintf(one_record.m_value, sizeof(one_record.m_value), "%s", m_ado_qry->Fields->FieldByNumber(1)->AsAnsiString.Trim().c_str());
		data_vec.push_back(one_record);

		// 取下一条记录
		m_ado_qry->Next();
	}
	m_ado_qry->Close();
	return 0;
}

// 检查当前的语句是否已经到了" as "(sqlserver查询多个字段要使用"as name"格式)
bool CDbSqlServer::is_end_field(const char* sql)
{
	if (sql == NULL || strlen(sql) == 0)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "sql is null.");
		return false;
	}

	char tmp[5];    // " as "
	memset(tmp, 0x0, sizeof(tmp));
	strncpy(tmp, sql, sizeof(tmp) - 1);
	if ((tmp[0] == ' ' || tmp[0] == '\t') && (strncmpi(tmp + 1, "as", 2) == 0) && (tmp[3] == ' ' || tmp[3] == '\t'))
	{
		return true;
	}

	return false;
}


// 解析select查询语句, 带查询条件或者不带查询条件. 查询sql语句需要在调用函数中构造.
// select: field_list和data中是where条件.
int CDbSqlServer::parse_select(int type, char* where, int len)
{
	memset(where, 0, len);
	if (type == DBES_SELECT_NOWHERE_OPTION)  // 不带查询条件
	{
        if (m_req_msg_body.value_list.size != 0)
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "无条件查询时, body中不能带查询条件!");
			return NO_WHERE_OPTION_ERROR;
		}

		return SUCCESS;
	}
	else if (type == DBES_SELECT_WHERE_OPTION)  // 需要有查询条件
	{
		if (m_req_msg_body.value_list.size == 0)
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "有条件查询时, body中没有查询条件!");
			return MSG_BODY_EMPTY;
		}

		return parse_where(where, len);
	}
	else
	{
        g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "不是查询操作!");
		return XML_HEAD_TYPE_ERROR;
    }
}


// 构造where条件, 如果有and_or就放在field_list第一个字段中.
// 把body中的字段名和值按照and/or组合在一起, 构造where子句.
int CDbSqlServer::parse_where(char* where, int len)
{
	memset(where, 0, len);
    if (m_req_msg_body.value_list.size == 0)
	{
		return SUCCESS;
	}

	PARAM_FIELD_STRUCT *ptr = m_req_msg_body.value_list.head;

	// and_or要放在field_list的第一个字段中，如果没有默认为and
	char and_or[6] = "and";
	int i = 0;
	if (stricmp(m_req_msg_body.field_list[0].m_field_name, "and_or") == 0)
	{
		memset(and_or, 0, sizeof(and_or));
		strncpy(and_or, ptr->m_value, ptr->m_len);
		i++;
		ptr = ptr->next;
	}

	int off_set = 0;
	snprintf(where + off_set, len - off_set, WHERE);
	off_set = strlen(where);
	for (; i < m_req_msg_body.value_list.size; i++)
	{
        // 字段名
		snprintf(where + off_set, len - off_set, "(%s ", m_req_msg_body.field_list[i].m_field_name);
		off_set = strlen(where);

        // 字段值
		memset(m_field_value, 0, MAX_FIELD_LEN);
		strncpy(m_field_value, ptr->m_value, ptr->m_len);  // 拷贝指定长度的字符串
		snprintf(where + off_set, len - off_set, "%s)", m_field_value);
		off_set = strlen(where);
		if (off_set >= len)
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "数组越界.");
			return ARRAY_OVERFLOW;
		}

		if (i != (m_req_msg_body.value_list.size - 1))   // 不是最后一个条件
		{
			snprintf(where + off_set, len - off_set, " %s ", and_or);   // and/or
			off_set = strlen(where);
			if (off_set >= len)
			{
				g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "数组越界.");
				return ARRAY_OVERFLOW;
			}
		}

		ptr = ptr->next;
	}

	return SUCCESS;
}


/**
 * @brief  把二进制和其他类型的数据保存到field_memory中，其中二进制的数据要经过
 *           base64的解密
 * @param [in] field_type 字段类型
 * @param [in] field_value 字段value
 * @param [in] field_len 字段长度
 * @param [out] field_memory 保存转换后字符
 * 返回值: 转换后二进制字符串的长度
 * @author：hongliming 2011-07-29
 */
int CDbSqlServer::assign_accept_value(int  field_type, char* field_value,unsigned int field_len, char* field_memory)
{
    int ret = 0;
    int dstlen = MAX_FIELD_LEN;
    if ((NULL == field_value) || (NULL == field_memory))
    {
        return PARA_ERROR;
	}

	// 根据数组的内容, 依次给报文体value赋值
	if((FIXED_BINARY_DATA_TYPE_ID == field_type) || (VAR_BINARY_DATA_TYPE_ID == field_type))
	{
		char * buf = new char[MAX_FIELD_LEN];
		ASSERT2(buf != NULL, LOG_ERR);
		int hex_len = MAX_FIELD_LEN;
		if(!base64_decode(field_value, field_len, field_memory, dstlen))
		{
			delete []buf;
			buf = NULL;
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "base64解密失败.");
			return BASE64_ENCODE_ERROR;
		}

		char2hex(field_memory, dstlen, buf, hex_len);
		snprintf(field_memory, MAX_FIELD_LEN, "0x%s", buf);
		delete []buf;
		buf = NULL;
	}
	else
	{
		strncpy(field_memory,field_value,field_len);
	}

	return SUCCESS;
}


// 根据表名和字段名/字段值, 构造insert sql语句. 字段名和值是对应的.
// insert: field_list和data中对应的都是字段名和值;
// eg: insert into dbo.task_result_table(task_id, device_id, result) values(123, 260, 'abc2345')
int CDbSqlServer::parse_insert_sql(int type, const char* table_name, char* sql, int len)
{
	int i, off_set = 0;
    int field_type = 0;

	if (type != DBES_INSERT_OPTION)
	{
        g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "不是增加操作!");
		return XML_HEAD_TYPE_ERROR;
	}

    if (m_req_msg_body.value_list.size == 0)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "body中没有字段值!");
		return MSG_BODY_EMPTY;
	}

	PARAM_FIELD_STRUCT *ptr = m_req_msg_body.value_list.head;
	memset(sql, 0, len);
	snprintf(sql, len, "insert into %s(", table_name);
	off_set = strlen(sql);
    // 构造字段名, 字段名一般不大, 这里没有判断数组越界
	for (i = 0; i < m_req_msg_body.value_list.size; i++)
	{
		snprintf(sql + off_set, len - off_set, "%s", m_req_msg_body.field_list[i].m_field_name);
		off_set = strlen(sql);

		if (i != (m_req_msg_body.value_list.size - 1))
		{
			snprintf(sql + off_set, len - off_set, ", ");
			off_set = strlen(sql);
		}
	}

    snprintf(sql + off_set, len - off_set, ") values(");
	off_set = strlen(sql);
	// 构造值
    for (i = 0; i < m_req_msg_body.value_list.size; i++, ptr = ptr->next)
	{
        // 字段值
        memset(m_field_value, 0, MAX_FIELD_LEN);
        field_type = m_req_msg_body.field_list[i].m_type;        
        if(assign_accept_value(field_type, ptr->m_value, ptr->m_len,m_field_value) != SUCCESS)
        {
            return -1;
        }             
		snprintf(sql + off_set, len - off_set, "%s", m_field_value);
		off_set = strlen(sql);
		if (off_set >= len)
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "数组越界.");
			return ARRAY_OVERFLOW;
		}

		if (i != (m_req_msg_body.value_list.size - 1))
		{
			snprintf(sql + off_set, len - off_set, ", ");
			off_set = strlen(sql);
			if (off_set >= len)
			{
				g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "数组越界.");
				return ARRAY_OVERFLOW;
			}
		}
	}

    snprintf(sql + off_set, len - off_set, ")");
	off_set = strlen(sql);
	return SUCCESS;
}


// 根据表名和字段名/字段值, 构造update sql语句, set后面必须有值, where条件可有可没有.
// update: 需要区分set字段名和where字段名, 规则是在字段名后加[set], [where], 如dbname[set], dbname[where];
// eg: update dbo.alarm_details_current set op_status = '已确认', alarm_status = '已产生' where trap_id = 1234
int CDbSqlServer::parse_update_sql(int type, const char* table_name, char* sql, int len)
{
	int filed_len, off_set = 0;
//	bool is_have_and_or = false;//是否有and_or标记
	char set_where[8]; // 字段名后的5个或7个字符, 即[set]或[where]
    int field_type = 0;

	if (type != DBES_UPDATE_OPTION)    // where条件可有, 也可以没有
	{
        g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "不是update操作!");
		return XML_HEAD_TYPE_ERROR;
	}

    if (m_req_msg_body.value_list.size == 0)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "body中没有字段值!");
		return MSG_BODY_EMPTY;
	}

	// and_or要放在field_list的第一个字段中，如果没有默认为and
	char and_or[6] = "and";
	int i = 0;
	PARAM_FIELD_STRUCT *ptr = m_req_msg_body.value_list.head;
	if (stricmp(m_req_msg_body.field_list[0].m_field_name, "and_or") == 0)
	{
		strncpy(and_or, ptr->m_value, ptr->m_len);
		i++;
		ptr = ptr->next;
//		is_have_and_or = true;
	}

	memset(sql, 0, len);
	snprintf(sql, len, "update %s set ", table_name);
	off_set = strlen(sql);

    // 构造set设置的字段值
	for (; i < m_req_msg_body.value_list.size; i++, ptr = ptr->next)
	{
        // 判断是[set]还是[where]
		filed_len = strlen(m_req_msg_body.field_list[i].m_field_name);

		// 先取后5个字符
        if (filed_len <= 5)
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "字段名太短!");
			return -1;
		}
		memset(set_where, 0, sizeof(set_where));
		strncpy(set_where, &(m_req_msg_body.field_list[i].m_field_name[filed_len - 5]), sizeof(set_where) - 1);

//		if(is_have_and_or)
//		{
//			if (i==1 && stricmp(set_where, "here]") == 0)  // 第一个字段是where条件
//			{
//				g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "消息格式错误!");
//				return -1;
//			}
//		}
//		else
//		{
//			if (i==0 && stricmp(set_where, "here]") == 0)  // 第一个字段是where条件
//			{
//				g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "消息格式错误!");
//				return -1;
//			}
//		}

		// [set]
		if (stricmp(set_where, "[set]") == 0)
		{
			m_req_msg_body.field_list[i].m_field_name[filed_len - 5] = '\0';  // 去掉[set]
		}
		else
		{
			// 去掉最后两个字符", "
            sql[off_set - 2] = '\0';
            off_set -= 2;
			break;
        }

		// 字段名
		snprintf(sql + off_set, len - off_set, "%s ", m_req_msg_body.field_list[i].m_field_name);
		off_set = strlen(sql);

        // 字段值
		memset(m_field_value, 0, MAX_FIELD_LEN);
        field_type = m_req_msg_body.field_list[i].m_type;        
        if(assign_accept_value(field_type, ptr->m_value, ptr->m_len,m_field_value) != SUCCESS)
        {
            return -1;
          
        }
        /*二进制数据要加上=*/
        if((FIXED_BINARY_DATA_TYPE_ID == field_type) ||(VAR_BINARY_DATA_TYPE_ID == field_type))
        {
            snprintf(sql + off_set, len - off_set, " = %s, ", m_field_value);
        }
        else
        {
            snprintf(sql + off_set, len - off_set, "%s, ", m_field_value);
        }
        off_set = strlen(sql);
        if (off_set >= len)
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "数组越界.");
			return ARRAY_OVERFLOW;
		}
	}

	snprintf(sql + off_set, len - off_set, " where");
	off_set = strlen(sql);
	// 构造where条件
    for (; i < m_req_msg_body.value_list.size; i++, ptr = ptr->next)
	{
		// 判断是[set]还是[where]
		filed_len = strlen(m_req_msg_body.field_list[i].m_field_name);

		// 先取后7个字符
        if (filed_len <= 7)
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "字段名太短!");
			return -1;
		}
		memset(set_where, 0, sizeof(set_where));
		strncpy(set_where, &(m_req_msg_body.field_list[i].m_field_name[filed_len - 7]), sizeof(set_where) - 1);

		// [where]
		if (stricmp(set_where, "[where]") == 0)
		{
        	m_req_msg_body.field_list[i].m_field_name[filed_len - 7] = '\0';  // 去掉[where]
		}
		else
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "消息格式错误!");
			return -1;
        }

        // 字段名, 字段名和值用()括起来
		snprintf(sql + off_set, len - off_set, " (%s ", m_req_msg_body.field_list[i].m_field_name);
        off_set = strlen(sql);

        // 字段值
		memset(m_field_value, 0, MAX_FIELD_LEN);
		strncpy(m_field_value, ptr->m_value, ptr->m_len);  // 拷贝指定长度的字符串
		snprintf(sql + off_set, len - off_set, "%s) ", m_field_value);
		off_set = strlen(sql);
        if (off_set >= len)
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "数组越界.");
			return ARRAY_OVERFLOW;
		}

		if (i != (m_req_msg_body.value_list.size - 1))   // 不是最后一个条件
		{
			snprintf(sql + off_set, len - off_set, " %s ", and_or);  // and/or
			off_set = strlen(sql);
			if (off_set >= len)
			{
				g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "数组越界.");
				return ARRAY_OVERFLOW;
			}
		}
	}

	return SUCCESS;
}

// 根据表名和字段名/字段值, 构造delete sql语句, where可有可没有.
// delete: field_list和data中是where条件;
// eg: delete from alarm_details_history where trap_id in ("aaa")
int CDbSqlServer::parse_delete_sql(int type, const char* table_name, char* sql, int len)
{
	if (type != DBES_DELETE_OPTION)    // where条件可有, 也可以没有
	{
        g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "不是delete操作!");
		return XML_HEAD_TYPE_ERROR;
	}

	memset(sql, 0, len);
	snprintf(sql, len, "delete from %s ", table_name);
	int off_set = strlen(sql);

	char where[1024];
	int ret = parse_where(where, sizeof(where));
	if (ret == SUCCESS)
	{
		if (len > (off_set + strlen(where)))
		{
        	snprintf(sql + off_set, len - off_set, "%s", where);
			off_set = strlen(sql);
		}
		else
		{
        	g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "数组越界.");
			return ARRAY_OVERFLOW;
        }
	}

	return ret;
}


// 根据表名和字段名/字段值, 构造insert, delete或update sql语句.
int CDbSqlServer::parse_add_del_update_sql(int type, const char* table_name, char* sql, int len)
{
	if (type == DBES_INSERT_OPTION)
	{
    	return parse_insert_sql(type, table_name, sql, len);
	}
	else if (type == DBES_DELETE_OPTION)
	{
		return parse_delete_sql(type, table_name, sql, len);
	}
    else if (type == DBES_UPDATE_OPTION)
	{
    	return parse_update_sql(type, table_name, sql, len);
	}
	else
	{
        return DB_OPER_TYPE_ERR;
    }
}


// 更新表信息(包括增加/修改/删除)
// 注意: 只能update单个表
int CDbSqlServer::update_table_info(const char* table_name, MSG_HEAD &head, MSG_BODY &res_body, CMem &xml_mem)
{
	int ret = SUCCESS;

    try
	{
		ret = parse_add_del_update_sql(head.type, table_name, m_sql_buff, MAX_SQL_LEN);
        if (ret != SUCCESS)
		{
			return ret;
		}

		ret = execute_sql(m_sql_buff);
		if (ret != SUCCESS)
		{
			return ret;
		}

		res_body.data_num = 0;
    }
	catch (Oleauto::EOleException &dbe)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "数据库异常:%s%s, error_code:%d", dbe.ClassName().t_str(), dbe.Message.t_str(), dbe.ErrorCode);
		if (dbe.ErrorCode == SQLSERVER_DISCONNECT_ERRCODE)
		{
        	db_re_connect();  // 重连DB
		}
		ret = DB_ACTION_EXECUTE_FAILED;
	}
    catch (Exception& e)
	{
        g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "异常:%s%s", e.ClassName().t_str(), e.Message.t_str());
		ret = DB_ACTION_EXECUTE_FAILED;
	}

	return ret;
}


// 给where查询条件的字段指定具体表名
// map<char*, char*> field_map: key表示body中的field, value表示查询语句中具体的数据库表名.
void CDbSqlServer::change_where_option(char* where, char* new_where, int new_where_len, const map<char*, char*> &field_map)
{
	char* ptr;
	int ptr_len;
	int offset = 0;
	char temp[1024];
	int front_len;

	if (where == NULL || strlen(where) >= sizeof(temp))
	{
        g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "where is error!");
		return;
	}

	g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_DEBUG, "old_where: [%s]", where);
	memset(new_where, 0, new_where_len);
	strncpy(new_where, where, new_where_len - 1);
    map<char*, char*>::const_iterator it;
	for (it = field_map.begin(); it != field_map.end(); it++)
	{
		ptr = strstr(new_where, it->first); // 找到查询条件中的字段
		if (ptr != NULL)
		{
			memset(temp, 0, sizeof(temp));

			ptr_len = strlen(ptr);
			front_len = ptr - new_where;
			strncpy(temp, new_where, front_len);   // 拷贝字段名前面的字符串
			offset = strlen(temp);
			strcpy(temp + offset, it->second);     // 被替换的新字段名
			offset = strlen(temp);
			strcpy(temp + offset, ptr + strlen(it->first)); // 拷贝字段名后面的字符串

			// 得到新的where语句
			strcpy(new_where, temp);
		}
	}

    g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_DEBUG, "new_where: [%s]", new_where);
}


// 执行存储过程, key:存储过程的输入参数, value:传给存储过程的值.
void CDbSqlServer::exec_store_proc(const char* sp_name, const map<char*, char*> &input_param_map)
{
	m_store_proc->ProcedureName = sp_name;  // 存储过程名字
	m_store_proc->Parameters->Clear();
	m_store_proc->Parameters->Refresh();

	map<char*, char*>::const_iterator it;
	for (it = input_param_map.begin(); it != input_param_map.end(); it++)
	{
		m_store_proc->Parameters->ParamByName(it->first)->Value = it->second;
	}

	m_store_proc->ExecProc();
}


// 根据数组的值, 依次给报文体field list赋值
int CDbSqlServer::assign_field(FIELD_STRUCT field_arr[], int size, MSG_BODY &res_body)
{
	if (size > MAX_FIELD_LIST_SIZE)
	{
    	return ARRAY_OVERFLOW;
	}

	for (int i = 0; i < size; ++i)
	{
        memset(res_body.field_list[i].m_field_name, 0, sizeof(res_body.field_list[i].m_field_name));
		strncpy(res_body.field_list[i].m_field_name, field_arr[i].m_field_name, sizeof(res_body.field_list[i].m_field_name) - 1);
        res_body.field_list[i].m_type = field_arr[i].m_type;
	}
	return 0;
}


// 根据数组的内容, 依次给报文体value赋值
int CDbSqlServer::get_assign_value(FIELD_STRUCT field_arr[], int size, TADOQuery* ado_qry, MSG_BODY &res_body, CMem &xml_mem)
{
	int ret;

	// 字段名的数量不能超过最大值
	if (size > MAX_FIELD_LIST_SIZE)
	{
    	return ARRAY_OVERFLOW;
	}

    // 把字段值存到分配的内存中
	for (int i = 0; i < size; ++i)
	{
        // 整型
		if (field_arr[i].m_type == SHORT_INT_DATA_TYPE_ID || field_arr[i].m_type == UNSIGNED_SHORT_INT_DATA_TYPE_ID
			|| field_arr[i].m_type == INT_DATA_TYPE_ID || field_arr[i].m_type == UNSIGNED_INT_DATA_TYPE_ID
			|| field_arr[i].m_type == LONG_INT_DATA_TYPE_ID || field_arr[i].m_type == UNSIGNED_LONG_INT_DATA_TYPE_ID)
		{
			ret = CMsg::assign(res_body, ado_qry->FieldByName(field_arr[i].m_field_name)->AsInteger, xml_mem);
		}
        else if (field_arr[i].m_type == BOOL_DATA_TYPE_ID)  // bool
		{
            int value = ado_qry->FieldByName(field_arr[i].m_field_name)->AsBoolean ? 1 : 0;
			ret = CMsg::assign(res_body, value, xml_mem);
		}
		else if (field_arr[i].m_type == CHAR_DATA_TYPE_ID)  // 字符型
		{
			ret = CMsg::assign(res_body, ado_qry->FieldByName(field_arr[i].m_field_name)->AsAnsiString.Trim().c_str(), xml_mem);
		}
		else if (field_arr[i].m_type == FIXED_BINARY_DATA_TYPE_ID)  // 定长二进制数据, 使用TBytes
		{
			TBytes fixed_data = ado_qry->FieldByName(field_arr[i].m_field_name)->AsBytes;
            // base64编码, m_field_value存放base64编码后的字符串
			ret = base64_code(fixed_data, fixed_data.Length, m_field_value, MAX_FIELD_LEN);   // base64编码
			if (ret != 0)
			{
				return ret;
			}

			ret = CMsg::assign(res_body, m_field_value, xml_mem);
		}
		else if (field_arr[i].m_type == VAR_BINARY_DATA_TYPE_ID)  // 变长二进制数据, 使用TStream
		{
			TStream *stream = ado_qry->CreateBlobStream(ado_qry->FieldByName(field_arr[i].m_field_name), bmRead);
			// base64编码, m_field_value存放base64编码后的字符串
			ret = base64_code(stream, stream->Size, m_field_value, MAX_FIELD_LEN);
			if (ret != 0)
			{
				return ret;
			}

			ret = CMsg::assign(res_body, m_field_value, xml_mem);
		}
		else if (field_arr[i].m_type == FLOAT_DATA_TYPE_ID || field_arr[i].m_type == DOUBLE_DATA_TYPE_ID)
		{
			ret = CMsg::assign(res_body, ado_qry->FieldByName(field_arr[i].m_field_name)->AsFloat, xml_mem);
		}
		else
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "数据类型%d错误!", field_arr[i].m_type);
			return DATA_STRUCT_ID_ERROR;
		}

		// 检查结果是否正确
		if (ret != 0)
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "错误ret:%d!", ret);
			return ret;
		}
	}

	return ret;
}


/**
 * @brief 对TBytes类型的二进制数据进行base64编码
 * @param [in] bin_data  需要base64编码的TBytes类型的二进制数据, 一般是定长的二进制数据
 * @param [in] bin_len  bin_data的字节数
 * @param [out] base64_str 存放对二进制数据进行base64编码之后的字符串
 * @param [in] base64_len 存放base64编码之后的字符串的缓冲区大小
 * 返回值: 0表示解析成功, 其他表示出错.
 */
int CDbSqlServer::base64_code(TBytes bin_data, int bin_len, char *base64_str, int base64_len)
{
	bool flag;
	char bin_str[1024] = {0};

	if (bin_len > sizeof(bin_str))
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "数组空间不够!");
		return ARRAY_OVERFLOW;
	}

	// 二进制数据存在进行base64编码, 不存在就填为空字符串
	if (bin_len > 0)
	{
		for (int i = 0; i < bin_len; i++)
		{
			bin_str[i] = bin_data[i];
		}

		flag = base64_encode(bin_str, bin_len, base64_str, base64_len);
		if (!flag)
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "base64编码失败!");
			return BASE64_ENCODE_ERROR;
		}
	}
	else
	{
		strcpy(base64_str, "");
	}

	return SUCCESS;
}


/**
 * @brief 对TStream类型的二进制数据进行base64编码
 * @param [in] stream  需要base64编码的TStream类型的二进制数据
 * @param [in] stream_size stream的字节数
 * @param [out] base64_str 存放对二进制数据进行base64编码之后的字符串
 * @param [in] base64_len 存放base64编码之后的字符串的缓冲区大小
 * 返回值: 0表示解析成功, 其他表示出错.
 * 说明: 调用函数用法如下, 使用m_field_value成员变量:
     TStream *stream = m_ado_qry->CreateBlobStream(m_ado_qry->FieldByName("icon_pic"), bmRead);
     ret = base64_code(stream, stream->Size, m_field_value, MAX_FIELD_LEN);
 */
int CDbSqlServer::base64_code(TStream *stream, long stream_size, char* base64_str, int base64_len)
{
	// 不能超过字段最大长度
	if (stream_size >= MAX_FIELD_LEN)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "字段长度太大!");
		return FIELD_LENGTH_TOO_BIG;
	}

    // 不存在就填为空字符串
	if (stream_size == 0)
	{
		strcpy(base64_str, "");
		return SUCCESS;
	}

	bool flag;
	int ret = SUCCESS;
	char* bin_data = new char[stream_size];
	if (bin_data == NULL)
	{
    	g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "分配内存失败!");
		return ALLOC_MEMORY_FAIL;
	}

	stream->ReadBuffer(bin_data, stream_size);
	flag = base64_encode(bin_data, stream_size, base64_str, base64_len);
	if (!flag)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "base64编码失败!");
		ret = BASE64_ENCODE_ERROR;
	}

	delete []bin_data;
	return ret;
}


