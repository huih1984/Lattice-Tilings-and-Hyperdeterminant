#include "StdAfx.h"
#include "Perm_datum.h"
#include "Inferior_datum.h"
#include "Inferior_datum_mutiplicity.h"
#include "InferiorOper.h"
#include"filemanage.h"
#include<math.h>
#include"InferiorFileRW.h"
#include "preimage.h"
FILE* f, *f2, *f3, *f4, *f5, *f6, *f7;
//f6打印文件寻找左逆元
#define TRAVESE_INFERIOR 1000
#define ORBITED_CHANGE_GROUP 1001
#define LEFTMULTIPLE 1002
#define MUTATIONMULTIPLES 1003
static int OPER = MUTATIONMULTIPLES;
void print_list(Inferior_datum& datum)
{
	int i,j;
	for(i = 0; i < innerdim; ++i)
	{
		for(j = 0; j < outerdim; ++j)
		{
			fprintf(f, "%d", datum.matrix[i][j]);
		}
	}
	fprintf(f, "\n");
}


void add_to_list(Hashtable* tbl, Inferior_datum_mutiplicity& new_datum)
{
	//检验转换是否成功
	new_datum.conv_to_regular();
	new_datum.set_hashkey();

}


void print_list2(Inferior_datum_mutiplicity& datum)
{
	int i,j;
	for(i = 0; i < innerdim; ++i)
	{
		for(j = 0; j < outerdim; ++j)
		{
			fprintf(f2, "%d", datum.matrix[i][j]);
		}
	}
	fprintf(f2, "  %d %d %d\n", datum.mutiplicity, datum.genius_mutiplicity, datum.no_cal_row_sgn_mutiplicity);
}


void print_list3(Inferior_datum& datum)
{
	int i,j;
	for(i = 0; i < innerdim; ++i)
	{
		for(j = 0; j < outerdim; ++j)
		{
			fprintf(f3, "%d", datum.matrix[i][j]);
		}
	}
	fprintf(f3, "  %d %d\n", datum.sign, datum.sign2);
}


void print_list4(Inferior_datum& datum)
{
	int i,j;
	for(i = 0; i < innerdim; ++i)
	{
		for(j = 0; j < outerdim; ++j)
		{
			fprintf(f4, "%d", datum.matrix[i][j]);
		}
	}
	fprintf(f4, "  %d %d\n", datum.sign, datum.sign2);
}

void print_list7(Inferior_datum& datum)
{
	int i,j;
	for(i = 0; i < innerdim; ++i)
	{
		for(j = 0; j < outerdim; ++j)
		{
			fprintf(f7, "%d", datum.matrix[i][j]);
		}
	}
	fprintf(f7, "\n");
}

bool is_primer(unsigned int &s)
{
	for(int i = 2; i <sqrt((double)s); ++i)
	{
		if(s % i == 0)
			return false;
	}
	return true;
}


InferiorOper::InferiorOper(void)
{
	perm.get_permutation();
	try{
		if(OPER == TRAVESE_INFERIOR)
		{
			f = fopen("f://Inferior_datum.txt", "w+");
			f2 = fopen("f://Inferior_datum_mutiplicity.txt", "w+");
			f3 = fopen("f://Inferior_datum_one.txt", "w+");
			f5 = fopen("f://hashkey.txt", "a+");
		}
		else if(OPER == ORBITED_CHANGE_GROUP)
		{
			f4 = fopen("f://Inferior_datum_trans.txt", "w+");
		}
		else if(OPER == LEFTMULTIPLE)
		{
			f6 = fopen("f://leftmultiple_one.txt", "w+");
		}
		else if(OPER == MUTATIONMULTIPLES)
		{
			f7 = fopen("f://mutationmultiple.txt", "w+");
		}
	}
	catch(exception e)
	{
		printf(e.what());
	}
	//计算需求的hashtable的大小，申请数组
	unsigned int s;
	for(s = 1000000*4; ; ++s)
	{
		if(is_primer(s))
			break;
	}
	Hashtable* tbl = new Hashtable[s];
	for(int t = 0; t < s; ++t)
	{
		tbl[t].address = NULL;
	}
	Inferior_datum_mutiplicity::hashtblsize = s;
	Inferior_datum_mutiplicity::tbl = tbl;
}

