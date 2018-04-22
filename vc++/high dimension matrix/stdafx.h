// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <afxwin.h> 
#include<string>
#include<iostream>
#include"Index.h"

#define innerdim 4
#define outerdim 8

typedef struct TRSV
{
	int i;
	struct TRSV *next;
}TRS;
typedef struct Permu
{

	TRS *S;
	struct Permu* frontPerm;
	struct Permu* next;
}Perm;

typedef struct permlist
{
	int S[outerdim];
} Perpoint;
typedef struct baselist
{
	int scalar;
	int scalar2;
	int**  point;
}basl;
struct basenum
{
	char g[32];
	char h[8];
	char l[3];

};

struct handle
{
bool endoffile;
HANDLE  hFileMap;
char *lpbMapAddress;
LPVOID lpbstartaddress;
int headuselessbits;
DWORD dwBlockBytes;
__int64 qwFileOffset;
__int64 qwFileSize;
};

struct Param
{
	char *a;
	int n;
};
//Perm*& getnextpermutation(Perm* thislink);
//basenum getfilestring(FILE* fpname,int i);
//void refinefile(char *destfile,char *sourcefile);
//int bigger(basenum a,basenum b);
//int getfilelen(FILE* fpname);
//int  getpermutation();
//void getfileshandle();

// TODO: 在此处引用程序需要的其他头文件


#import "C:\Program Files\Common Files\System\ado\msado15.dll" rename("EOF","ADOEOF")

using namespace ADODB;
using namespace std;


