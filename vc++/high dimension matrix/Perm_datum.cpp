#include "StdAfx.h"
#include "Perm_datum.h"
#include <list>
#include <vector>
using namespace std;
Perm_datum::Perm_datum(void)
{
}

Perm_datum::~Perm_datum(void)
{
}






PermOper::PermOper()
{
	unsigned int fac = factorial();
	permlist = new Perm_datum[fac];
}


PermOper::~PermOper()
{
	delete []permlist;
	permlist = NULL;
}


unsigned int PermOper::factorial()
{
	unsigned int k=1;
	for(int i = 1; i <= outerdim; ++i)
	{
		k = k * i;
	}
	fac = k;
	return k;
}

int PermOper::get_sign(Perm_datum permdata)
{
	int sign = 1;
	short j,k;
	for(j = 0; j < outerdim ; ++j)
	{
		for(k = j + 1; k < outerdim; ++k)
		{
			if(permdata.datum[j] > permdata.datum[k])
			{
				sign *= -1;
			}
		}
	}
	return sign;
}


vector<vector<int>> getVecList(int mat[outerdim][outerdim][outerdim][outerdim][outerdim][outerdim])
{
	vector<vector<int>> data;
	for(int i = 0; i < outerdim; ++i)
	{
		for(int j = 0; j < outerdim; ++j)
		{
			for(int k = 0; k< outerdim; ++k)
			{
				for(int l = 0; l < outerdim; ++l)
				{
					for(int m = 0; m < outerdim; ++m)
					{
						for(int n = 0; n < outerdim; ++n)
						{
							if(abs(mat[i][j][k][l][m][n]) != 0 ){
								vector<int> item;
								item.push_back(i);
								item.push_back(j);
								item.push_back(k);
								item.push_back(l);
								data.push_back(item);
							}
						}
					}
				}
			}
		}
	}
	return data;
}

bool index2Regal(Perm_datum permdata, int i, vector<int> vec )
{
	for(int j = 0; j < outerdim; ++j){
		if(i == vec[j]){
			if(permdata.datum[i] == vec[(j+1)%4])
			{
				return true;
			}
		}else{
			return false;
		}
	}
	return false;
}

bool index2RegalAll(Perm_datum permdata, vector<vector<int>> list)
{
	for(int i = 0; i < outerdim; ++i){
		bool init = false;
		for(int j = 0; j < list.size(); ++j)
		{
			if(index2Regal(permdata, i, list[j]))
			{
				init = true;
				break;
			}
		}
		if(init == false)
		{
			return false;
		}
	}
	return true;
}


bool index3Regal(Perm_datum permdata, Perm_datum Prepermdata, int i, vector<int> vec )
{
	for(int j = 0; j < 4; ++j){
		if(Prepermdata.datum[i] == vec[j]&&i==vec[(j+3)%4]){
			if(permdata.datum[i] == vec[(j+1)%4])
			{
				return true;
			}
		}else{
			return false;
		}
	}
	return false;
}



bool index3RegalAll(Perm_datum permdata, Perm_datum Prepermdata, vector<vector<int>> list)
{
	for(int i = 0; i < 4; ++i){
		bool init = false;
		for(int j = 0; j < list.size(); ++j)
		{
			if(index3Regal(permdata, Prepermdata, i, list[j]))
			{
				init = true;
				break;
			}
		}
		if(init == false)
		{
			return false;
		}
	}
	return true;
}

bool index4Regal(Perm_datum permdata, Perm_datum Prepermdata,Perm_datum PrePrepermdata, int i, vector<int> vec )
{
	for(int j = 0; j < 4; ++j){
		if(Prepermdata.datum[i] == vec[j] && PrePrepermdata.datum[i] == vec[(j+3)%4] && i == vec[(j+2)%4]){
			if(permdata.datum[i] == vec[(j+1)%4])
			{
				return true;
			}
		}else{
			return false;
		}
	}
	return false;
}

