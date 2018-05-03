/****************************************************************
* Copyright(c) 2009-2012 Keybridge Communication Technology Co.,Ltd.
* All rights reserved.
*
* �ļ����ƣ�db.cpp
* ժ    Ҫ�����ݿ����ͳһ�ӿ�
*
* ��    ��:  ����� 2011-02-27
****************************************************************/

#include "db.h"
#include <icrsint.h>

/**
 * @brief ���캯��
 */
CDataBase::CDataBase() 
{
	// ���ݿ����Ӳ���
	memset(m_username, 0x0, sizeof(m_username));
	memset(m_password, 0x0, sizeof(m_password));
	memset(m_server_name, 0x0, sizeof(m_server_name));
	memset(m_dbname, 0x0, sizeof(m_dbname));
	
	// SQL ִ�н��
	m_sqlcode = 0;
	memset(m_sqlerr, 0x0, sizeof(m_sqlerr));
	m_connected = false;

	memset(m_split_symbol, 0x0, sizeof(m_split_symbol));
	strcpy(m_split_symbol, "'^'");    // Ĭ�Ϸָ���
	memset(m_union_symbol, 0x0, sizeof(m_union_symbol));

	m_sql_buff = new char[MAX_SQL_LEN];
	ASSERT2(m_sql_buff != NULL, LOG_ERR);

	m_field_value = new char[MAX_FIELD_LEN];
	ASSERT2(m_field_value != NULL, LOG_ERR);
}

/**
 * @brief ��������
 */
