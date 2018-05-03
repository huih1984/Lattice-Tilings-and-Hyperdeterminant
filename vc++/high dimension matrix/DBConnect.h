#pragma once

class DBConnect
{
public:
	DBConnect(string strDstAddress,string strUsername, string strPassword,	string strDBName,BOOL &state);
	~DBConnect();
public:
	// ���ӵ����ݿ�

	int Open(string strDstAddress, string strUsername,string strPassword, string strDBName);
	// �ر����ݿ�

	int Close();
	// ���ݿ��Ƿ�������

	BOOL IsOpen() const;
protected:
	ADODB::_ConnectionPtr           m_pConnection;   //ADO�����ݿ���������ָ��
	_CommandPtr m_pCommand; 
	_RecordsetPtr m_pRecordset;

	bool                            _isAdoValid;       //ADO�����Ƿ��Ѿ������ɹ���־��
	bool							m_bDBOpen;

	string							_strDstAddress;    //����Դ��ַ�������
	string							_strUsername;      //���ݿ��û���
	string							_strPassword;      //���ݿ�����
	string							_strDBName;        //���ݿ�����
	//���⹫���ӿ�
public:
	void setparameter(char *paraname, DataTypeEnum valuetype, _variant_t value, ADODB::ParameterDirectionEnum pde, int len);
};	

void dump_com_error(_com_error &e);