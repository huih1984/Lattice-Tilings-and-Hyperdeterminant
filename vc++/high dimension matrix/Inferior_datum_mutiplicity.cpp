#include "StdAfx.h"
#include "Inferior_datum_mutiplicity.h"
#include<math.h>
unsigned int Inferior_datum_mutiplicity::hashtblsize = 0;
Hashtable* Inferior_datum_mutiplicity::tbl = NULL;
extern FILE* f5;
Inferior_datum_mutiplicity::Inferior_datum_mutiplicity(void)
{
	mutiplicity = 0;
	genius_mutiplicity = 0;
}

Inferior_datum_mutiplicity::~Inferior_datum_mutiplicity(void)
{
}

int GETM(const int& n) 
{
	int s = 1;
	for(int i = 0; i < n; ++i)
	{
		s *= 10;
	}
	return s;
}

void Inferior_datum_mutiplicity::set_hashkey()
{
	int hashkeystep = 0;
	hashkey = 0;
	for(int i = 0; i <innerdim; ++i)
	{
		for(int j = 0; j < outerdim; ++j)
		{
			hashkey += matrix[i][j]*pow(3.0,j); 
			//if(j == outerdim - 1)
			//	hashkeystep += matrix[i][j]*GETM(j + 1);
		}
	}
	
	hashkey = hashkey/10;
	hashkey = hashkey%hashtblsize;
	while(tbl[hashkey].address != NULL)
	{
		if(compare((Inferior_datum_mutiplicity*)tbl[hashkey].address) == 0)
		{
			((Inferior_datum_mutiplicity*)tbl[hashkey].address)->mutiplicity += sign;
			((Inferior_datum_mutiplicity*)tbl[hashkey].address)->genius_mutiplicity += sign2;
			((Inferior_datum_mutiplicity*)tbl[hashkey].address)->no_cal_row_sgn_mutiplicity += no_cal_row_sgn_mutiplicity;
			fprintf(f5, "%d\n", hashkey);
			return;
		}
		hashkey *= 2;//+= hashkeystep*100;
		hashkey = hashkey%hashtblsize;
	}

	mutiplicity = sign;
	genius_mutiplicity = sign2;
	no_cal_row_sgn_mutiplicity = no_cal_row_sgn_mutiplicity;
	Inferior_datum_mutiplicity* new_Inf = new Inferior_datum_mutiplicity;
	*new_Inf = *this;
	tbl[hashkey].address = new_Inf;
	fprintf(f5, "%d\n", hashkey);

}