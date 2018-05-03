#pragma once
#include "perm_datum.h"
class matrix_Param
{
public:
	short** a;
	short n;
};


class Inferior_datum
{
public:
	Inferior_datum();
	~Inferior_datum();
	short get_sign();
	//���������ź͹��ɵ�ϵ��
	short get_six_sum_factor();
	short get_six_fivemuti_sum_factor();
	short get_fifteen_doublemuti_sum_factor();
	short get_not_cal_last_row_sign();
	short get_not_cal_all_row_sign();
	void get_regular_datum(Inferior_datum& reg_datum);
	void conv_to_regular();
	void this_qsort(short**a,short n);
	void qsort_column();
	void swap(short** para1, short** para2);
	short compare(const Inferior_datum* datum2);
	void Trans(Perm_datum& data);
	//����һ��ȫ���У�����任���
	void OnePermTrans(short arraynums[outerdim]);
	void LeftMultiple(Inferior_datum & leftval);
	void printme(FILE* f);
	void printme();
	void printme(char* pathname);
	int  getinversesign(short arraynums[], short size);
public:
	short matrix[innerdim][outerdim];
	short sign;

	//���һ�еķ��Ų��������ڵľ������
	short sign2;
	//�������κ��з���
	short sign_not_cal_all;
};

int comp(void*a, void*b, size_t& element_interval_size, size_t& element_num);

