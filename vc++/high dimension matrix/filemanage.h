#include"stdafx.h"
#include <sys/stat.h>
#include <io.h>
#include <FCNTL.H>
#include<winbase.h>
#include<string>
#include<iostream>
#include <stdio.h> 
#include<stdlib.h>
int bigger(basenum a,basenum b);
void compareFiles(char* filename[],int length);
bool findposition(basenum*destp,int starti,int filelen,basenum sourcebase,int & placei);
__int64 getallfilesize(handle han[]);
int getfilelen(FILE* fpname);
HANDLE getfilemaphandle(LPCWSTR strfile, DWORD &dwBlockBytes,__int64 &qwFileSize, char *&lpbMapAddress);
HANDLE getfilemaphandle(HANDLE &hFile,LPCWSTR strfile, DWORD &dwBlockBytes,__int64 &qwFileSize, char *&lpbMapAddress);
void getfileshandle();
basenum getfilestring(FILE* fpname,int i);
bool getminchar(handle han[],int minfilesindex[],int minfilesnum ,basenum& val);
void refinefile(char *destfile,char *sourcefile);
int smaller(char*lpmapaddress1,char*lpmapaddress2);
void writecharinnewfile(char*& MapAddress,basenum newval);
//bool writefileonmapaddresses(handle han[],HANDLE newfilehandle,char *newfileaddress);
HANDLE getnewfilemaphandle( HANDLE &hFile,  LPCWSTR strfile, char *&lpbMapAddress,__int64 qwFileSize);
int sgn(int* M,int m);
bool  getpfa4value(char* lpaddress,int* value);
void partfile();
void evaluepfa();
void getdetvalue();