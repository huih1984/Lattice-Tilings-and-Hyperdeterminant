/****************************************************************
* Copyright(c) 2009-2012 Keybridge Communication Technology Co.,Ltd.
* All rights reserved.
*
* �ļ����ƣ�sqlserver_db.cpp
* ժ    Ҫ��sqlserver���ݿ⴦��
*
* ��    ��:  ����� 2011-02-28
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
	strcpy(m_split_symbol, "'^#'");  // Ϊ������ֶ�ֵ���ֲ���, ����ʹ�ö���ַ�
	strcpy(m_union_symbol, "+");     // sqlserver���ӷ�
	m_connected = false;
	m_conn = new TADOConnection(NULL);
	m_ado_qry = new TADOQuery(NULL);
	m_store_proc = new TADOStoredProc(NULL);
}

// ��������: �ر����ݿ�����
CDbSqlServer::~CDbSqlServer()
{
    db_close();
}


// �����ݿ�����
// �Զ��������������ʱû��.
bool CDbSqlServer::db_open()
{
	// ���ݿ�������
	if (m_connected)
	{
		return true;
	}
	
	try
	{  
        m_conn->ConnectionString = AnsiString(g_cfg.m_sql_server_conn_str);
		m_conn->Connected = true;
		m_ado_qry->Connection = m_conn;    // ָ�����ݿ�����
		m_store_proc->Connection = m_conn;
	}
	catch (...)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "�������ݿ�[user:%s, password:%s, srvname:%s, dbname:%s]ʧ��!", m_username, m_password, m_server_name, m_dbname);
		return false;
	}

	m_connected = true;
	return true;	
}


// �����ݿ�����
bool CDbSqlServer::db_open(const char* username, const char* password, const char* server_name, const char * dbname)
{
	strcpy(m_username, username);
	strcpy(m_password, password);
	strcpy(m_server_name, server_name);
	strcpy(m_dbname, dbname);
	return db_open();
}

// �ر����ݿ�����
// bool commit: �Ƿ��Զ��ύ����
void CDbSqlServer::db_close(bool commit)
{
	// ������ݿ������Ƿ�Ͽ�
	if (!m_connected)
	{
		return;
	}

	// �Ͽ�����, �ͷ���Դ
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

// ��ʼ����
int CDbSqlServer::db_begintran()
{
	return m_conn->BeginTrans();
}

// �ύ��ǰ����
void CDbSqlServer::db_commit()
{
	m_conn->CommitTrans();
}

// �ع�����
void CDbSqlServer::db_rollback()
{
	m_conn->RollbackTrans();
}

// �ж����ݿ��Ƿ���������
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
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "���ݿ��쳣:%s%s, error_code:%d", dbe.ClassName().t_str(), dbe.Message.t_str(), dbe.ErrorCode);
		if (dbe.ErrorCode == SQLSERVER_DISCONNECT_ERRCODE)
		{
        	return db_re_connect();  // ����DB
		}
	}
    catch (Exception& e)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "�쳣:%s%s", e.ClassName().t_str(), e.Message.t_str());
		return DB_ACTION_EXECUTE_FAILED;
	}

    return ret;
}

// �������ݿ�
int CDbSqlServer::db_re_connect()
{
	// DB���ӶϿ�, ����, ��������3��, 3�ζ����ɹ��ͷ�������ʧ��
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
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "%s%s, �������ݿ�[user:%s, password:%s, srvname:%s, dbname:%s]ʧ��!", e.ClassName().t_str(), e.Message.t_str(), m_username, m_password, m_server_name, m_dbname);
			continue;
		}

		if (m_conn->Connected)
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_INFO, "�������ݿ�[user:%s, password:%s, srvname:%s, dbname:%s]�ɹ�.", m_username, m_password, m_server_name, m_dbname);
			return SUCCESS;
        }
	}

	if (i == DB_RE_CONNECT_COUNT)
	{
        g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "�������ݿ�[user:%s, password:%s, srvname:%s, dbname:%s]3��ʧ��!", m_username, m_password, m_server_name, m_dbname);
		return DB_CONNECT_FAILED;
	}

	return SUCCESS;
}

// ִ���޷��ؽ������SQL���(insert, delete, update)
// �쳣�ڵ��ú����д���
int CDbSqlServer::execute_sql(const char* sql)
{
	g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_DEBUG, "sql:[%s]", sql);
	m_ado_qry->SQL->Clear();   // ���sql
	m_ado_qry->SQL->Add(sql);
	m_ado_qry->ParamCheck = true;
	int ret_code = m_ado_qry->ExecSQL();
	g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_DEBUG, "ret_code: %d.", ret_code);
	return 0;
}

// ִ�з���һ����¼��select���
int CDbSqlServer::execute_select_sql(const char* sql, ONE_RECORD_STRUCT &one_record)
{
	vector<ONE_RECORD_STRUCT> data_vec;
	execute_select_sql(sql, data_vec);

	// �鲻����¼
	if (data_vec.size() == 0)
	{
		return DB_NOT_FOUND_DATA;
	}

	// �鵽������¼
	if (data_vec.size() > 1)
	{
		return DB_MORE_DATAS;
	}

	memset(one_record.m_value, 0, sizeof(one_record.m_value));
	strcpy(one_record.m_value, data_vec[0].m_value);
	return 0;
}


/* ִ�з���һ���������¼��select���.
 * sqlserver��Ҫ�������ֶ�ת��Ϊ�ַ���. ----oralce�������Զ�ת��,�����ͺ��ַ���?
 * select����ʽ: select name, convert(varchar(10), id), ... as dbname from table_name ...
 *	 select֮����ֶζ�ת��Ϊ�ַ���, sqlserver��ѯ����ֶ�һ��Ҫ����as name, ����adoȡֵ.
 */
