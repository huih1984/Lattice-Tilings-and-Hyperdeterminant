#ifndef CHDOPER_H
#define CHDOPER_H
#pragma once

#include "dbconnect.h"
#include <vector>
struct RCD
{
	int v[25];
};
class Chdoperator :
	public DBConnect
{
public:
	Chdoperator(string strDstAddress,string strUsername, string strPassword,string strDBName,BOOL &state);
	~Chdoperator(void);
private:

public:
	void InsertData2base(Permu *point, char * tablename, int& index);
	void GetbasefirstRcd(int value[24]);
	void outputnewtable(int value[24], int& tableindex, int& recordcount);
	void getnewrecord(int sourcevalue[24],int value[24], int* theta);
	void get_all_table();
	void get_rcd(char* tablename);
	void sort_matrix_6_4_table();
	void sort_rcd(int source[24],int dest[24],int& sgn);
	int comp_sgn(int v[6]);
	void exec_s(int para[24],int sgn,int tableindex);
	void merge_sorted_value();
	void merge_vector(vector<RCD> prcdarr,int i);
	void exec_s_m(int para[25],int tableindex);
};
#endif
