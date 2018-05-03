// high dimension matrix.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include"filemanage.h"
#include "Chdoperator.h"
#include"Perm_datum.h"
#include "InferiorOper.h"

#define random(x) (rand()%x);
using namespace std;
Chdoperator *Oper;
int faco;
Perpoint* permlist;
#define SUBFUN
#ifndef SUBFUN
//
//int main()
//{
//	//Inferior_datum obj;
//	//short arrays[] = {1,2,8,3};//{3,5,7,6,4,2,8,1};//{8,6,5,3,1,4,2,7};//{2,4,8,7,6,1,5,3};
//	//short sgn;
//	//sgn = obj.getinversesign(arrays, 4);
//	//printf("%d", sgn);
//	//system("pause");
//	/*Inferior_datum lv_datum;
//	short temp[4][8] = {{1,2,3,4,5,6,7,8},{2,1,4,3,6,5,8,7},{3,4,8,2,7,1,5,6},{4,3,2,1,8,7,6,5}};
//	memcpy(lv_datum.matrix, temp, sizeof(temp));
//	int  i = lv_datum.get_sign();*/
//	//InferiorOper Inf_oper;
//	//Inf_oper.oper_one_regular_trans();
//	//Inf_oper.oper_one_origins();
//	//Inf_oper.oper_one_sort_origins();
//	//Inf_oper.oper_one_six_sum_factorial();
//	//Inf_oper.oper_one_six_fivemuti_sum_factorial();
//	//Inf_oper.oper_one_fifteen_doublemuti_sum_factor();
//	//Inf_oper.oper_multiply_mutations();
//	//Inf_oper.get_positive_num();
//	//Inf_oper.oper_one_part_to_class();
//	/*Inf_oper.oper();
//	try{
//	for(int i = 0; i < Inferior_datum_mutiplicity::hashtblsize; ++i)
//	{
//		if(Inferior_datum_mutiplicity::tbl[i].address != NULL)
//		{
//			delete Inferior_datum_mutiplicity::tbl[i].address;
//			Inferior_datum_mutiplicity::tbl[i].address = NULL;
//		}
//	}
//	delete []Inferior_datum_mutiplicity::tbl;
//	}
//	catch(exception e)
//	{
//		printf(e.what());
//	}*/
//	return 0;
//}
#endif