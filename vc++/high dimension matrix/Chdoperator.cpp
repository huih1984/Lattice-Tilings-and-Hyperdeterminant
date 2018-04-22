#include "StdAfx.h"
#include "Chdoperator.h"
#include <vector>
extern int faco;
extern Perpoint* permlist;
Chdoperator::Chdoperator(string strDstAddress,string strUsername, string strPassword,string strDBName,BOOL &state)
:DBConnect(strDstAddress,strUsername,strPassword,strDBName,state)
{

}

Chdoperator::~Chdoperator(void)
{
}

void Chdoperator::InsertData2base(Permu *point, char * tablename, int& index)
{
	char Int[20];
	string storepname,storeprastr;
	m_pCommand.CreateInstance("ADODB.Command");
	m_pCommand->ActiveConnection=m_pConnection;
	m_pCommand->CommandText=_bstr_t("addrecord");///命令字串 

	storeprastr = "matrix_6_4_base";
	setparameter("tablename",adVarChar,storeprastr.c_str(),adParamInput,20);

	TRS *ps = NULL;
	Permu *ppermu = point;
	for(int i =0; i < innerdim; i++)
	{
		ps = ppermu->S;
		for(int j = 0; j < outerdim; j++)
		{
			storepname= "r";
			storepname += itoa(i*6+j+1,Int,10);
			storeprastr =itoa(ps->i,Int,10);
			setparameter((char*)storepname.c_str(),adInteger,storeprastr.c_str(),adParamInput,4);
			ps=ps->next;
		}
		ppermu =ppermu->next;
	}
	try
	{
		//m_pCommand-> Parameters-> Refresh();
		//int nuu=m_pCommand ->GetParameters()->Count;
		m_pCommand->Execute(NULL,NULL,adCmdStoredProc); 
	}
	catch (_com_error &e)
	{
		dump_com_error(e);
	} 

	index++;
	//m_pCommand->CommandText=_bstr_t("select count(*) as nums from matrix_6_4_base");
	//m_pRecordset=m_pCommand->Execute(&vNULL,&vNULL,adCmdText); 
	//m_pCommand->CommandText=_bstr_t("insert into recordnum values('matrix_6_4_base')");
	//m_pRecordset=m_pCommand->Execute(&vNULL,&vNULL,adCmdText); 
}