bool index4RegalAll(Perm_datum permdata, Perm_datum Prepermdata,Perm_datum PrePrepermdata, vector<vector<int>> list)
{
	for(int i = 0; i < 4; ++i){
		bool init = false;
		for(int j = 0; j < list.size(); ++j)
		{
			if(index4Regal(permdata, Prepermdata, PrePrepermdata, i, list[j]))
			{
				init = true;
				break;
			}
		}
		if(init == false)
		{
			return false;
		}
	}
	return true;
}
int PermOper::det()
{
	int mat[outerdim][outerdim][outerdim][outerdim][outerdim][outerdim] = {0};
	//mat[0][1][2][3] = 1;
	//mat[4][5][6][7] = 1;
	//mat[8][9][10][11] = 1;
	//mat[1][2][3][4] = 1;
	//mat[2][3][4][0] = 1;
	//mat[3][4][0][1] = 1;
	//mat[4][0][1][2] = 1;
	//mat[6][7][8][9] = 1;
	//mat[7][8][9][10] = 1;
	//mat[8][9][10][11] = 1;
	//mat[9][10][11][5] = 1;
	//mat[10][11][5][6] = 1;
	//mat[11][5][6][7] = 1;
	//mat[3][5][1][2] = 1;
	//mat[7][4][2][6] = 1;
	//mat[2][0][4][5] = 1;
	//mat[6][7][0][1] = 1;
	//mat[1][2][3][4] = 1;
	//mat[5][6][7][0] = 1;
	int val = 1;
	for(int i = 0; i < outerdim; ++i)
	{
		for(int j = 0; j < outerdim; ++j)
		{
			for(int k = 0; k< outerdim; ++k)
			{
				for(int l = 0; l < outerdim; ++l)
				{
					for(int m = 0; m < outerdim; ++m)
					{
						for(int n = 0; n < outerdim; ++n)
						{
							if(i != j && ((i == k && j == l) || (i == l && j == k)) && ((i == m && j == n) || (i == n && j == m)))
							{
								if(i < j)
								{
									if( k < l && m < n) mat[i][j][k][l][m][n] = val;
									if( k < l && m > n) mat[i][j][k][l][m][n] = -val;
									if( k > l && m > n) mat[i][j][k][l][m][n] = val;
									if( k > l && m < n) mat[i][j][k][l][m][n] = -val;
								} else 
								{
									if( k < l && m < n) mat[i][j][k][l][m][n] = -val;
									if( k < l && m > n) mat[i][j][k][l][m][n] = val;
									if( k > l && m > n) mat[i][j][k][l][m][n] = -val;
									if( k > l && m < n) mat[i][j][k][l][m][n] = val;
								}
							}
						}
					}
					/*if(j==i || k==l){
					mat[i][j][k][l] = 0;
					} else if(abs(mat[i][j][k][l]) == 0){
					if(i < j && i == k && j == l){
					if(i == 0){
					mat[i][j][k][l] = val;
					} else {
					mat[i][j][k][l] = val;
					}
					} else if(i == k && j == l){
					mat[i][j][k][l] = val;
					}
					}*/
				}
			}
		}
	}
	//mat[0][2][0][2] = -2;
	//mat[0][3][0][3] = -2;
	//mat[0][1][0][1] = -2;
	//mat[1][2][1][2] = 2;
	for(int i = 0; i < outerdim; ++i)
	{
		for(int j = 0; j < outerdim; ++j)
		{
			for(int k = 0; k< outerdim; ++k)
			{
				for(int l = 0; l < outerdim; ++l)
				{
					for(int m = 0; m < outerdim; ++m)
					{
						for(int n = 0; n < outerdim; ++n)
						{
							//if(abs(mat[j][i][k][l]) != 0 && j < i)
							//{
							//	if( k <l){
							//		mat[i][j][k][l] = -mat[j][i][k][l];
							//	} else {
							//		mat[i][j][k][l] = mat[j][i][l][k];
							//	}
							//} else if(abs(mat[j][i][k][l]) != 0 && j > i && l < k)
							//{
							//	mat[i][j][k][l] = -mat[i][j][l][k];
							//} 
							printf("[%d %d %d %d %d %d] %d", i,j,k,l,m,n,mat[i][j][k][l][m][n]);
						}
						printf("\n");
					}

					printf("\n");
				}
				printf("\n");

			}
			printf("\n");
		}

		printf("\n");
	}
	get_permutation();
	vector<vector<int>> vList = getVecList(mat);
	long long det = 0;
	FILE* f = fopen("D://dat.txt", "w+");
	int fac = factorial();
	for(int s1 = 0; s1 < fac; ++s1)
	{
		/*for(int s2 = 0; s2 < fac; ++s2)
		{

		for(int s3 = 0; s3 < fac; ++s3)
		{

		int temp = get_sign(permlist[s1])*get_sign(permlist[s2])*get_sign(permlist[s3])*
		mat[0][permlist[s1].datum[0]][permlist[s2].datum[0]][permlist[s3].datum[0]]*
		mat[1][permlist[s1].datum[1]][permlist[s2].datum[1]][permlist[s3].datum[1]]*
		mat[2][permlist[s1].datum[2]][permlist[s2].datum[2]][permlist[s3].datum[2]]*
		mat[3][permlist[s1].datum[3]][permlist[s2].datum[3]][permlist[s3].datum[3]];
		det += temp;
		if(temp != 0)
		{
		fprintf(f, "%d,%d,%d,%d,\n%d,%d,%d,%d,\n%d,%d,%d,%d,\n%d,%d,%d,%d, %d\n", 
		0,permlist[s1].datum[0],permlist[s2].datum[0], permlist[s3].datum[0], 
		1,permlist[s1].datum[1],permlist[s2].datum[1], permlist[s3].datum[1], 
		2,permlist[s1].datum[2],permlist[s2].datum[2], permlist[s3].datum[2], 
		3,permlist[s1].datum[3],permlist[s2].datum[3], permlist[s3].datum[3], 
		temp);
		}
		}
		}*/
		int temp = 0;
		Perm_datum s2, s3, s4,s5;
		int sss = 0;
		getOneItem(temp, 0, permlist[s1], s2, s3,s4,s5, mat,det, sss, f);
	}
	fclose(f);
	return det;
}

