#include "StdAfx.h"
#include"qsort.h"
#include<stack>
#include "Inferior_datum.h"

Inferior_datum::Inferior_datum()
{
	
}

Inferior_datum::~Inferior_datum()
{
}


short Inferior_datum::get_sign()
{
	sign = 1;
	short i,j,k;
	for(i = 0; i < innerdim; ++i)
	{
		for(j = 0; j < outerdim ; ++j)
		{
			for(k = j + 1; k < outerdim; ++k)
			{
				if(matrix[i][j] > matrix[i][k])
				{
					sign *= -1;
				}
			}

			for(k = i + 1; k < innerdim; ++k)
			{
				if(matrix[i][j] > matrix[k][j])
				{
					sign *= -1;
				}
			}
		}
	}

	return sign;
}

short Inferior_datum::get_six_sum_factor()
{
	int sgn[3] = {1,1,1};
	int myValuefactor = 1;
	short i,j,k;
	for(i = 0; i < innerdim; ++i)
	{
		for(j = 0; j < outerdim ; ++j)
		{
			for(k = j + 1; k < outerdim; ++k)
			{
				if(matrix[i][j] > matrix[i][k])
				{
					if(i >0)
					{
						sgn[i - 1] *= -1;
					}
				}
			}

			for(k = i + 1; k < innerdim; ++k)
			{
				if(matrix[i][j] > matrix[k][j])
				{
					myValuefactor *= -1;
				}
			}
		}
	}
	return (sgn[0]+sgn[1]+sgn[2]+sgn[0]*sgn[1]+sgn[0]*sgn[2]+sgn[1]*sgn[2])*myValuefactor;
}


short Inferior_datum::get_six_fivemuti_sum_factor()
	{
		int sgn[3] = {1,1,1};
	int myValuefactor = 1;
	short i,j,k;
	for(i = 0; i < innerdim; ++i)
	{
		for(j = 0; j < outerdim ; ++j)
		{
			for(k = j + 1; k < outerdim; ++k)
			{
				if(matrix[i][j] > matrix[i][k])
				{
					if(i >0)
					{
						sgn[i - 1] *= -1;
					}
				}
			}

			for(k = i + 1; k < innerdim; ++k)
			{
				if(matrix[i][j] > matrix[k][j])
				{
					myValuefactor *= -1;
				}
			}
		}
	}
	return sgn[0]*sgn[1]*sgn[2]*(sgn[0] +sgn[1]+sgn[2]+sgn[0]*sgn[1]+sgn[0]*sgn[2]+sgn[1]*sgn[2])*myValuefactor;

	}


short Inferior_datum::get_fifteen_doublemuti_sum_factor()
{
	int sgn[3] = {1,1,1};
	int myValuefactor = 1;
	short i,j,k;
	for(i = 0; i < innerdim; ++i)
	{
		for(j = 0; j < outerdim ; ++j)
		{
			for(k = j + 1; k < outerdim; ++k)
			{
				if(matrix[i][j] > matrix[i][k])
				{
					if(i >0)
					{
						sgn[i - 1] *= -1;
					}
				}
			}

			for(k = i + 1; k < innerdim; ++k)
			{
				if(matrix[i][j] > matrix[k][j])
				{
					myValuefactor *= -1;
				}
			}
		}
	}
	return (sgn[0]*sgn[1] + sgn[0]*sgn[2] + sgn[1] + sgn[2] + sgn[0]*sgn[1]*sgn[2] + sgn[1]*sgn[2] + sgn[0] + sgn[0]*sgn[1]*sgn[2] + sgn[2] + sgn[0]*sgn[1]*sgn[2] + sgn[0] + sgn[1] + sgn[1]*sgn[2] + sgn[0]*sgn[2] + sgn[0]*sgn[1])*myValuefactor;
}


short Inferior_datum::get_not_cal_last_row_sign()
{
	sign2 = sign;
	for(int i = 0; i < outerdim; ++i)
	{
		for(int j = i + 1; j < outerdim; ++j)
		{
			if(matrix[innerdim -1][j] < matrix[innerdim -1][i])
				sign2 *= -1;
		}
	}
	return sign2;
}


short Inferior_datum::get_not_cal_all_row_sign()
{
	sign_not_cal_all = 1;
	short i,j,k;
	for(i = 0; i < innerdim; ++i)
	{
		for(j = 0; j < outerdim ; ++j)
		{
			for(k = i + 1; k < innerdim; ++k)
			{
				if(matrix[i][j] > matrix[k][j])
				{
					sign_not_cal_all *= -1;
				}
			}
		}
	}
	return sign_not_cal_all;
}


int comp(void*a, void*b, size_t& element_interval_size, size_t& element_num)
{
	short* t_a = (short*)a, * t_b = (short*)b;
	for(int i = 0; i < element_num; ++i)
	{
		if(*t_a - *t_b < 0)
			return -1;
		else if(*t_a - *t_b > 0)
			return 1;
		t_a += element_interval_size/sizeof(short);
		t_b += element_interval_size/sizeof(short);
	}
	return 0;
}