void Chdoperator::GetbasefirstRcd(int value[24])
{
	_variant_t vNULL; 
	vNULL.vt=VT_ERROR; 

	vNULL.scode=DISP_E_PARAMNOTFOUND;///定义为无参数 
	m_pCommand->CommandText=_bstr_t("select top 1 * from matrix_6_4_base");

	try
	{
		m_pRecordset = NULL;
		m_pRecordset=m_pCommand->Execute(&vNULL,&vNULL,adCmdText); 
	}
	catch (_com_error &e)
	{
		dump_com_error(e);
	}
	//_variant_t point_0_0, point_0_1, point_0_2, point_0_3, point_0_4, point_0_5, point_1_0, point_1_1, 
	//	point_1_2, point_1_3, point_1_4, point_1_5,point_2_0, point_2_1, point_2_2, point_2_3, 
	//	point_2_4, point_2_5, point_3_0, point_3_1, point_3_2, point_3_3, point_3_4, point_3_5; 
	try{
		value[0] =m_pRecordset->GetCollect("point_0_0");
		value[1]= m_pRecordset->GetCollect("point_0_1");
		value[2]= m_pRecordset->GetCollect("point_0_2");
		value[3]= m_pRecordset->GetCollect("point_0_3");
		value[4]= m_pRecordset->GetCollect("point_0_4");
		value[5]= m_pRecordset->GetCollect("point_0_5");
		value[6]= m_pRecordset->GetCollect("point_1_0");
		value[7]= m_pRecordset->GetCollect("point_1_1");
		value[8]= m_pRecordset->GetCollect("point_1_2");
		value[9]= m_pRecordset->GetCollect("point_1_3");
		value[10]= m_pRecordset->GetCollect("point_1_4");
		value[11]= m_pRecordset->GetCollect("point_1_5");
		value[12]= m_pRecordset->GetCollect("point_2_0");
		value[13]= m_pRecordset->GetCollect("point_2_1");
		value[14]= m_pRecordset->GetCollect("point_2_2");
		value[15]= m_pRecordset->GetCollect("point_2_3");
		value[16]= m_pRecordset->GetCollect("point_2_4");
		value[17]= m_pRecordset->GetCollect("point_2_5");
		value[18]= m_pRecordset->GetCollect("point_3_0");
		value[19]= m_pRecordset->GetCollect("point_3_1");
		value[20]= m_pRecordset->GetCollect("point_3_2");
		value[21]= m_pRecordset->GetCollect("point_3_3");
		value[22]= m_pRecordset->GetCollect("point_3_4");
		value[23]= m_pRecordset->GetCollect("point_3_5");
	}
	catch (_com_error &e)
	{
		dump_com_error(e);
	} 
	/*point_0_0= m_pRecordset->GetCollect("point_0.0");
	point_0_1= m_pRecordset->GetCollect("point_0.1");
	point_0_2= m_pRecordset->GetCollect("point_0.2");
	point_0_3= m_pRecordset->GetCollect("point_0.3");
	point_0_4= m_pRecordset->GetCollect("point_0.4");
	point_0_5= m_pRecordset->GetCollect("point_0.5");
	point_1_0= m_pRecordset->GetCollect("point_1.0");
	point_1_1= m_pRecordset->GetCollect("point_1.1");
	point_1_2= m_pRecordset->GetCollect("point_1.2");
	point_1_3= m_pRecordset->GetCollect("point_1.3");
	point_1_4= m_pRecordset->GetCollect("point_1.4");
	point_1_5= m_pRecordset->GetCollect("point_1.5");
	point_2_0= m_pRecordset->GetCollect("point_2.0");
	point_2_1= m_pRecordset->GetCollect("point_2.1");
	point_2_2= m_pRecordset->GetCollect("point_2.2");
	point_2_3= m_pRecordset->GetCollect("point_2.3");
	point_2_4= m_pRecordset->GetCollect("point_2.4");
	point_2_5= m_pRecordset->GetCollect("point_2.5");
	point_3_0= m_pRecordset->GetCollect("point_3.0");
	point_3_1= m_pRecordset->GetCollect("point_3.1");
	point_3_2= m_pRecordset->GetCollect("point_3.2");
	point_3_3= m_pRecordset->GetCollect("point_3.3");
	point_3_4= m_pRecordset->GetCollect("point_3.4");
	point_3_5= m_pRecordset->GetCollect("point_3.5");*/
	m_pRecordset->Close();
}


void Chdoperator::getnewrecord(int sourcevalue[24],int value[24], int* theta)
{
	for (int i=0;i<24;i++)
	{
		value[i]=sourcevalue[i];
	}
	for (int i=0;i<24;i++)
	{
		value[i]=theta[value[i]];
	}
	for (int i=0;i<6;i++)
	{
		for(int j=i+1;j<6;j++)
		{
			if(value[i]>value[j])
			{
				value[i] = value[i]^value[j];
				value[j] = value[i]^value[j];
				value[i] = value[i]^value[j];

				value[i+6] = value[i+6]^value[j+6];
				value[j+6] = value[i+6]^value[j+6];
				value[i+6] = value[i+6]^value[j+6];

				value[i+12] = value[i+12]^value[j+12];
				value[j+12] = value[i+12]^value[j+12];
				value[i+12] = value[i+12]^value[j+12];

				value[i+18] = value[i+18]^value[j+18];
				value[j+18] = value[i+18]^value[j+18];
				value[i+18] = value[i+18]^value[j+18];
			}
		}
	}
}


