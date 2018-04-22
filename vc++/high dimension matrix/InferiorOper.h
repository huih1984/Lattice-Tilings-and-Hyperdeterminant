#pragma once
#include "Inferior_datum_mutiplicity.h"
#include "preimage.h"
#include<list>
#include"qsort.h"
#include<vector>
using namespace std;
class InferiorOper
{
public:
	PermOper perm;
	CPreimage preimage;
public:
	InferiorOper(void);
	virtual ~InferiorOper(void);


	Inferior_datum* get_dest_regular_data_list(Inferior_datum dest_regular);

	//_CRTIMP void __cdecl set_Inferior_datum( _In_ void (__cdecl * fun)(Inferior_datum));
	void  traverse_Inferior_datum( void ( * fun)(Inferior_datum&),  void(* addtolist)(Hashtable*, Inferior_datum_mutiplicity&));

	void oper();
	void get_filedata();
	void get_traverse_next(Inferior_datum_mutiplicity& inf_d, Inferior_datum_mutiplicity& charge_inf_d, int rownum);


	//*************************
	//检测给定一个下标矩阵，能否在所有的下标矩阵左乘变换下变成单位
	void get_one_Inferior_datum();
	void output_one_Inferior_datum_to_file();
	Inferior_datum m_randInferior_datum;
	Inferior_datum m_leftmultiple_datum;

	//辅助函数
	void setonerowval(Inferior_datum& one, short rownum);
	//************************

	//录入下标矩阵函数m_regular_matrix
	//get_min_matrix(),按列字典排序取最小矩阵
	//按字典升序取所有可能值，get_ascend_next();
	Inferior_datum m_regular_matrix;
	bool compare_row(int i);
	void get_dict_next(int n);
	void get_next_array(int n);
	void get_regular_matrix();
	void get_min_matrix();
	void get_ascend_next();
	void oper_one_regular_trans();
	short m_permus[7][24][4];
	void get_permutation_row(int n);
	void get_row_next(short l_perm_serial_num);
	int m_total;
	list<Inferior_datum*>  m_v_regular_matrix;
	void insert_to_vector();
	bool temp_store_matrix[innerdim][outerdim];
	void oper_one_origins();
	void oper_one_sort_origins();
	void oper_one_six_sum_factorial();
	void oper_one_six_fivemuti_sum_factorial();
	void oper_one_fifteen_doublemuti_sum_factor();
	void oper_one_part_to_class();
	void oper_multiply_mutations();
	Inferior_datum_mutiplicity a;
	Inferior_datum_mutiplicity b;

	void InferiorOper::get_positive_num();
public:
	/*handle f_map_handle;*/
	//void print_list(Inferior_datum& datum);
};