InferiorOper::~InferiorOper(void)
{
	if(OPER == TRAVESE_INFERIOR)
	{
		fclose(f);
		fclose(f2);
		fclose(f3);
		fclose(f5);
	}
	else if(OPER == ORBITED_CHANGE_GROUP)
	{
		fclose(f4);
	}
	else if(OPER == LEFTMULTIPLE)
	{
		fclose(f6);
	}
}


void InferiorOper::get_traverse_next(Inferior_datum_mutiplicity& inf_d, Inferior_datum_mutiplicity& charge_inf_d, int rownum)
{

	int comp_index = 0,j = 0 ;
	bool right;
	for(int i = 0; i < perm.fac; ++i)
	{
		right = true;
		for(comp_index = 0; comp_index < outerdim; ++comp_index)
		{
			for(j = 0; j < rownum; ++j)
			{
				if(perm.permlist[i].datum[comp_index] == inf_d.matrix[j][comp_index])
				{
					right = false;
					break;
				}
			}
			//只要任意前行的一列数字不同了，就不要再循环了，寻找下一个排列啰
			if(!right)
			{
				break;
			}
		}
		if(right)
		{
			memcpy(inf_d.matrix[rownum], perm.permlist[i].datum, outerdim*sizeof(short));
			if(rownum < innerdim - 1)
			{
				get_traverse_next(inf_d, charge_inf_d, rownum + 1);
			}
			else
			{
				//获得了inf_d，下面将操作交给多个线程来处理
				//主要操作都在这里添加。。。。。。。。。。。。。。。。。。。。。。
				inf_d.sign = inf_d.get_sign();
				inf_d.sign2 = inf_d.get_not_cal_last_row_sign();
				inf_d.no_cal_row_sgn_mutiplicity = inf_d.get_not_cal_all_row_sign();
				print_list(inf_d);
				charge_inf_d = inf_d;
				add_to_list(Inferior_datum_mutiplicity::tbl, charge_inf_d);
				//if(charge_inf_d.compare(&dest_data) == 0)

				if (charge_inf_d.hashkey == preimage.imagehashkey)
				{
					if(preimage.Fetch_smallarray_end == false)
					{
						preimage.get_smallarray(inf_d);
					}
				}
				else
				{
					if(preimage.reminderindex < REMSIZE)
					{
						preimage.fetch_some_reminder(inf_d);
					}
					else
					{
						preimage.print_reminder_collection(inf_d);
					}
				}
			}
		}
	}
}


void  InferiorOper::traverse_Inferior_datum( void ( * print_list)(Inferior_datum&),  void(* addtolist)(Hashtable*, Inferior_datum_mutiplicity&))
{
	Inferior_datum_mutiplicity inf_d, charge_inf_d;
	int comp_index;
	int i,j,k,l;
	int temp_index = 0;
	preimage.imagehashkey = 492;
	for(i = 0; i < 1; ++i)
	{
		//第一行赋值
		memcpy(inf_d.matrix[0], perm.permlist[i].datum, outerdim*sizeof(short));

		get_traverse_next(inf_d, charge_inf_d, 1);
	}
}


void InferiorOper::oper()
{

	if(OPER == TRAVESE_INFERIOR)
	{
		traverse_Inferior_datum(print_list, add_to_list);
		for(int i = 0; i < Inferior_datum_mutiplicity::hashtblsize; ++i)
		{
			if(Inferior_datum_mutiplicity::tbl[i].address != NULL)
				print_list2(*Inferior_datum_mutiplicity::tbl[i].address);
		}
	}
	else if(OPER == ORBITED_CHANGE_GROUP)
	{
		get_filedata();
	}
	else if(OPER == LEFTMULTIPLE)
	{
		get_one_Inferior_datum();
		output_one_Inferior_datum_to_file();
	}
}