void Chdoperator::outputnewtable(int value[24], int& tableindex, int& recordcount)
{
	int newval[24]={0};
	string storeprastr;
	string storepname;
	char Int[20];
	string instr = itoa(tableindex,Int,10);
	recordcount =0;
	for (int i=0;i<faco;i++)
	{
		getnewrecord(value,newval,permlist[i].S);

		m_pCommand.CreateInstance("ADODB.Command");
		m_pCommand->ActiveConnection=m_pConnection;
		m_pCommand->CommandText=_bstr_t("addrecord");///命令字串 

		storeprastr = string("matrix_6_4_")+itoa(tableindex,Int,10);
		setparameter("tablename",adVarChar,storeprastr.c_str(),adParamInput,20);

		for (int j=0;j<24;j++)
		{
			storepname= "r";
			storepname += itoa(j+1,Int,10);
			storeprastr =itoa(newval[j],Int,10);
			setparameter((char*)storepname.c_str(),adInteger,storeprastr.c_str(),adParamInput,4);
		}
		try
		{
			//m_pCommand-> Parameters-> Refresh();
			//int nuu=m_pCommand ->GetParameters()->Count;
			m_pCommand->Execute(NULL,NULL,adCmdStoredProc); 
		}
		catch (_com_error &e)
		{
			dump_com_error(e);
		} 
	}
	string storeprocstr;
	//删除新建的表记录中重复的
	storeprocstr = "Select distinct * into #Tmp from matrix_6_4_";
	storeprocstr += instr;
	storeprocstr += " Drop table matrix_6_4_";
	storeprocstr += instr;
	storeprocstr += " Select * into matrix_6_4_";
	storeprocstr += instr;
	storeprocstr += " from #Tmp";
	storeprocstr += " Drop table #Tmp";
	m_pCommand.CreateInstance("ADODB.Command");
	m_pCommand->ActiveConnection=m_pConnection;
	m_pCommand->CommandText=_bstr_t(storeprocstr.c_str());///命令字串 
	try
	{
		m_pRecordset=m_pCommand->Execute(NULL,NULL,adCmdText);
	}
	catch (_com_error &e)
	{
		dump_com_error(e);
	} 
	storeprocstr = "select count(*) AS   Nums from ";
	storeprocstr += string("matrix_6_4_")+instr;
	m_pCommand.CreateInstance("ADODB.Command");
	m_pCommand->ActiveConnection=m_pConnection;
	m_pCommand->CommandText=_bstr_t(storeprocstr.c_str());///命令字串 
	try
	{
		//m_pRecordset.CreateInstance( "ADODB.RecordSet ");
		m_pRecordset=m_pCommand->Execute(NULL,NULL,adCmdText); 
		recordcount =m_pRecordset->GetCollect("Nums");
		m_pRecordset->Close();
	}
	catch (_com_error &e)
	{
		dump_com_error(e);
	} 
	//在表recordnum中添加这个表的记录
	storeprocstr = "insert into recordnum values(";
	storeprocstr += "'matrix_6_4_";
	storeprocstr += instr;
	storeprocstr += "', ";
	storeprocstr += string(itoa(recordcount,Int,10))+")";
	m_pCommand.CreateInstance("ADODB.Command");
	m_pCommand->ActiveConnection=m_pConnection;
	m_pCommand->CommandText=_bstr_t(storeprocstr.c_str());///命令字串 
	try
	{
		m_pCommand->Execute(NULL,NULL,adCmdText); 
	}
	catch (_com_error &e)
	{
		dump_com_error(e);
	} 
	tableindex++;
}
void Chdoperator::get_all_table()
{
	_variant_t vNULL; 
	vNULL.vt=VT_ERROR; 
	char Int[20];
	int recordcount;
	int base_rcd_num = 393120;
	int value[24];
	string storeprocstr;
	base_rcd_num = GetPrivateProfileInt(L"base_table",L"rcdnum",0,L"d:\\new_table.ini");
	while (base_rcd_num > 0)
	{
		base_rcd_num = GetPrivateProfileInt(L"base_table",L"rcdnum",0,L"d:\\new_table.ini");
		int newindex = GetPrivateProfileInt(L"table",L"newindex",0,L"d:\\new_table.ini");
		GetbasefirstRcd(value);
		//创建新表
		//storeprocstr = "CrMatrixT '";
		//storeprocstr += itoa(newindex,Int,10);
		//storeprocstr += "'";
		m_pCommand.CreateInstance("ADODB.Command");
		m_pCommand->ActiveConnection=m_pConnection;
		m_pCommand->CommandText=_bstr_t("CrMatrixT");///命令字串 

		setparameter("TabNameindex",adVarChar,itoa(newindex,Int,10),adParamInput,20);
		try
		{
			m_pCommand->Execute(NULL,NULL,adCmdStoredProc); 
			//for(int   n=0;n <m_pCommand-> GetParameters()-> Count;   n++) 
			//{ 
			//	m_pCommand-> GetParameters()-> Delete(_variant_t(n)); 
			//}
		}
		catch (_com_error &e)
		{
			dump_com_error(e);
		} 
		//向新表中添加记录
		outputnewtable(value, newindex, recordcount);

		//从总表中删除新表

		storeprocstr ="matrix_6_4_";
		storeprocstr += itoa(newindex-1,Int,10);
		m_pCommand.CreateInstance("ADODB.Command");
		m_pCommand->ActiveConnection=m_pConnection;
		m_pCommand->CommandText=_bstr_t("deletercord");///命令字串 
		setparameter("sourcetablename ",adVarChar,"matrix_6_4_base",adParamInput,20);
		setparameter("desttablename",adVarChar,storeprocstr.c_str(),adParamInput,20);
		try
		{
			m_pCommand->Execute(NULL,NULL,adCmdStoredProc); 
		}
		catch (_com_error &e)
		{
			dump_com_error(e);
		} 
		//写新表序号
		//MultiByteToWideChar(
		USES_CONVERSION;
		WritePrivateProfileString(L"table",L"newindex",A2W(itoa(newindex,Int,10)),L"d:\\new_table.ini");
		//写总表记录个数
		base_rcd_num = base_rcd_num - recordcount;
		WritePrivateProfileString(L"base_table",L"rcdnum",A2W(itoa(base_rcd_num,Int,10)),L"d:\\new_table.ini");
	}
}


