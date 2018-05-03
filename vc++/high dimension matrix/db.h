
#ifndef _DB_H_
#define _DB_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <vector>
using namespace std;




// sql������󳤶�, 2M
#define MAX_SQL_LEN (2 * MB)

// ת��Ϊ�����ֶβ�ѯ��sql������󳤶�
#define MAX_CHANGE_SELECT_SQL_LEN 2047

// ��������ֽ���, ���ﲻ��������������.
#define RECORD_MAX_LEN 1023

// ���ݿ���������
#define DB_RE_CONNECT_COUNT 3

// where�����ֶε���󳤶�
#define MAX_WHERE_LEN 1024




// ���ݿ�����
enum EnumDbType
{
	DB_ORACLE = 1, 	    // oracle ���ݿ�
	DB_SYSBASE = 2,	    // sysbase ���ݿ�
	DB_DB2 = 3,			// db2 ���ݿ�
	DB_SQLSERVER = 4,   // sqlserver ���ݿ�
	DB_MYSQL = 5        // mysql ���ݿ�
};


// sql��DML����
enum EnumDml
{
	DML_UNKNOWN = 1,	// δ����Ĳ���, ����Call, �ڵ�ǰ�����²������е�����
	DML_INSERT = 2, 	// insert ����
	DML_UPDATE = 3, 	// update ����
	DML_DELETE = 4, 	// delete ����
	DML_SELECT = 5  	// select ����
};


// ʹ��select���ȡ���ĵ�����¼ֵ
typedef struct
{
	char m_value[RECORD_MAX_LEN + 1];
}ONE_RECORD_STRUCT;


// ���ݿ������
class CDataBase
{
public:
	CDataBase();
	virtual ~CDataBase();
	
	// �����ݿ�����
	virtual bool db_open(const char* username, const char* password, const char* server_name, const char* dbname) = 0;
	
	// �����ݿ�����
	virtual bool db_open() = 0;
	
	// �ر����ݿ�����
	virtual void db_close(bool commit = true) = 0;
	
	// ��ʼ����
	virtual int db_begintran() = 0;
	
	// �ύ��ǰ����
	virtual void db_commit() = 0;
	
	// �ع�����
	virtual void db_rollback() = 0;
	
	// �������ݿ�
	virtual int db_re_connect() = 0;

	// �ж����ݿ��Ƿ���������
	virtual int db_test_conn() = 0;
	
	// ִ���޷��ؽ������SQL���(insert, delete, update)
	virtual int execute_sql(const char* sql) = 0;
	
	// ִ�з���һ����¼��select���
	virtual int execute_select_sql(const char* sql, ONE_RECORD_STRUCT &one_record) = 0;
	
	// ִ�з���һ���������¼��select���
	virtual int execute_select_sql(const char* sql, vector<ONE_RECORD_STRUCT> &data_vec) = 0;

	// ִ�д洢����
	virtual void exec_store_proc(const char* sp_name, const map<char*, char*> &input_param_map) = 0;
	  
	// ���ݿ����ӵ��û���
    void get_username(char* username);
    void set_username(const char* username);
    
    // ����
    void set_password(const char* password);
    void get_password(char* password);
  
    // ���ݿ��������
    void get_server_name(char* server_name);    
    void set_server_name(const char* server_name);
  
    // ���ݿ�����
    void get_dbname(char* dbname);    
    void set_dbname(const char* dbname);
  
    // ����SQLCODE 
    int get_sqlcode();
  
    // SQL���ִ�н��
    void set_sql_result(int sqlcode, int row = 0);
  
    // ���ݿ�����״̬
	bool IsConnected()
	{
		return m_connected;
	}
   
    // ���ݿ�����
	EnumDbType get_dbtype()
	{
		return m_dbtype;
	}

    
protected:
	// ��select���ת��Ϊ������������sql���
	int get_sql(const char* oldsql, char* newsql);

	// ��ȡsql��������
	EnumDml get_sql_dml(const char* sql);

	// ��鵱ǰ������Ƿ��Ѿ�����������β(" from "��" as ")
	virtual bool is_end_field(const char* sql);

	// ���ݿ�������Ϣ
	char m_username[32]; 		// �û���
	char m_password[32]; 		// ����
	char m_server_name[32]; 	// ���ݿ������
	char m_dbname[32]; 			// ���ݿ���
	EnumDbType m_dbtype;        // ���ݿ�����
		
	// SQL���ִ����Ϣ
	int m_sqlcode; 			        // SQLCODE 
	int m_row;						// SQL ���Ӱ�������
	char m_sqlerr[1024];	 // ִ�е�SQL��������Ϣ

	bool m_connected; 	     // ���ݿ�����״̬: ���ݿ��Ƿ�����  
 
    char m_split_symbol[10];     // �ֶηָ���, ����ʹ���������ֶ������ֶ�ֵ���ַ�
	char m_union_symbol[10];     // �Ѷ���ֶν������ӵ����ӷ�

	// ���sql���Ļ�����, 2M
	char* m_sql_buff;

	// ����ֶ�ֵ�Ļ�����, 1M
	char* m_field_value;
};



#endif