int CDbSqlServer::execute_select_sql(const char* sql, vector<ONE_RECORD_STRUCT> &data_vec)
{
	char newsql[MAX_CHANGE_SELECT_SQL_LEN] = {0};
    int ret;
	ret = get_sql(sql, newsql); // ת��sql���
	if (0 != ret)
	{
		return ret;
	}

    ONE_RECORD_STRUCT one_record;
	m_ado_qry->SQL->Clear();   // ���sql
	m_ado_qry->SQL->Add(newsql);
	m_ado_qry->ParamCheck = true;
	m_ado_qry->Open();
	m_ado_qry->First();
	while (!m_ado_qry->Eof)
	{
        // ȡ������Ϊ1�ļ�¼
		snprintf(one_record.m_value, sizeof(one_record.m_value), "%s", m_ado_qry->Fields->FieldByNumber(1)->AsAnsiString.Trim().c_str());
		data_vec.push_back(one_record);

		// ȡ��һ����¼
		m_ado_qry->Next();
	}
	m_ado_qry->Close();
	return 0;
}

// ��鵱ǰ������Ƿ��Ѿ�����" as "(sqlserver��ѯ����ֶ�Ҫʹ��"as name"��ʽ)
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


// ����select��ѯ���, ����ѯ�������߲�����ѯ����. ��ѯsql�����Ҫ�ڵ��ú����й���.
// select: field_list��data����where����.
int CDbSqlServer::parse_select(int type, char* where, int len)
{
	memset(where, 0, len);
	if (type == DBES_SELECT_NOWHERE_OPTION)  // ������ѯ����
	{
        if (m_req_msg_body.value_list.size != 0)
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "��������ѯʱ, body�в��ܴ���ѯ����!");
			return NO_WHERE_OPTION_ERROR;
		}

		return SUCCESS;
	}
	else if (type == DBES_SELECT_WHERE_OPTION)  // ��Ҫ�в�ѯ����
	{
		if (m_req_msg_body.value_list.size == 0)
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "��������ѯʱ, body��û�в�ѯ����!");
			return MSG_BODY_EMPTY;
		}

		return parse_where(where, len);
	}
	else
	{
        g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "���ǲ�ѯ����!");
		return XML_HEAD_TYPE_ERROR;
    }
}