void Chdoperator::sort_matrix_6_4_table()
{
	int rcdnum = GetPrivateProfileInt(L"table",L"newindex",0,L"d:\\new_table.ini");
	char tblname[16];
	char index_str[6];
	_RecordPtr prcd;
	VARIANT* collect=0;
	_variant_t vNULL; 
	vNULL.vt=VT_ERROR; 

	vNULL.scode=DISP_E_PARAMNOTFOUND;///定义为无参数 
	int rcd[24]={0};
	int newrcd[24]={0};
	char point_str[16]={"point_"};
	int sgn=1;
	for (int i=296;i<rcdnum;i++)
	{
		strncpy(tblname,"matrix_6_4_",sizeof(tblname));
		strncat(tblname,itoa(i,index_str,10),sizeof(tblname));
		get_rcd(tblname);
		m_pCommand.CreateInstance("ADODB.Command");
		m_pCommand->ActiveConnection=m_pConnection;
		m_pCommand->CommandText =_bstr_t("cr_sorttable");
		string newtblname=tblname;
		newtblname+="_s";
		setparameter("tablename",adVarChar,newtblname.c_str(),adParamInput,20);
		try
		{
			m_pCommand->Execute(NULL,NULL,adCmdStoredProc); 
		}
		catch (_com_error &e)
		{
			dump_com_error(e);
		} 
		while(!m_pRecordset->ADOEOF)
		{
			for(int jj=0;jj<4;jj++)
			{
				for (int j=0;j<6;j++)
				{
					strncat(point_str,itoa(jj,index_str,10),sizeof(point_str));
					strncat(point_str,"_",sizeof(point_str));
					strncat(point_str,itoa(j,index_str,10),sizeof(point_str));
					rcd[jj*6+j] = m_pRecordset->GetCollect(point_str);
					memset(point_str,0,sizeof(point_str));
					strncpy(point_str,"point_",sizeof(point_str));
				}
			}
			sort_rcd(rcd,newrcd,sgn);
			exec_s(newrcd,sgn,i);
			m_pRecordset->MoveNext();
		}
	}
}