void InferiorOper::get_filedata()
{
	CInferiorFileRW Foper;
	Foper.OpenFile("f://Inferior_datum_mutiplicity.txt");
	Inferior_datum out;
	while(!Foper.f.eof())
	{
		if(Foper.ReadLine(out) == false)
		{
			break;
		}
		for(int i = 0;  i < perm.fac; ++i)
		{
			out.Trans(perm.permlist[i]);
			print_list4(out);
		}
	}

}

//临时需要一个产生随机数的函数
int RangedRandDemo( int range_min, int range_max )
{
	int i;
	int u = (double)rand() / (RAND_MAX + 1) * (range_max - range_min)
		+ range_min;
	return u;
}


void InferiorOper::get_one_Inferior_datum()
{
	bool right;
	int randnum = 0;
	for(int i = 0; i < innerdim;  ++i)
	{
		bool right = true;
		int permindex = 0;
		for(permindex = 0; permindex < perm.fac; ++permindex)
		{
			right = true;
			//randnum = RangedRandDemo(0,perm.fac);
			for(int comp_index = 0; comp_index < outerdim; ++comp_index)
			{
				for(int j = 0; j < i; ++j)
				{
					if(perm.permlist[permindex].datum[comp_index] == m_randInferior_datum.matrix[j][comp_index])
					{
						right = false;
						break;
					}
				}
				//只要任意前行的一列数字不同了，就不要再循环了，寻找下一个排列啰
				if(!right)
				{
					break;
				}
			}
			if(right)
			{
				break;
			}
		}
		if(i == 0)
		{
			memcpy(m_randInferior_datum.matrix[i], perm.permlist[0].datum, sizeof(short)*outerdim);
		}
		else
		{

			memcpy(m_randInferior_datum.matrix[i], perm.permlist[permindex].datum, sizeof(short)*outerdim);
		}
	}
}
int stemp = 0;
void InferiorOper::setonerowval(Inferior_datum& one, short rownum)
{
	for(int i = 0; i < perm.fac; ++i)
	{
		memcpy(one.matrix[rownum], perm.permlist[i].datum, sizeof(short)*outerdim);
		if(rownum == innerdim - 1)
		{
			//output_one_Inferior_datum_to_file执行操作实际在这里
			//printf("%d", i);
			++stemp;
			m_leftmultiple_datum = m_randInferior_datum;
			m_leftmultiple_datum.LeftMultiple(one);
			m_leftmultiple_datum.printme(f6);
		}
		else
		{
			//setonerowval(one, rownum + 1);
		}
	}
}
void InferiorOper::output_one_Inferior_datum_to_file()
{
	Inferior_datum leftval;
	for(int i = 0; i < outerdim; ++i)
	{
		leftval.matrix[0][i] = i;
	}
	setonerowval(leftval, 1);
	printf("%d", stemp);
}

void InferiorOper::get_regular_matrix()
{
	fstream stream("E:\\tupleall.txt", ios::in|ios::binary);
	char lline[10] = {0};
	gets(lline);
	char temp[100] = {0};
	int indexli = 0;
	while(indexli++ < atoi(lline))
	{
		stream.getline(temp, 100);
	}
	int streamindex = 0;
	for(int i = 0; i < outerdim; ++i)
	{
		for(int j = 0; j < innerdim; ++j)
		{
			m_regular_matrix.matrix[j][i] = temp[streamindex++] - '0';
		}
	}
}


bool InferiorOper::compare_row(int i)
{
	for(int lv_i = 0;  lv_i < innerdim; ++lv_i)
	{
		for(int lv_j = 0; lv_j < i; ++lv_j)
		{
			if( m_regular_matrix.matrix[lv_i][lv_j] == m_regular_matrix.matrix[lv_i][i])
			{
				return false;
			}
		}
	}
	return true;
}

void InferiorOper::get_dict_next(int n)
{

}