// ����where����, �����and_or�ͷ���field_list��һ���ֶ���.
// ��body�е��ֶ�����ֵ����and/or�����һ��, ����where�Ӿ�.
int CDbSqlServer::parse_where(char* where, int len)
{
	memset(where, 0, len);
    if (m_req_msg_body.value_list.size == 0)
	{
		return SUCCESS;
	}

	PARAM_FIELD_STRUCT *ptr = m_req_msg_body.value_list.head;

	// and_orҪ����field_list�ĵ�һ���ֶ��У����û��Ĭ��Ϊand
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
        // �ֶ���
		snprintf(where + off_set, len - off_set, "(%s ", m_req_msg_body.field_list[i].m_field_name);
		off_set = strlen(where);

        // �ֶ�ֵ
		memset(m_field_value, 0, MAX_FIELD_LEN);
		strncpy(m_field_value, ptr->m_value, ptr->m_len);  // ����ָ�����ȵ��ַ���
		snprintf(where + off_set, len - off_set, "%s)", m_field_value);
		off_set = strlen(where);
		if (off_set >= len)
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "����Խ��.");
			return ARRAY_OVERFLOW;
		}

		if (i != (m_req_msg_body.value_list.size - 1))   // �������һ������
		{
			snprintf(where + off_set, len - off_set, " %s ", and_or);   // and/or
			off_set = strlen(where);
			if (off_set >= len)
			{
				g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "����Խ��.");
				return ARRAY_OVERFLOW;
			}
		}

		ptr = ptr->next;
	}

	return SUCCESS;
}


/**
 * @brief  �Ѷ����ƺ��������͵����ݱ��浽field_memory�У����ж����Ƶ�����Ҫ����
 *           base64�Ľ���
 * @param [in] field_type �ֶ�����
 * @param [in] field_value �ֶ�value
 * @param [in] field_len �ֶγ���
 * @param [out] field_memory ����ת�����ַ�
 * ����ֵ: ת����������ַ����ĳ���
 * @author��hongliming 2011-07-29
 */
int CDbSqlServer::assign_accept_value(int  field_type, char* field_value,unsigned int field_len, char* field_memory)
{
    int ret = 0;
    int dstlen = MAX_FIELD_LEN;
    if ((NULL == field_value) || (NULL == field_memory))
    {
        return PARA_ERROR;
	}

	// �������������, ���θ�������value��ֵ
	if((FIXED_BINARY_DATA_TYPE_ID == field_type) || (VAR_BINARY_DATA_TYPE_ID == field_type))
	{
		char * buf = new char[MAX_FIELD_LEN];
		ASSERT2(buf != NULL, LOG_ERR);
		int hex_len = MAX_FIELD_LEN;
		if(!base64_decode(field_value, field_len, field_memory, dstlen))
		{
			delete []buf;
			buf = NULL;
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "base64����ʧ��.");
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


// ���ݱ������ֶ���/�ֶ�ֵ, ����insert sql���. �ֶ�����ֵ�Ƕ�Ӧ��.
// insert: field_list��data�ж�Ӧ�Ķ����ֶ�����ֵ;
// eg: insert into dbo.task_result_table(task_id, device_id, result) values(123, 260, 'abc2345')
int CDbSqlServer::parse_insert_sql(int type, const char* table_name, char* sql, int len)
{
	int i, off_set = 0;
    int field_type = 0;

	if (type != DBES_INSERT_OPTION)
	{
        g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "�������Ӳ���!");
		return XML_HEAD_TYPE_ERROR;
	}

    if (m_req_msg_body.value_list.size == 0)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "body��û���ֶ�ֵ!");
		return MSG_BODY_EMPTY;
	}

	PARAM_FIELD_STRUCT *ptr = m_req_msg_body.value_list.head;
	memset(sql, 0, len);
	snprintf(sql, len, "insert into %s(", table_name);
	off_set = strlen(sql);
    // �����ֶ���, �ֶ���һ�㲻��, ����û���ж�����Խ��
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
	// ����ֵ
    for (i = 0; i < m_req_msg_body.value_list.size; i++, ptr = ptr->next)
	{
        // �ֶ�ֵ
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
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "����Խ��.");
			return ARRAY_OVERFLOW;
		}

		if (i != (m_req_msg_body.value_list.size - 1))
		{
			snprintf(sql + off_set, len - off_set, ", ");
			off_set = strlen(sql);
			if (off_set >= len)
			{
				g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "����Խ��.");
				return ARRAY_OVERFLOW;
			}
		}
	}

    snprintf(sql + off_set, len - off_set, ")");
	off_set = strlen(sql);
	return SUCCESS;
}


