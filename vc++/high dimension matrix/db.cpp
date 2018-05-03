/****************************************************************
* Copyright(c) 2009-2012 Keybridge Communication Technology Co.,Ltd.
* All rights reserved.
*
* 文件名称：db.cpp
* 摘    要：数据库操作统一接口
*
* 作    者:  杨雨衡 2011-02-27
****************************************************************/

#include "db.h"
#include <icrsint.h>

/**
 * @brief 构造函数
 */
CDataBase::CDataBase() 
{
	// 数据库连接参数
	memset(m_username, 0x0, sizeof(m_username));
	memset(m_password, 0x0, sizeof(m_password));
	memset(m_server_name, 0x0, sizeof(m_server_name));
	memset(m_dbname, 0x0, sizeof(m_dbname));
	
	// SQL 执行结果
	m_sqlcode = 0;
	memset(m_sqlerr, 0x0, sizeof(m_sqlerr));
	m_connected = false;

	memset(m_split_symbol, 0x0, sizeof(m_split_symbol));
	strcpy(m_split_symbol, "'^'");    // 默认分隔符
	memset(m_union_symbol, 0x0, sizeof(m_union_symbol));

	m_sql_buff = new char[MAX_SQL_LEN];
	ASSERT2(m_sql_buff != NULL, LOG_ERR);

	m_field_value = new char[MAX_FIELD_LEN];
	ASSERT2(m_field_value != NULL, LOG_ERR);
}

/**
 * @brief 析构函数
 */
CDataBase::~CDataBase() 
{
//	db_close();    ----这里会出现pure function call错误, 在vc下会出现连接错误
	if (m_sql_buff)
	{
		delete []m_sql_buff;
		m_sql_buff = NULL;
	}

	if (m_field_value)
	{
		delete []m_field_value;
		m_field_value = NULL;
	}
}

	
/**
 * @brief 数据库连接的用户名
 * @param [out] username 用户名
 */
void CDataBase::get_username(char* username)
{
	if (username != NULL)
	{
		sprintf(username, "%s", m_username);
	}
}

/**
 * @brief 设置数据库连接的用户名
 * @param [in] username 用户名
 */
void CDataBase::set_username(const char* username)
{
	if (username != NULL)
	{
		snprintf(m_username, sizeof(m_username), "%s", username);		
	}
}

/**
 * @brief 数据库服务器名称
 * @param [out] server_name 服务器名称
 */
void CDataBase::get_server_name(char* server_name)
{
	if (server_name != NULL )
	{
		sprintf(server_name, "%s", m_server_name);
	}
}

/**
 * @brief 设置数据库服务器名称
 * @param [in] server_name 服务器名称
 */
void CDataBase::set_server_name(const char* server_name)
{
	if (server_name != NULL)
	{
		strncpy(m_server_name, server_name, sizeof(m_server_name) - 1);
	}
}

/**
 * @brief 数据库名称
 * @param [out] dbname 数据库名称
 */
void CDataBase::get_dbname(char* dbname)
{
	if (dbname != NULL)
	{
		sprintf(dbname, "%s", m_dbname);
	}
}

/**
 * @brief 数据库名称
 * @param [out] dbname 数据库名称
 */
void CDataBase::set_dbname(const char* dbname)
{
	if (dbname != NULL)
	{
		strncpy(m_dbname, dbname, sizeof(m_dbname) - 1);
	}
}

/**
 * @brief 密码
 * @param [in] password 密码
 */
void CDataBase::set_password(const char* password)
{
	if (password != NULL)
	{
		strncpy(m_password, password, sizeof(m_password) - 1);
	}
}

/**
 * @brief 密码
 * @param [in] password 密码
 */
void CDataBase::get_password(char* password)
{
	if (password != NULL)
	{
		sprintf(password, "%s", m_password);
	}
}

/**
 * @brief 保存SQL语句的执行结果
 */
void CDataBase::set_sql_result(int sqlcode, int row)
{
	m_sqlcode = sqlcode;
	m_row = row;		
}


/**
 * @brief 返回SQL语句执行的sqlcode
 */
int CDataBase::get_sqlcode()
{
	return m_sqlcode;
}


/**
 * @brief 对select语句进行处理, 转换成使用连接分隔符对多个字段相连的单字段.
 *   注意: 不能使用select *, 要写出具体的字段名.
 * @param [in] oldsql    原select语句
 * @param [out] newsql   被转换后的select语句
 */
