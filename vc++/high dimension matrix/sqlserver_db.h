#ifndef _SQLSERVER_DB_H_
#define _SQLSERVER_DB_H_


#define WHERE "where "



// sqlserver���ݿ������
class CDbSqlServer: public CDataBase
{
public:
	CDbSqlServer();
	CDbSqlServer(const char* username, const char* password, const char* server_name, const char* dbname);
	virtual ~CDbSqlServer();
	
	// �����ݿ�����
	virtual bool db_open(const char* username, const char* password, const char* server_name, const char* dbname);
	
	// �����ݿ�����
	virtual bool db_open();
	
	// �ر����ݿ�����
	virtual void db_close(bool commit = true);
	
	// ��ʼ����
	virtual int db_begintran();
	
	// �ύ��ǰ����
	virtual void db_commit();
	
	// �ع�����
	virtual void db_rollback();
	
    // �ж����ݿ��Ƿ���������
	virtual int db_test_conn();

	// �������ݿ�
    virtual int db_re_connect();
	
	// ִ���޷��ؽ������SQL���(insert, delete, update)
	virtual int execute_sql(const char* sql);
	
	// ִ�з���һ����¼��select���
	virtual int execute_select_sql(const char* sql, ONE_RECORD_STRUCT &one_record);
	
	// ִ�з���һ���������¼��select���
	virtual int execute_select_sql(const char* sql, vector<ONE_RECORD_STRUCT> &data_vec);

	// ִ�д洢����
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
	// ��鵱ǰ������Ƿ��Ѿ�����" as "(sqlserver����ֶβ�ѯʹ��as name��ʽ)
	virtual bool is_end_field(const char* sql);

	// ����select��ѯ���
    int parse_select(int type, char* where, int len);

    // ����where����, �����and_or�ͷ���field_list��һ���ֶ���.
	int parse_where(char* where, int len);

    // ���ݱ������ֶ���/�ֶ�ֵ, ����insert sql���.
	int parse_insert_sql(int type, const char* table_name, char* sql, int len);

	// ���ݱ������ֶ���/�ֶ�ֵ, ����update sql���
	int parse_update_sql(int type, const char* table_name, char* sql, int len);

	// ���ݱ������ֶ���/�ֶ�ֵ, ����delete sql���
	int parse_delete_sql(int type, const char* table_name, char* sql, int len);

    // ���ݱ������ֶ���/�ֶ�ֵ, ����insert, delete��update sql���.
	int parse_add_del_update_sql(int type, const char* table_name, char* sql, int len);

    // ���±���Ϣ(��������/�޸�/ɾ��)
	int update_table_info(const char* table_name, MSG_HEAD &head, MSG_BODY &res_body, CMem &xml_mem);

    // ��where��ѯ�������ֶ�ָ���������
	void change_where_option( char* where, char* new_where, int new_where_len, const map<char*, char*> &field_map);

	// ���������ֵ, ���θ�������field list��ֵ
	int assign_field(FIELD_STRUCT field_arr[], int size, MSG_BODY &res_body);

	// �������������, ���θ�������value��ֵ
	int get_assign_value(FIELD_STRUCT field_arr[], int size, TADOQuery* ado_qry, MSG_BODY &res_body, CMem &xml_mem);
	
    //�������ݵ�������װSQL ����
    int assign_accept_value(int  field_type, char* field_value,unsigned int field_len, char* field_memory);
       
	// ��TBytes���͵Ķ��������ݽ���base64����
	int base64_code(TBytes bin_data, int bin_len, char *base64_str, int base64_len);

	// ��TStream���͵Ķ��������ݽ���base64����
	int base64_code(TStream *stream, long stream_size, char* base64_str, int base64_len);




	TADOConnection* m_conn;  // ���ݿ�����
	TADOQuery* m_ado_qry;
	TADOStoredProc *m_store_proc;   // �洢����


	// ������ͷ
	MSG_HEAD m_req_msg_head;

    // ��������
	MSG_BODY m_req_msg_body;

	
private:
    // ��ʼ��
	void init();

		
};


#endif