void InferiorOper::oper_multiply_mutations()
{
	CPreimage preimage;
    PermOper permOper;
	permOper.get_permutation();
	for(int i = 0; i < permOper.factorial(); ++i){
		for(int j = i; j < permOper.factorial(); ++j){
			
			memcpy(a.matrix[0],permOper.permlist[i].datum, sizeof(short)*outerdim);
			
			memcpy(a.matrix[1],permOper.permlist[i].datum, sizeof(short)*outerdim);
			short lastval = a.matrix[1][outerdim - 1];
			for(int k = outerdim - 2; k >= 0; --k){
				a.matrix[1][k + 1] = a.matrix[1][k];
			}
			a.matrix[1][0] = lastval;

			memcpy(b.matrix[0],permOper.permlist[j].datum, sizeof(short)*outerdim);
			
			memcpy(b.matrix[1],permOper.permlist[j].datum, sizeof(short)*outerdim);
			lastval = b.matrix[1][outerdim - 1];
			for(int k = outerdim - 2; k >= 0; --k){
				b.matrix[1][k + 1] = b.matrix[1][k];
			}
			b.matrix[1][0] = lastval;
			Inferior_datum_mutiplicity val;
			preimage.multiple(a,b,val);
			print_list7(val);
		}
	}
	
}

void InferiorOper::get_next_array(int n)
{
	while(!compare_row(n))
	{
		get_dict_next(n);
	}
}

void InferiorOper::get_min_matrix()
{
	for(int i = 0; i < outerdim; ++i)
	{
		get_next_array(i);
	}
}

void InferiorOper::get_ascend_next()
{
	for(int i = outerdim; i > 0; --i)
	{
		get_dict_next(i);
	}
}

void InferiorOper::insert_to_vector()
{
	Inferior_datum* lv_new_regular_matrix = new Inferior_datum;
	*lv_new_regular_matrix = m_regular_matrix;
	m_v_regular_matrix.push_back(lv_new_regular_matrix);
}

int m_temp = 0;//计算每个固定标准元的变换的个数
/*就每一列元素集合保持相同的情况，遍历所有可能，得到最终能够变换成相同标准型的结果放到m_v_regular_matrix中*/
void InferiorOper::get_row_next(short row_num)
{
	if(row_num == 7)
	{
		memset(temp_store_matrix, false, sizeof(temp_store_matrix));
		for(int i = 0; i < innerdim; ++i)
		{
			for(int j = 0; j < outerdim - 1; ++j)
			{
				temp_store_matrix[i][m_regular_matrix.matrix[i][j]] = true;
			}
		}
		for(int i = 0; i < innerdim; ++i)
		{
			for(int j = 0; j < outerdim; ++j)
			{
				if(temp_store_matrix[i][j] == false)
				{
					m_regular_matrix.matrix[i][outerdim - 1] = j;
					break;
				}
			}
		}
		m_temp++;

		Inferior_datum* lv_new_regular_matrix = new Inferior_datum;
		*lv_new_regular_matrix = m_regular_matrix;
		m_v_regular_matrix.push_back(lv_new_regular_matrix);
		return;
	}
	for(int i = 0; i < 24; ++i)
	{
		for(int j = 0; j < innerdim; j ++)
		{
			m_regular_matrix.matrix[j][row_num] = m_permus[row_num][i][j];
		}
		if(row_num > 0)
		{
			if(compare_row(row_num))
			{
				get_row_next(row_num + 1);
			}			
		}
		else
		{
			get_row_next(row_num + 1);
		}
	}
}