bool regal(Perm_datum s1, Perm_datum& s2, Perm_datum& s3, Perm_datum& s4, Perm_datum& s5, int outIndex)
{
	bool legal = true;
	for(int tt = 0; tt < outIndex; ++tt)
	{
		if(s2.datum[tt] == s2.datum[outIndex] || s3.datum[tt] == s3.datum[outIndex]
		|| s4.datum[tt] == s4.datum[outIndex] || s5.datum[tt] == s5.datum[outIndex])
		{
			legal = false;
			break;
		}
	}
	return legal;
}

void PermOper::getOneItem(int& temp, int outIndex, Perm_datum s1, Perm_datum& s2, Perm_datum& s3, Perm_datum& s4, Perm_datum& s5, int mat[outerdim][outerdim][outerdim][outerdim][outerdim][outerdim], long long& det, int& sss, FILE* f)
{

	if(outIndex < outerdim)
	{

		s2.datum[outIndex] = outIndex;
		s3.datum[outIndex] = s1.datum[outIndex];
		s4.datum[outIndex] = outIndex;
		s5.datum[outIndex] = s1.datum[outIndex];
		bool legal = true;
		legal = regal(s1, s2, s3, s4,s5,outIndex);
		if(legal)
		{
			getOneItem(temp, outIndex+ 1, s1,s2,s3,s4,s5,mat, det, sss, f);
		}

		s2.datum[outIndex] = outIndex;
		s3.datum[outIndex] = s1.datum[outIndex];
		s4.datum[outIndex] = s1.datum[outIndex];
		s5.datum[outIndex] = outIndex;
		legal = true;
		legal = regal(s1, s2, s3, s4,s5,outIndex);
		if(legal)
		{
			getOneItem(temp, outIndex+ 1, s1,s2,s3,s4,s5,mat, det, sss, f);
		}

		s3.datum[outIndex] = outIndex;
		s2.datum[outIndex] = s1.datum[outIndex];
		s4.datum[outIndex] = outIndex;
		s5.datum[outIndex] = s1.datum[outIndex];
		legal = true;
		legal = regal(s1, s2, s3, s4,s5,outIndex);
		if(legal)
		{
			getOneItem(temp, outIndex+ 1, s1,s2,s3,s4,s5,mat, det, sss, f);
		}

		s3.datum[outIndex] = outIndex;
		s2.datum[outIndex] = s1.datum[outIndex];
		s4.datum[outIndex] = s1.datum[outIndex];
		s5.datum[outIndex] = outIndex;
		legal = true;
		legal = regal(s1, s2, s3, s4,s5,outIndex);
		if(legal)
		{
			getOneItem(temp, outIndex+ 1, s1,s2,s3,s4,s5,mat, det, sss, f);
		}
	}
	else
	{
		sss++;
		int cal  = 1;
		for(int i = 0; i < outerdim; ++i)
		{
			cal *= mat[i][s1.datum[i]][s2.datum[i]][s3.datum[i]][s4.datum[i]][s5.datum[i]];

		}
		temp = get_sign(s1)*get_sign(s2)*get_sign(s3)*get_sign(s4)*get_sign(s5)*cal;
		if(temp  != 0)
		{
			/*							if(permlist[s1].datum[0] == 1 && permlist[s1].datum[1] == 0 && 
			permlist[s1].datum[2] == 3 && permlist[s1].datum[3] == 2)
			{*/
			det += temp;
			if(sss > 1000) return;

			fprintf(f, "%d,%d,%d,%d,%d,%d,\n%d,%d,%d,%d,%d,%d,\n%d,%d,%d,%d,%d,%d,\n%d,%d,%d,%d,%d,%d,\n%d,%d,%d,%d,%d,%d,\n%d,%d,%d,%d,%d,%d,\n%d,%d,%d,%d,%d,%d, %d\n", 
				0,s1.datum[0],s2.datum[0], s3.datum[0], s4.datum[0], s5.datum[0], 
				1,s1.datum[1],s2.datum[1], s3.datum[1], s4.datum[1], s5.datum[1], 
				2,s1.datum[2],s2.datum[2], s3.datum[2], s4.datum[2], s5.datum[2], 
				3,s1.datum[3],s2.datum[3], s3.datum[3], s4.datum[3], s5.datum[3], 
				4,s1.datum[4],s2.datum[4], s3.datum[4], s4.datum[4], s5.datum[4], 
				5,s1.datum[5],s2.datum[5], s3.datum[5], s4.datum[5], s5.datum[5], 
				6,s1.datum[6],s2.datum[6], s3.datum[6], s4.datum[6], s5.datum[6], 
				temp);

		}
	}
}

