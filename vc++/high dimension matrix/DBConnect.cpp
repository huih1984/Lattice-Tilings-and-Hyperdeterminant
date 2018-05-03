#include "StdAfx.h"
#include "DBConnect.h"


//////////////////////////////////////DBConnect/////////////////////////
void dump_com_error(_com_error &e)
{
	printf("Error\n");
	printf("\a\tCode = %08lx\n", e.Error());
	printf("\a\tCode meaning = %s", e.ErrorMessage());
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	printf("\a\tSource = %s\n", (LPCSTR) bstrSource);
	printf("\a\tDescription = %s\n", (LPCSTR) bstrDescription);
}


DBConnect::DBConnect(string strDstAddress,string strUsername, string strPassword,string strDBName,BOOL &state)
{
	::CoInitialize(NULL);
	m_bDBOpen = false;
	if (0 == Open(strDstAddress,strUsername,strPassword,strDBName))
	{
		m_pCommand.CreateInstance("ADODB.Command");
		m_pRecordset.CreateInstance("ADODB.Recordset");
		m_pCommand->ActiveConnection=m_pConnection;///�ǳ��ؼ���һ�䣬�����������Ӹ�ֵ�� 
		state =true;
	}
	else
	{
		state = false;
	}
}
DBConnect::~DBConnect()
{
	Close();
}
int DBConnect::Open(string strDstAddress, string strUsername, string strPassword, string strDBName)
{
	_strDstAddress = strDstAddress;
	_strDBName       = strDBName;
	_strUsername     = strUsername;
	_strPassword     = strPassword;

	//HRESULT comhr = ::CoInitialize(NULL);//˵�����������������������EPIC��DLL������ĵ�ַ��ͻ����û�����DLL������Ҫ����䡣
	//if (FAILED(comhr))
	//{
	//return -1;
	//}
	HRESULT hr = m_pConnection.CreateInstance(__uuidof(Connection));

	if (FAILED(hr))
	{
		return -1;
	}
	string szSQL="Driver=SQL Server;Server="+strDstAddress+";"+"DATABASE="+strDBName+"";
	try
	{
		// ���ӵ������������ݿ�
		m_pConnection->Open(szSQL.c_str(), strUsername.c_str(), strPassword.c_str(),adModeUnknown);
		if (FAILED(hr))
			return -1;
	}
	catch (_com_error &err)
	{
		dump_com_error(err);
		return -1;
	}
	m_bDBOpen = TRUE;
	return 0;
}


int DBConnect::Close()
{ 
	if (m_bDBOpen)
	{
		HRESULT hr =m_pConnection->Close();

		if (FAILED(hr))
		{
			return -1;
		}
		//::CoUninitialize(); //˵����������������������EPIC��DLL������ĵ�ַ��ͻ����û�����DLL������Ҫ����䡣
		m_bDBOpen = FALSE;
	}
	CoUninitialize();
	return 0;
}


void DBConnect::setparameter(char *paraname, DataTypeEnum valuetype, _variant_t value, ADODB::ParameterDirectionEnum pde, int len)
{
		ADODB::_ParameterPtr m_pParam;//�������ز���
		m_pParam.CreateInstance(__uuidof(Parameter));
		//�������ݾݱ�������
		m_pParam=m_pCommand->CreateParameter(paraname,valuetype,pde,len);
		m_pParam->Value=value;
		m_pCommand->Parameters->Append(m_pParam);

}