/*从4*8的所有排列的文件中摘取一行，作为原始数据，对其进行变换得到针对一列是相同集合的情形，所有具有相同正则性的结果放到list中，并打印。这里由于对第一行没有进行排序，所以结果有可能存在相同的情况，没有进行删除处理*/
void InferiorOper::oper_one_regular_trans()
{
	get_regular_matrix();
	for(int i = 0; i < outerdim - 1; ++i)
	{
		get_permutation_row(i);
	}
	m_total = 0;
	get_row_next(0);
	FILE* f = fopen("f://regular_matrix.txt", "a+");
	int lv_sign;
	list<Inferior_datum*>::iterator it = m_v_regular_matrix.begin();
	int lv_listnum = 0;
	while(it != m_v_regular_matrix.end())
	{
		qsort(&((*it)->matrix[0][0]), outerdim, sizeof(short), sizeof(short), sizeof(short)*outerdim, innerdim, comp);
		(*it)->printme("f://regular_matrix.txt");
		delete (*it);
		(*it) = NULL;
		it++;
	}
	
	printf("   %d\n", m_total);
	printf("%d", m_temp);
	fclose(f);
	system("pause");
}

/*将从文件中获取的一个4*8下标矩阵m_regular_matrix的每一列的全排列24个值按照字典序从小到大顺序排列放在m_permus中*/
void InferiorOper::get_permutation_row(int n)
{
	for (int i = 0; i < innerdim; ++i)
	{
		m_permus[n][0][i] = m_regular_matrix.matrix[i][n];
	};

	for (unsigned int i = 1;i < 24; ++i)//从后面取到一个比这个数字大的数
	{
		bool havegot=0;
		int minp=0;
		int minindex=-1;
		int j=0;
		for(j = innerdim - 2;j >= 0;--j)
		{

			for(int k = j + 1; k < innerdim; ++k)
			{
				if(m_permus[n][i - 1][j]<m_permus[n][i - 1][k])
				{
					havegot = 1;
					minp = m_permus[n][i - 1][k];
					minindex = k;
				}//如果j后面有数字比这个数字大找到最小的那个，交换，后对i+1___outerdimension排序
				if(havegot==1&&m_permus[n][i - 1][k]>m_permus[n][i - 1][j]&&m_permus[n][i - 1][k]<minp)
				{
					minindex = k;
					minp = m_permus[n][i - 1][k];
				}
			}
			if(havegot == 1)break;
		}
		for(int s = 0;s < innerdim; ++s)
		{
			m_permus[n][i][s] = m_permus[n][i-1][s];
		}
		int temp;
		temp = m_permus[n][i][j];
		m_permus[n][i][j] = m_permus[n][i][minindex];
		m_permus[n][i][minindex] = temp;
		for(int s = j + 1;s < innerdim; ++s)
		{
			int ind=s;
			int ss;
			for(ss= s + 1; ss < innerdim; ++ss)
			{
				if(m_permus[n][i][ss] < m_permus[n][i][ind])
					ind = ss;
			}
			int temp;
			if(ind!=s)
			{
				temp = m_permus[n][i][s];
				m_permus[n][i][s] = m_permus[n][i][ind];
				m_permus[n][i][ind] = temp;
			}
		}
	}
}

short matrix[100000][innerdim][outerdim];
/*将oper_one_regular_trans中得到的结果进行排序*/
void InferiorOper::oper_one_origins()
{
	fstream stream("f://regular_matrix.txt", ios::in|ios::binary);
	char temp[100] = {0};
	int indexli = 0;
	while(!stream.eof())
	{
		stream.getline(temp, 100);
		int streamindex = 0;
		for(int j = 0; j < innerdim; ++j)
		{
			for(int i = 0; i < outerdim; ++i)
			{
				matrix[indexli][j][i] = temp[streamindex++] - '0';
			}
		}
		indexli++;
	}
	qsort(matrix, m_temp, sizeof(short)*32, sizeof(short), sizeof(short), 32, comp);
	Inferior_datum lv_temp_inf;
	for(int i = 0; i < m_temp; ++i)
	{
		memcpy(lv_temp_inf.matrix, matrix[i], sizeof(lv_temp_inf.matrix));
		lv_temp_inf.printme("f://sort_regular_matrix.txt");
	}
}