// ���ݱ������ֶ���/�ֶ�ֵ, ����update sql���, set���������ֵ, where�������п�û��.
// update: ��Ҫ����set�ֶ�����where�ֶ���, ���������ֶ������[set], [where], ��dbname[set], dbname[where];
// eg: update dbo.alarm_details_current set op_status = '��ȷ��', alarm_status = '�Ѳ���' where trap_id = 1234
int CDbSqlServer::parse_update_sql(int type, const char* table_name, char* sql, int len)
{
	int filed_len, off_set = 0;
//	bool is_have_and_or = false;//�Ƿ���and_or���
	char set_where[8]; // �ֶ������5����7���ַ�, ��[set]��[where]
    int field_type = 0;

	if (type != DBES_UPDATE_OPTION)    // where��������, Ҳ����û��
	{
        g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "����update����!");
		return XML_HEAD_TYPE_ERROR;
	}

    if (m_req_msg_body.value_list.size == 0)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "body��û���ֶ�ֵ!");
		return MSG_BODY_EMPTY;
	}

	// and_orҪ����field_list�ĵ�һ���ֶ��У����û��Ĭ��Ϊand
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

    // ����set���õ��ֶ�ֵ
	for (; i < m_req_msg_body.value_list.size; i++, ptr = ptr->next)
	{
        // �ж���[set]����[where]
		filed_len = strlen(m_req_msg_body.field_list[i].m_field_name);

		// ��ȡ��5���ַ�
        if (filed_len <= 5)
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "�ֶ���̫��!");
			return -1;
		}
		memset(set_where, 0, sizeof(set_where));
		strncpy(set_where, &(m_req_msg_body.field_list[i].m_field_name[filed_len - 5]), sizeof(set_where) - 1);

//		if(is_have_and_or)
//		{
//			if (i==1 && stricmp(set_where, "here]") == 0)  // ��һ���ֶ���where����
//			{
//				g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "��Ϣ��ʽ����!");
//				return -1;
//			}
//		}
//		else
//		{
//			if (i==0 && stricmp(set_where, "here]") == 0)  // ��һ���ֶ���where����
//			{
//				g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "��Ϣ��ʽ����!");
//				return -1;
//			}
//		}

		// [set]
		if (stricmp(set_where, "[set]") == 0)
		{
			m_req_msg_body.field_list[i].m_field_name[filed_len - 5] = '\0';  // ȥ��[set]
		}
		else
		{
			// ȥ����������ַ�", "
            sql[off_set - 2] = '\0';
            off_set -= 2;
			break;
        }

		// �ֶ���
		snprintf(sql + off_set, len - off_set, "%s ", m_req_msg_body.field_list[i].m_field_name);
		off_set = strlen(sql);

        // �ֶ�ֵ
		memset(m_field_value, 0, MAX_FIELD_LEN);
        field_type = m_req_msg_body.field_list[i].m_type;        
        if(assign_accept_value(field_type, ptr->m_value, ptr->m_len,m_field_value) != SUCCESS)
        {
            return -1;
          
        }
        /*����������Ҫ����=*/
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
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "����Խ��.");
			return ARRAY_OVERFLOW;
		}
	}

	snprintf(sql + off_set, len - off_set, " where");
	off_set = strlen(sql);
	// ����where����
    for (; i < m_req_msg_body.value_list.size; i++, ptr = ptr->next)
	{
		// �ж���[set]����[where]
		filed_len = strlen(m_req_msg_body.field_list[i].m_field_name);

		// ��ȡ��7���ַ�
        if (filed_len <= 7)
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "�ֶ���̫��!");
			return -1;
		}
		memset(set_where, 0, sizeof(set_where));
		strncpy(set_where, &(m_req_msg_body.field_list[i].m_field_name[filed_len - 7]), sizeof(set_where) - 1);

		// [where]
		if (stricmp(set_where, "[where]") == 0)
		{
        	m_req_msg_body.field_list[i].m_field_name[filed_len - 7] = '\0';  // ȥ��[where]
		}
		else
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "��Ϣ��ʽ����!");
			return -1;
        }

        // �ֶ���, �ֶ�����ֵ��()������
		snprintf(sql + off_set, len - off_set, " (%s ", m_req_msg_body.field_list[i].m_field_name);
        off_set = strlen(sql);

        // �ֶ�ֵ
		memset(m_field_value, 0, MAX_FIELD_LEN);
		strncpy(m_field_value, ptr->m_value, ptr->m_len);  // ����ָ�����ȵ��ַ���
		snprintf(sql + off_set, len - off_set, "%s) ", m_field_value);
		off_set = strlen(sql);
        if (off_set >= len)
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "����Խ��.");
			return ARRAY_OVERFLOW;
		}

		if (i != (m_req_msg_body.value_list.size - 1))   // �������һ������
		{
			snprintf(sql + off_set, len - off_set, " %s ", and_or);  // and/or
			off_set = strlen(sql);
			if (off_set >= len)
			{
				g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "����Խ��.");
				return ARRAY_OVERFLOW;
			}
		}
	}

	return SUCCESS;
}

