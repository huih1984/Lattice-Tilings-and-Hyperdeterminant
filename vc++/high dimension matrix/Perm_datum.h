#pragma once

class Perm_datum
{
public:
	Perm_datum(void);
	~Perm_datum(void);

public:
	short datum[outerdim];
};


class PermOper
{
public:
	Perm_datum* permlist;
	unsigned int fac;

public:
	PermOper();
	~PermOper();
	unsigned int factorial();
	void get_permutation();
	int det();
	int get_sign(Perm_datum permdata);
	void getOneItem(int& temp, int outIndex, Perm_datum s1, Perm_datum& s2, Perm_datum& s3, Perm_datum& s4, Perm_datum& s5, int mat[outerdim][outerdim][outerdim][outerdim][outerdim][outerdim], long long& det, int& sss, FILE* f);
};