void InferiorOper::get_positive_num()
{
	fstream stream("f://sort_no_multi_regular_matrix.txt", ios::in|ios::binary);
	char temp[100] = {0};
	int indexli = 0;
	while(!stream.eof())
	{
		stream.getline(temp, 100);
		if(temp[33] == '1')
		{
			indexli++;
		}
		
	}
	printf("%d", indexli);
}

int sort_regular_lines = 0;
int m_sgn = 0;
void InferiorOper::oper_one_sort_origins()
{
	fstream stream("f://sort_regular_matrix.txt", ios::in|ios::binary);
	char temp[100] = {0};
	int indexli = 0;
	while(!stream.eof())
	{
		stream.getline(temp, 100);
		int streamindex = 0;
		for(int j = 0; j < innerdim; ++j)
		{
			for(int i = 0; i < outerdim; ++i)
			{
				matrix[indexli][j][i] = temp[streamindex++] - '0';
			}
		}
		indexli++;
	}
	vector<Inferior_datum> lv_v_temp_inf;
	Inferior_datum lv_temp_inf;
	int lv_index = 0;
	int sgn = 0;
	for(int i = 0; i < m_temp; ++i)
	{
		memcpy(lv_temp_inf.matrix, matrix[i], sizeof(lv_temp_inf.matrix));
		if(lv_index > 0)
		{
			if(lv_temp_inf.compare(&lv_v_temp_inf[lv_index - 1]) != 0)
			{
				sgn += lv_temp_inf.get_sign();
				lv_temp_inf.printme("f://sort_no_multi_regular_matrix.txt");
				lv_v_temp_inf.push_back(lv_temp_inf);
				lv_index++;
			}
		}
		else
		{
			sgn += lv_temp_inf.get_sign();
			lv_temp_inf.printme("f://sort_no_multi_regular_matrix.txt");
			lv_v_temp_inf.push_back(lv_temp_inf);
			lv_index++;
		}
	}
	sort_regular_lines = lv_index;
	m_sgn = sgn;
	printf("%d", sgn);
}


void InferiorOper::oper_one_six_sum_factorial(){
	
	FILE* f = fopen("f://six_sum.txt", "a+");
	fstream stream("f://sort_no_multi_regular_matrix.txt", ios::in|ios::binary);
	char temp[100] = {0};
	int indexli = 0;
	while(!stream.eof())
	{
		stream.getline(temp, 100);
		int streamindex = 0;
		for(int j = 0; j < innerdim; ++j)
		{
			for(int i = 0; i < outerdim; ++i)
			{
				matrix[indexli][j][i] = temp[streamindex++] - '0';
			}
		}
		indexli++;
	}
	fprintf(f,"%s ", temp);
	vector<Inferior_datum> lv_v_temp_inf;
	Inferior_datum lv_temp_inf;
	int lv_index = 0;
	int six_sum = 0;
	for(int i = 0; i < sort_regular_lines; ++i)
	{
		memcpy(lv_temp_inf.matrix, matrix[i], sizeof(lv_temp_inf.matrix));
		six_sum += lv_temp_inf.get_six_sum_factor();
	}
	fprintf(f,"sort_regular_lines = %d, m_sgn=%d, six_sum=%d", sort_regular_lines, m_sgn, six_sum); 
	fclose(f);
}


void InferiorOper::oper_one_six_fivemuti_sum_factorial()
{
FILE* f = fopen("f://six_fivemuti_sum.txt", "a+");
	fstream stream("f://sort_no_multi_regular_matrix.txt", ios::in|ios::binary);
	char temp[100] = {0};
	int indexli = 0;
	while(!stream.eof())
	{
		stream.getline(temp, 100);
		int streamindex = 0;
		for(int j = 0; j < innerdim; ++j)
		{
			for(int i = 0; i < outerdim; ++i)
			{
				matrix[indexli][j][i] = temp[streamindex++] - '0';
			}
		}
		indexli++;
	}
	fprintf(f,"%s ", temp);
	vector<Inferior_datum> lv_v_temp_inf;
	Inferior_datum lv_temp_inf;
	int lv_index = 0;
	int six_fivemuti_sum = 0;
	for(int i = 0; i < sort_regular_lines; ++i)
	{
		memcpy(lv_temp_inf.matrix, matrix[i], sizeof(lv_temp_inf.matrix));
		six_fivemuti_sum += lv_temp_inf.get_six_fivemuti_sum_factor();
	}
	fprintf(f,"sort_regular_lines = %d, m_sgn=%d, six_fivemuti_sum=%d", sort_regular_lines, m_sgn, six_fivemuti_sum); 
	fclose(f);
}