CDataBase::~CDataBase() 
{
//	db_close();    ----��������pure function call����, ��vc�»�������Ӵ���
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
 * @brief ���ݿ����ӵ��û���
 * @param [out] username �û���
 */
void CDataBase::get_username(char* username)
{
	if (username != NULL)
	{
		sprintf(username, "%s", m_username);
	}
}

/**
 * @brief �������ݿ����ӵ��û���
 * @param [in] username �û���
 */
void CDataBase::set_username(const char* username)
{
	if (username != NULL)
	{
		snprintf(m_username, sizeof(m_username), "%s", username);		
	}
}

/**
 * @brief ���ݿ����������
 * @param [out] server_name ����������
 */
void CDataBase::get_server_name(char* server_name)
{
	if (server_name != NULL )
	{
		sprintf(server_name, "%s", m_server_name);
	}
}

/**
 * @brief �������ݿ����������
 * @param [in] server_name ����������
 */
void CDataBase::set_server_name(const char* server_name)
{
	if (server_name != NULL)
	{
		strncpy(m_server_name, server_name, sizeof(m_server_name) - 1);
	}
}

/**
 * @brief ���ݿ�����
 * @param [out] dbname ���ݿ�����
 */
void CDataBase::get_dbname(char* dbname)
{
	if (dbname != NULL)
	{
		sprintf(dbname, "%s", m_dbname);
	}
}

/**
 * @brief ���ݿ�����
 * @param [out] dbname ���ݿ�����
 */
void CDataBase::set_dbname(const char* dbname)
{
	if (dbname != NULL)
	{
		strncpy(m_dbname, dbname, sizeof(m_dbname) - 1);
	}
}

/**
 * @brief ����
 * @param [in] password ����
 */
void CDataBase::set_password(const char* password)
{
	if (password != NULL)
	{
		strncpy(m_password, password, sizeof(m_password) - 1);
	}
}

/**
 * @brief ����
 * @param [in] password ����
 */
void CDataBase::get_password(char* password)
{
	if (password != NULL)
	{
		sprintf(password, "%s", m_password);
	}
}

/**
 * @brief ����SQL����ִ�н��
 */
void CDataBase::set_sql_result(int sqlcode, int row)
{
	m_sqlcode = sqlcode;
	m_row = row;		
}


/**
 * @brief ����SQL���ִ�е�sqlcode
 */
int CDataBase::get_sqlcode()
{
	return m_sqlcode;
}


/**
 * @brief ��select�����д���, ת����ʹ�����ӷָ����Զ���ֶ������ĵ��ֶ�.
 *   ע��: ����ʹ��select *, Ҫд��������ֶ���.
 * @param [in] oldsql    ԭselect���
 * @param [out] newsql   ��ת�����select���
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
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "û��ָ���ֶ����ӷ�");
		return -1;
	}

	// ��鵱ǰ�����SQL����Ƿ�Ϊselect���
	if (this->get_sql_dml(param_oldsql) != DML_SELECT)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "����������selectʹ��, sql: [%s]", param_oldsql);
		return -1;
	}

	// ����SQL���
	char curr_sql[2 * MAX_CHANGE_SELECT_SQL_LEN] = {0};
	char oldsql[MAX_CHANGE_SELECT_SQL_LEN] = {0};
	char word[20];
	char* ptr;
	int i;
	strcpy(oldsql, param_oldsql);
	trim(oldsql);
	g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_DEBUG, "oldsql: [%s]", oldsql);
	i = strcspn(oldsql, " \t"); // �ո����tab��
	// ��pָ��"select "����һ���ַ�
	ptr = oldsql + i + 1;

    // ����select count(*) from..., select count(1) from..., ����Ҫת��
	// �жϺ�8���ַ��ǲ���count(*)/count(1)
    memset(word, 0, sizeof(word));
	strncpy(word, ptr, 8);
	if (stricmp(word, "count(*)") == 0 || stricmp(word, "count(1)") == 0)
	{
        i = strcspn(ptr, " \t"); // �ո����tab��
		memset(word, 0, sizeof(word));
		strncpy(word, ptr + i + 1, 4);  // from
		if (stricmp(word, "from") == 0)
		{
			strcpy(newsql, oldsql);
			return 0;
		}
		else
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "sql������������!");
			return -1;
        }
	}

	strncpy(curr_sql, oldsql, i + 1);
	char* ptr_out = curr_sql;
	ptr_out = ptr_out + i + 1;

	// ѭ�������ַ�",", ����Ҫע�⺯����ʹ�ã�����Ƕ�׺���
	int kuo_hao = 0;
	while (*ptr != 0)
	{
		// �����ǰ�ַ���Ϊ���һ������(����" from "��" as "�ж�), ���ʾ����, ѭ���˳�
		if (is_end_field(ptr))
		{
			sprintf(ptr_out, " %s%s ", m_union_symbol, m_split_symbol);
			strcat(ptr_out, ptr);
			break;
		}

		// �Ƿ��Ǻ���
		if (*ptr == '(')
		{
            kuo_hao++;
        }

		if (*ptr == ')')
		{
            kuo_hao--;
        }

		// ��ǰ�ַ���"," �Ҳ���������(����)�Ķ���
		if (*ptr == ',' && kuo_hao == 0)
		{
			// �Ե�ǰ�ַ����д���
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


// ��鵱ǰ������Ƿ��Ѿ�����������β(" from "��" as ", Ĭ��" from ")
bool CDataBase::is_end_field(const char* sql)
{
	if (sql == NULL || strlen(sql) == 0)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "sql is null.");
		return false;
	}

	char tmp[7];  // " from "ռ7���ֽ�
	memset(tmp, 0x0, sizeof(tmp));
	strncpy(tmp, sql, sizeof(tmp) - 1);
	if ((tmp[0] == ' ' || tmp[0] == '\t') && (strncmpi(tmp + 1, "from", 4) == 0) && (tmp[5] == ' ' || tmp[5] == '\t'))
	{
		return true;
	}

	return false;
}

// ��ȡsql��������
EnumDml CDataBase::get_sql_dml(const char* sql)
{
	// ������
	if (sql == NULL || strlen(sql) == 0)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "sql is null");
		return DML_UNKNOWN;
	}

	char dml_oper[9] = {0};
    int n = strcspn(sql, " \t"); // �Կո����tab���ָ�
	strncpy(dml_oper, sql, n);

	// insert ����
	if (strcmpi(dml_oper, "insert") == 0)
	{
		return DML_INSERT;
    }

	// update ����
	if (strcmpi(dml_oper, "update") == 0)
	{
		return DML_UPDATE;
    }

	// delete ����
	if (strcmpi(dml_oper, "delete") == 0)
	{
		return DML_DELETE;
    }

	// select ����
	if (strcmpi(dml_oper, "select") == 0)
	{
		return DML_SELECT;
	}

	// ����ʶ�������
	return DML_UNKNOWN;
}