void PermOper::get_permutation()
{
	for (int i = 0; i < outerdim; ++i)
	{
		permlist[0].datum[i] = i;
	};

	for (unsigned int i = 1;i < fac; ++i)//从后面取到一个比这个数字大的数
	{
		bool havegot=0;
		int minp=0;
		int minindex=-1;
		int j=0;
		for(j = outerdim - 2;j >= 0;--j)
		{

			for(int k = j + 1; k < outerdim; ++k)
			{
				if(permlist[i-1].datum[j]<permlist[i-1].datum[k])
				{
					havegot = 1;
					minp = permlist[i-1].datum[k];
					minindex = k;
				}//如果j后面有数字比这个数字大找到最小的那个，交换，后对i+1___outerdimension排序
				if(havegot==1&&permlist[i-1].datum[k]>permlist[i-1].datum[j]&&permlist[i-1].datum[k]<minp)
				{
					minindex = k;
					minp = permlist[i-1].datum[k];
				}
			}
			if(havegot == 1)break;
		}
		for(int s = 0;s < outerdim; ++s)
		{
			permlist[i].datum[s] = permlist[i-1].datum[s];
		}
		int temp;
		temp = permlist[i].datum[j];
		permlist[i].datum[j] = permlist[i].datum[minindex];
		permlist[i].datum[minindex] = temp;
		for(int s = j + 1;s < outerdim; ++s)
		{
			int ind=s;
			int ss;
			for(ss= s + 1; ss < outerdim; ++ss)
			{
				if(permlist[i].datum[ss] < permlist[i].datum[ind])
					ind = ss;
			}
			int temp;
			if(ind!=s)
			{
				temp = permlist[i].datum[s];
				permlist[i].datum[s] = permlist[i].datum[ind];
				permlist[i].datum[ind] = temp;
			}
		}
	}
}