void InferiorOper::oper_one_fifteen_doublemuti_sum_factor()
{
FILE* f = fopen("f://fifteen_doublemuti_sum.txt", "a+");
	fstream stream("f://sort_no_multi_regular_matrix.txt", ios::in|ios::binary);
	char temp[100] = {0};
	int indexli = 0;
	while(!stream.eof())
	{
		stream.getline(temp, 100);
		int streamindex = 0;
		for(int j = 0; j < innerdim; ++j)
		{
			for(int i = 0; i < outerdim; ++i)
			{
				matrix[indexli][j][i] = temp[streamindex++] - '0';
			}
		}
		indexli++;
	}
	fprintf(f,"%s ", temp);
	vector<Inferior_datum> lv_v_temp_inf;
	Inferior_datum lv_temp_inf;
	int lv_index = 0;
	int fifteen_doublemuti_sum = 0;
	for(int i = 0; i < sort_regular_lines; ++i)
	{
		memcpy(lv_temp_inf.matrix, matrix[i], sizeof(lv_temp_inf.matrix));
		fifteen_doublemuti_sum += lv_temp_inf.get_fifteen_doublemuti_sum_factor();
	}
	fprintf(f,"sort_regular_lines = %d, m_sgn=%d, fifteen_doublemuti_sum=%d", sort_regular_lines, m_sgn, fifteen_doublemuti_sum); 
	fclose(f);
}

void InferiorOper::oper_one_part_to_class()
{
	fstream stream("F:\\sort_no_multi_regular_matrix.txt", ios::in|ios::binary);
	fstream stream_regular("F:\\sort_no_multi_regular_matrix_class_regular.txt", ios::out|ios::binary);
	fstream stream_noregular("F:\\sort_no_multi_regular_matrix_class_noregular.txt", ios::out|ios::binary);
	char temp[100] = {0};
	short index_mark[4][2];
	bool got_reg_bool, shouldnext;
go:;
	while(!stream.eof())
	{
		stream.getline(temp, 100);
		int streamindex = 0;
		shouldnext = false;
		got_reg_bool = false;
		for(int i = 0; i < innerdim; ++i)
		{
			for(int j = 0; j < outerdim; ++j)
			{
				m_regular_matrix.matrix[i][j] = temp[streamindex++] - '0';
				if(j < 4 && m_regular_matrix.matrix[i][j] == 5)
				{
					index_mark[0][0] = i;
					index_mark[0][1] = j;
				}
				if(j < 4 && m_regular_matrix.matrix[i][j] == 6)
				{
					index_mark[1][0] = i;
					index_mark[1][1] = j;
				}
				if(j >= 4 && m_regular_matrix.matrix[i][j] == 3)
				{
					index_mark[2][0] = i;
					index_mark[2][1] = j;
				}
				if(j >= 4 && m_regular_matrix.matrix[i][j] == 2)
				{
					index_mark[3][0] = i;
					index_mark[3][1] = j;
				}
			}
		}
		if (index_mark[1][0] == index_mark[2][0] && index_mark[0][0] == index_mark[3][0])
		{
			for(int k = 0; k < innerdim; ++k)
			{
				if(k != index_mark[1][0])
				{
					if(m_regular_matrix.matrix[k][index_mark[1][1]]== 3)
						goto go2;
				}
			}
			stream_regular<<temp;
			goto go;
		}
go2:;
		stream_noregular<<temp;
		goto go;
	}
}