// ���ݱ������ֶ���/�ֶ�ֵ, ����delete sql���, where���п�û��.
// delete: field_list��data����where����;
// eg: delete from alarm_details_history where trap_id in ("aaa")
int CDbSqlServer::parse_delete_sql(int type, const char* table_name, char* sql, int len)
{
	if (type != DBES_DELETE_OPTION)    // where��������, Ҳ����û��
	{
        g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "����delete����!");
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
        	g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "����Խ��.");
			return ARRAY_OVERFLOW;
        }
	}

	return ret;
}


// ���ݱ������ֶ���/�ֶ�ֵ, ����insert, delete��update sql���.
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


// ���±���Ϣ(��������/�޸�/ɾ��)
// ע��: ֻ��update������
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
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "���ݿ��쳣:%s%s, error_code:%d", dbe.ClassName().t_str(), dbe.Message.t_str(), dbe.ErrorCode);
		if (dbe.ErrorCode == SQLSERVER_DISCONNECT_ERRCODE)
		{
        	db_re_connect();  // ����DB
		}
		ret = DB_ACTION_EXECUTE_FAILED;
	}
    catch (Exception& e)
	{
        g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "�쳣:%s%s", e.ClassName().t_str(), e.Message.t_str());
		ret = DB_ACTION_EXECUTE_FAILED;
	}

	return ret;
}


// ��where��ѯ�������ֶ�ָ���������
// map<char*, char*> field_map: key��ʾbody�е�field, value��ʾ��ѯ����о�������ݿ����.
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
		ptr = strstr(new_where, it->first); // �ҵ���ѯ�����е��ֶ�
		if (ptr != NULL)
		{
			memset(temp, 0, sizeof(temp));

			ptr_len = strlen(ptr);
			front_len = ptr - new_where;
			strncpy(temp, new_where, front_len);   // �����ֶ���ǰ����ַ���
			offset = strlen(temp);
			strcpy(temp + offset, it->second);     // ���滻�����ֶ���
			offset = strlen(temp);
			strcpy(temp + offset, ptr + strlen(it->first)); // �����ֶ���������ַ���

			// �õ��µ�where���
			strcpy(new_where, temp);
		}
	}

    g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_DEBUG, "new_where: [%s]", new_where);
}


// ִ�д洢����, key:�洢���̵��������, value:�����洢���̵�ֵ.
void CDbSqlServer::exec_store_proc(const char* sp_name, const map<char*, char*> &input_param_map)
{
	m_store_proc->ProcedureName = sp_name;  // �洢��������
	m_store_proc->Parameters->Clear();
	m_store_proc->Parameters->Refresh();

	map<char*, char*>::const_iterator it;
	for (it = input_param_map.begin(); it != input_param_map.end(); it++)
	{
		m_store_proc->Parameters->ParamByName(it->first)->Value = it->second;
	}

	m_store_proc->ExecProc();
}


// ���������ֵ, ���θ�������field list��ֵ
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