void Chdoperator::get_rcd(char* tablename)
{
	char cmdtxt[64];
	strncpy(cmdtxt, "select * from ",sizeof(cmdtxt));
	strncat(cmdtxt, tablename, sizeof(cmdtxt));
	m_pCommand->CommandText =_bstr_t(cmdtxt);
	try
	{
		m_pRecordset = m_pCommand->Execute(NULL,NULL,adCmdText);
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
}


void Chdoperator::sort_rcd(int source[24],int dest[24],int& sgn)
{
	sgn=1;
	for(int i=0;i<4;i++)
	{
		sgn*=comp_sgn(&source[i*6]);
	}

	for(int i=0;i<24;i++)
	{
		dest[i]=source[i];
	}
	//竖向排序
	for(int i=0;i<6;i++)
	{
		for(int j=0;j<4;j++)
		{
			for(int jj=j+1;jj<4;jj++)
			{
				if(dest[jj*6+i]<dest[j*6+i])
				{
					dest[jj*6+i] = dest[jj*6+i]^dest[j*6+i];
					dest[j*6+i] = dest[jj*6+i]^dest[j*6+i];
					dest[jj*6+i] = dest[jj*6+i]^dest[j*6+i];
					sgn*=-1;
				}
			}
		}
	}
	//横向排序
	int minindex=0;
	for(int j=0;j<6;j++)
	{
		minindex=j;
		for(int jj=j+1;jj<6;jj++)
		{
			if(dest[jj]<dest[minindex])
			{
				minindex=jj;
			}
		}
		if(minindex!=j)
		{
			for(int i=0;i<4;i++)
			{
				dest[i*6+j]=dest[i*6+minindex]^dest[i*6+j];
				dest[i*6+minindex]=dest[i*6+minindex]^dest[i*6+j];
				dest[i*6+j]=dest[i*6+minindex]^dest[i*6+j];
			}
		}
	}
}

int Chdoperator::comp_sgn(int v[6])
{
	int sgn=1;
	for(int i=0;i<6;i++)
	{
		for(int j=i+1;j<6;j++)
		{
			if(v[j]<v[i])
			{
				sgn*=-1;
			}
		}
	}
	return sgn;
}

void exec_storeproc(char * storeproc_name, ...)//写一个变参数类型的函数，类似printf
{
}


//插入记录
void Chdoperator::exec_s(int para[24],int sgn,int tableindex)
{
	char Int[10];
	string storeprastr;
	string storepname;
	m_pCommand.CreateInstance("ADODB.Command");
	m_pCommand->ActiveConnection=m_pConnection;
	m_pCommand->CommandText=_bstr_t("insert_sorttable");///命令字串 

	storeprastr = string("matrix_6_4_")+itoa(tableindex,Int,10);
	storeprastr +="_s";
	setparameter("tablename",adVarChar,storeprastr.c_str(),adParamInput,20);

	for (int j=0;j<24;j++)
	{
		storepname= "r";
		storepname += itoa(j+1,Int,10);
		storeprastr =itoa(para[j],Int,10);
		setparameter((char*)storepname.c_str(),adInteger,storeprastr.c_str(),adParamInput,4);
	}
	storepname= "r";
	storepname += itoa(25,Int,10);
	storeprastr =itoa(sgn,Int,10);
	setparameter((char*)storepname.c_str(),adInteger,storeprastr.c_str(),adParamInput,4);
	try
	{
		m_pCommand->Execute(NULL,NULL,adCmdStoredProc); 
	}
	catch (_com_error &e)
	{
		dump_com_error(e);
	} 
}


void Chdoperator::merge_sorted_value()
{
int rcdnum = GetPrivateProfileInt(L"table",L"newindex",0,L"d:\\new_table.ini");
	char tblname[16];
	char index_str[6];
	_RecordPtr prcd;
	vector<RCD> prcdarr;

	char point_str[16]={"point_"};
	int sgn=1;
	for (int i=0;i<rcdnum;i++)
	{
		strncpy(tblname,"matrix_6_4_",sizeof(tblname));
		strncat(tblname,itoa(i,index_str,10),sizeof(tblname));
		strncat(tblname,"_s",sizeof(tblname));
		get_rcd(tblname);
		m_pCommand.CreateInstance("ADODB.Command");
		m_pCommand->ActiveConnection=m_pConnection;
		m_pCommand->CommandText =_bstr_t("cr_sorttable");
		string newtblname=tblname;
		newtblname+="_m";
		setparameter("tablename",adVarChar,newtblname.c_str(),adParamInput,20);
		try
		{
			m_pCommand->Execute(NULL,NULL,adCmdStoredProc); 
		}
		catch (_com_error &e)
		{
			dump_com_error(e);
		} 
		int rcdcount = m_pRecordset->GetRecordCount();
		int cursor = 0;

		while(!m_pRecordset->ADOEOF)
		{
			RCD rcd;
			for(int jj=0;jj<4;jj++)
			{
				for (int j=0;j<6;j++)
				{
					strncat(point_str,itoa(jj,index_str,10),sizeof(point_str));
					strncat(point_str,"_",sizeof(point_str));
					strncat(point_str,itoa(j,index_str,10),sizeof(point_str));
					rcd.v[jj*6+j] = m_pRecordset->GetCollect(point_str);
					memset(point_str,0,sizeof(point_str));
					strncpy(point_str,"point_",sizeof(point_str));
				}
			}
			rcd.v[24] = m_pRecordset->GetCollect("sgn");
			prcdarr.push_back(rcd);
			cursor++;
			m_pRecordset->MoveNext();
		}
		merge_vector(prcdarr,i);
		prcdarr.clear();
	}
}


void Chdoperator::merge_vector(vector<RCD> prcdarr,int index)
{
	RCD curnode,pnode;
	int i = 0; 
	vector<RCD>::iterator it;
	//for(it = prcdarr.begin(); it<prcdarr.end();it++)
	//{
	//}
	while(1)
	{
		int vsi = prcdarr.size();
		for (int j= i+1; j < vsi; j++)
		{
			int k;
			for(k =0;k<24;k++)
			{
				if(prcdarr.at(j).v[k] != prcdarr.at(i).v[k])
					break;
			}
			if (k == 24)
			{
				prcdarr.at(i).v[24] += prcdarr.at(j).v[24];
				it =prcdarr.begin()+j;
				prcdarr.erase(it);
				j = j -1;vsi = prcdarr.size();
			}
		}
		i++;
		if(i >= prcdarr.size())break;
	}
	for (i =0;i<prcdarr.size();i++)
	{
		exec_s_m(prcdarr.at(i).v,index);
	}

}


	//插入记录
void Chdoperator::exec_s_m(int para[25],int tableindex)
{
	char Int[10];
	string storeprastr;
	string storepname;
	m_pCommand.CreateInstance("ADODB.Command");
	m_pCommand->ActiveConnection=m_pConnection;
	m_pCommand->CommandText=_bstr_t("insert_sorttable");///命令字串 

	storeprastr = string("matrix_6_4_")+itoa(tableindex,Int,10);
	storeprastr +="_s_m";
	setparameter("tablename",adVarChar,storeprastr.c_str(),adParamInput,20);

	for (int j=0;j<25;j++)
	{
		storepname= "r";
		storepname += itoa(j+1,Int,10);
		storeprastr =itoa(para[j],Int,10);
		setparameter((char*)storepname.c_str(),adInteger,storeprastr.c_str(),adParamInput,4);
	}
	try
	{
		m_pCommand->Execute(NULL,NULL,adCmdStoredProc); 
	}
	catch (_com_error &e)
	{
		dump_com_error(e);
	} 
}
