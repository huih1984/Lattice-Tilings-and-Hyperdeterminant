#pragma once
#include<fstream>
#include"Inferior_datum.h"
#define DATANUM 100
class CInferiorFileRW
{
public:
	CInferiorFileRW(void);
	~CInferiorFileRW(void);
	void OpenFile(string filename);
	bool ReadLine(Inferior_datum& out);
	void CloseFile();
	fstream f;
};