// �������������, ���θ�������value��ֵ
int CDbSqlServer::get_assign_value(FIELD_STRUCT field_arr[], int size, TADOQuery* ado_qry, MSG_BODY &res_body, CMem &xml_mem)
{
	int ret;

	// �ֶ������������ܳ������ֵ
	if (size > MAX_FIELD_LIST_SIZE)
	{
    	return ARRAY_OVERFLOW;
	}

    // ���ֶ�ֵ�浽������ڴ���
	for (int i = 0; i < size; ++i)
	{
        // ����
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
		else if (field_arr[i].m_type == CHAR_DATA_TYPE_ID)  // �ַ���
		{
			ret = CMsg::assign(res_body, ado_qry->FieldByName(field_arr[i].m_field_name)->AsAnsiString.Trim().c_str(), xml_mem);
		}
		else if (field_arr[i].m_type == FIXED_BINARY_DATA_TYPE_ID)  // ��������������, ʹ��TBytes
		{
			TBytes fixed_data = ado_qry->FieldByName(field_arr[i].m_field_name)->AsBytes;
            // base64����, m_field_value���base64�������ַ���
			ret = base64_code(fixed_data, fixed_data.Length, m_field_value, MAX_FIELD_LEN);   // base64����
			if (ret != 0)
			{
				return ret;
			}

			ret = CMsg::assign(res_body, m_field_value, xml_mem);
		}
		else if (field_arr[i].m_type == VAR_BINARY_DATA_TYPE_ID)  // �䳤����������, ʹ��TStream
		{
			TStream *stream = ado_qry->CreateBlobStream(ado_qry->FieldByName(field_arr[i].m_field_name), bmRead);
			// base64����, m_field_value���base64�������ַ���
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
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "��������%d����!", field_arr[i].m_type);
			return DATA_STRUCT_ID_ERROR;
		}

		// ������Ƿ���ȷ
		if (ret != 0)
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "����ret:%d!", ret);
			return ret;
		}
	}

	return ret;
}


/**
 * @brief ��TBytes���͵Ķ��������ݽ���base64����
 * @param [in] bin_data  ��Ҫbase64�����TBytes���͵Ķ���������, һ���Ƕ����Ķ���������
 * @param [in] bin_len  bin_data���ֽ���
 * @param [out] base64_str ��ŶԶ��������ݽ���base64����֮����ַ���
 * @param [in] base64_len ���base64����֮����ַ����Ļ�������С
 * ����ֵ: 0��ʾ�����ɹ�, ������ʾ����.
 */
int CDbSqlServer::base64_code(TBytes bin_data, int bin_len, char *base64_str, int base64_len)
{
	bool flag;
	char bin_str[1024] = {0};

	if (bin_len > sizeof(bin_str))
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "����ռ䲻��!");
		return ARRAY_OVERFLOW;
	}

	// ���������ݴ��ڽ���base64����, �����ھ���Ϊ���ַ���
	if (bin_len > 0)
	{
		for (int i = 0; i < bin_len; i++)
		{
			bin_str[i] = bin_data[i];
		}

		flag = base64_encode(bin_str, bin_len, base64_str, base64_len);
		if (!flag)
		{
			g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "base64����ʧ��!");
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
 * @brief ��TStream���͵Ķ��������ݽ���base64����
 * @param [in] stream  ��Ҫbase64�����TStream���͵Ķ���������
 * @param [in] stream_size stream���ֽ���
 * @param [out] base64_str ��ŶԶ��������ݽ���base64����֮����ַ���
 * @param [in] base64_len ���base64����֮����ַ����Ļ�������С
 * ����ֵ: 0��ʾ�����ɹ�, ������ʾ����.
 * ˵��: ���ú����÷�����, ʹ��m_field_value��Ա����:
     TStream *stream = m_ado_qry->CreateBlobStream(m_ado_qry->FieldByName("icon_pic"), bmRead);
     ret = base64_code(stream, stream->Size, m_field_value, MAX_FIELD_LEN);
 */
int CDbSqlServer::base64_code(TStream *stream, long stream_size, char* base64_str, int base64_len)
{
	// ���ܳ����ֶ���󳤶�
	if (stream_size >= MAX_FIELD_LEN)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "�ֶγ���̫��!");
		return FIELD_LENGTH_TOO_BIG;
	}

    // �����ھ���Ϊ���ַ���
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
    	g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "�����ڴ�ʧ��!");
		return ALLOC_MEMORY_FAIL;
	}

	stream->ReadBuffer(bin_data, stream_size);
	flag = base64_encode(bin_data, stream_size, base64_str, base64_len);
	if (!flag)
	{
		g_log.WriteLog(__FILE__, __FUNC__, __LINE__, LOG_ERR, "base64����ʧ��!");
		ret = BASE64_ENCODE_ERROR;
	}

	delete []bin_data;
	return ret;
}


