#include "StdAfx.h"
#include "InferiorFileRW.h"

CInferiorFileRW::CInferiorFileRW(void)
{
}

CInferiorFileRW::~CInferiorFileRW(void)
{
}


void CInferiorFileRW::OpenFile(string filename)
{
	f.open(filename.c_str());
}

bool CInferiorFileRW::ReadLine(Inferior_datum& out)
{
	char onelinedata[DATANUM];
	f.getline(onelinedata, DATANUM);
	if( f.eof())
	{
		return false;
	}
	for(int i = 0; i < outerdim; ++i)
	{
		for(int j = 0; j < innerdim; ++j)
		{
			out.matrix[j][i] = onelinedata[j*outerdim + i] - '0';
		}
	}
	int index = 0;
	int totalindex = innerdim*outerdim;
	while(onelinedata[totalindex + index] == ' ')
	{
		++index;
	}
	totalindex += index;
	index = 0;
	while(onelinedata[totalindex + index] != ' ')
	{
		++index;
	}
	char temp_num[6] = {0};
	strncpy(temp_num, onelinedata + totalindex, index);
	out.sign = atoi(temp_num);

	totalindex += index + 1;
	out.sign2 = atoi(onelinedata + totalindex);
	return true;
}


void CInferiorFileRW::CloseFile()
{
	f.close();
}