void Inferior_datum::swap(short** para1, short** para2)
{
	for(short i = 0; i < innerdim; i++)
	{
		*((*para1) + i) = *((*para1) + i)^*((*para2) + i);
		*((*para2) + i) = *((*para1) + i)^*((*para2) + i);
		*((*para1) + i) = *((*para1) + i)^*((*para2) + i);
	}
}


void Inferior_datum::this_qsort(short**a,short n)
{
	stack<matrix_Param> sp;
	matrix_Param pm;
	for(;;)
	{
		if(n<=2)
		{
			if(n==2 && **(a + 1) < **a)swap(a + 1,a);
			if(sp.empty())break;
			pm=sp.top();
			sp.pop();
			a=pm.a;
			n=pm.n;
			continue;
		}
		swap(a,a+(n>>1));
		short** pivot=a;
		short **l= a + 1;
		short **r=a + (n-1);
		while(l<r)
		{
			while(** l< **r && **l < **pivot)l = l + 1;
			while(**r > **a && **pivot < **r || **pivot < **r) r = r - 1;
			if(**l < **r)swap(l,r);
		}
		if(**a > **r)swap(a,r);
		pm.a = r + 1,pm.n = n-1-(r-a)/sizeof(short);
		sp.push(pm);
		n=(r-a)/sizeof(short);
	}
}

void Inferior_datum::get_regular_datum(Inferior_datum& reg_datum)
{
	reg_datum = *this;
	reg_datum.conv_to_regular();
}


short Inferior_datum::compare(const Inferior_datum* datum2)
{
	short i,j,k;
	for(i = 0; i < outerdim; ++i)
	{
		for(j = 0; j < innerdim; ++j)
		{
			if(this->matrix[j][i] < datum2->matrix[j][i])
			{
				return -1;
			}
			else if(this->matrix[j][i] > datum2->matrix[j][i])
			{
				return 1;
			}
		}
	}
	return 0;
}


void Inferior_datum::conv_to_regular()
{
	for(int i = 0; i < outerdim; ++i)
	{
		//printf("%d", innerdim);
		qsort(&(matrix[0][i]), innerdim,  sizeof(short)*outerdim, sizeof(short), sizeof(short)*outerdim, 1, comp);
	}
	qsort(&(matrix[0][0]), outerdim, sizeof(short), sizeof(short), sizeof(short)*outerdim, innerdim, comp);
}


void Inferior_datum::Trans(Perm_datum& data)
{
	for(int i = 0; i < outerdim; ++i)
	{
		for(int j = 0; j < innerdim; ++j)
		{
			matrix[j][i] = data.datum[matrix[j][i]];
		}
	}
	qsort(&(matrix[0][0]), outerdim, sizeof(short), sizeof(short), sizeof(short)*outerdim, innerdim, comp);
	get_sign();
	get_not_cal_last_row_sign();
	
}


void Inferior_datum::LeftMultiple(Inferior_datum & leftval)
{
	//第一个行肯定是顺序排列的，所以不用去运算
	for(int i = 1; i < innerdim; ++i)
	{
		OnePermTrans(leftval.matrix[i]);
	}
	conv_to_regular();
}


void Inferior_datum::OnePermTrans(short arraynums[outerdim])
{
	for(int i = 0; i < innerdim; ++i)
	{
		for(int j = 0; j < outerdim; ++j)
		{
			matrix[i][j] = arraynums[matrix[i][j]];
		}
	}
}


void Inferior_datum::printme(FILE* f)
{
	for(int i = 0; i < innerdim; ++i)
	{
		for(int j = 0; j < outerdim; ++j)
		{
			fprintf(f, "%d", matrix[i][j]);
		}
	}
	fprintf(f, "\n");
}


void Inferior_datum::printme(char* pathname)
{
	FILE* f = fopen(pathname, "a+");
	for(int i = 0; i < innerdim; ++i)
	{
		for(int j = 0; j < outerdim; ++j)
		{
			fprintf(f, "%d", matrix[i][j]);
		}
	}
	fprintf(f, " %d\n", get_sign());
	fclose(f);
}

void Inferior_datum::printme()
{
	for(int i = 0; i < innerdim; ++i)
	{
		for(int j = 0; j < outerdim; ++j)
		{
			printf("%d", matrix[i][j]);
		}
	}
	printf("\n");
}



int  Inferior_datum::getinversesign(short arraynums[], short size)
{
	int sgn = 1;
	for(int i = 1;  i < size; ++i)
	{
		for(int j = 0; j < i; ++j)
		{
			if(arraynums[i] < arraynums[j])
			{
				sgn*= -1;
			}
		}
	}
	return sgn;
}