int CDataBase::get_sql(const char* param_oldsql, char* newsql)
{
	if (param_oldsql == NULL || newsql == NULL)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "sql is null.");
		return -1;
	}

	if (strlen(param_oldsql) >= MAX_CHANGE_SELECT_SQL_LEN)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "sql length = %d.", strlen(param_oldsql));
		return -1;
	}

	if (strcmp(m_union_symbol, "") == 0)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "没有指定字段连接符");
		return -1;
	}

	// 检查当前输入的SQL语句是否为select语句
	if (this->get_sql_dml(param_oldsql) != DML_SELECT)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "本函数仅供select使用, sql: [%s]", param_oldsql);
		return -1;
	}

	// 处理SQL语句
	char curr_sql[2 * MAX_CHANGE_SELECT_SQL_LEN] = {0};
	char oldsql[MAX_CHANGE_SELECT_SQL_LEN] = {0};
	char word[20];
	char* ptr;
	int i;
	strcpy(oldsql, param_oldsql);
	trim(oldsql);
	g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_DEBUG, "oldsql: [%s]", oldsql);
	i = strcspn(oldsql, " \t"); // 空格或者tab键
	// 将p指向"select "的下一个字符
	ptr = oldsql + i + 1;

    // 对于select count(*) from..., select count(1) from..., 不需要转换
	// 判断后8个字符是不是count(*)/count(1)
    memset(word, 0, sizeof(word));
	strncpy(word, ptr, 8);
	if (stricmp(word, "count(*)") == 0 || stricmp(word, "count(1)") == 0)
	{
        i = strcspn(ptr, " \t"); // 空格或者tab键
		memset(word, 0, sizeof(word));
		strncpy(word, ptr + i + 1, 4);  // from
		if (stricmp(word, "from") == 0)
		{
			strcpy(newsql, oldsql);
			return 0;
		}
		else
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "sql语句可能有问题!");
			return -1;
        }
	}

	strncpy(curr_sql, oldsql, i + 1);
	char* ptr_out = curr_sql;
	ptr_out = ptr_out + i + 1;

	// 循环处理字符",", 但需要注意函数的使用，例如嵌套函数
	int kuo_hao = 0;
	while (*ptr != 0)
	{
		// 如果当前字符串为最后一个列名(根据" from "或" as "判断), 则表示结束, 循环退出
		if (is_end_field(ptr))
		{
			sprintf(ptr_out, " %s%s ", m_union_symbol, m_split_symbol);
			strcat(ptr_out, ptr);
			break;
		}

		// 是否是函数
		if (*ptr == '(')
		{
            kuo_hao++;
        }

		if (*ptr == ')')
		{
            kuo_hao--;
        }

		// 当前字符是"," 且不是括弧内(函数)的东西
		if (*ptr == ',' && kuo_hao == 0)
		{
			// 对当前字符进行处理
			sprintf(ptr_out, " %s%s%s ", m_union_symbol, m_split_symbol, m_union_symbol);
			ptr_out = ptr_out + strlen(m_union_symbol) * 2 + strlen(m_split_symbol) + 1;
		}
		else
		{
			*ptr_out = *ptr;
			ptr_out++;
		}

		ptr++;
	}

	strcpy(newsql, curr_sql);
	g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_DEBUG, "newsql: [%s]", newsql);
	return 0;
}


// 检查当前的语句是否已经到了列名结尾(" from "或" as ", 默认" from ")
bool CDataBase::is_end_field(const char* sql)
{
	if (sql == NULL || strlen(sql) == 0)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "sql is null.");
		return false;
	}

	char tmp[7];  // " from "占7个字节
	memset(tmp, 0x0, sizeof(tmp));
	strncpy(tmp, sql, sizeof(tmp) - 1);
	if ((tmp[0] == ' ' || tmp[0] == '\t') && (strncmpi(tmp + 1, "from", 4) == 0) && (tmp[5] == ' ' || tmp[5] == '\t'))
	{
		return true;
	}

	return false;
}

// 获取sql操作类型
EnumDml CDataBase::get_sql_dml(const char* sql)
{
	// 检查参数
	if (sql == NULL || strlen(sql) == 0)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "sql is null");
		return DML_UNKNOWN;
	}

	char dml_oper[9] = {0};
    int n = strcspn(sql, " \t"); // 以空格或者tab键分隔
	strncpy(dml_oper, sql, n);

	// insert 命令
	if (strcmpi(dml_oper, "insert") == 0)
	{
		return DML_INSERT;
    }

	// update 命令
	if (strcmpi(dml_oper, "update") == 0)
	{
		return DML_UPDATE;
    }

	// delete 命令
	if (strcmpi(dml_oper, "delete") == 0)
	{
		return DML_DELETE;
    }

	// select 命令
	if (strcmpi(dml_oper, "select") == 0)
	{
		return DML_SELECT;
	}

	// 不可识别的命令
	return DML_UNKNOWN;
}



