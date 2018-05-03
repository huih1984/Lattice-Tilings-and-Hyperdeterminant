#pragma once

class DBConnect
{
public:
	DBConnect(string strDstAddress,string strUsername, string strPassword,	string strDBName,BOOL &state);
	~DBConnect();
public:
	// 连接到数据库

	int Open(string strDstAddress, string strUsername,string strPassword, string strDBName);
	// 关闭数据库

	int Close();
	// 数据库是否已连接

	BOOL IsOpen() const;
protected:
	ADODB::_ConnectionPtr           m_pConnection;   //ADO的数据库连接智能指针
	_CommandPtr m_pCommand; 
	_RecordsetPtr m_pRecordset;

	bool                            _isAdoValid;       //ADO环境是否已经初化成功标志量
	bool							m_bDBOpen;

	string							_strDstAddress;    //数据源地址或服务名
	string							_strUsername;      //数据库用户名
	string							_strPassword;      //数据库密码
	string							_strDBName;        //数据库名称
	//对外公共接口
public:
	void setparameter(char *paraname, DataTypeEnum valuetype, _variant_t value, ADODB::ParameterDirectionEnum pde, int len);
};	

void dump_com_error(_com_error &e);