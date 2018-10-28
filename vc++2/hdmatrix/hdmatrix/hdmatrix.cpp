/*下标纵向遍历的方法进行求解，并用set进行集合的处理*/
#include"stdafx.h"
#include<set>
#include<list>
#include<iostream>

//#define SUBFUN
#ifndef SUBFUN
using namespace std;
int m,n,dimension;

#define MAXN 80

struct Index{
	int cols[4];
	int vis;
	//重载“<”操作符，自定义排序规则
	bool operator < (const Index &a) const
	{
		//按score从大到小排列
		return a.cols[0]<cols[0]||(a.cols[0]==cols[0]&&a.cols[1]<cols[1])||(a.cols[0]==cols[0]&&a.cols[1]==cols[1]&&a.cols[2]<cols[2])||(a.cols[0]==cols[0]&&a.cols[1]==cols[1]&&a.cols[2]==cols[2]&&a.cols[3]<cols[3]);
	}
	void operator = (Index a)
	{
		//按score从大到小排列
		cols[0] = a.cols[0];
		cols[1] = a.cols[1];
		cols[2] = a.cols[2];
		cols[3] = a.cols[3];
	}
	bool operator == (const Index &a) const
	{
		//按score从大到小排列
		return a.cols[0]==cols[0]&&a.cols[1]==cols[1]&&a.cols[2]==cols[2]&&a.cols[3]==cols[3];
	}
};

set<Index> indexSet[MAXN];
set<Index> indexSet2[MAXN];
int tot;
int indexSystem[MAXN][5];
int l1[MAXN];
int l2[MAXN];
int l3[MAXN];
int l4[MAXN];
int cnt=0;

int get_sign(int permdata[][5])
{
	short j,k;
	int sign;
	sign = 1;
	for(int i=1;i<=4;++i){
		for(j = 1; j <= dimension ; ++j)
		{
			for(k = j + 1; k <= dimension; ++k)
			{
				if(permdata[k][i] > permdata[j][i])
				{
					sign *= -1;
				}
			}
		}
	}
	for(j = 1; j <= dimension ; ++j)
	{
		for(int i=1;i<=4;++i){
			for(k = i + 1; k <=4; ++k)
			{
				if(permdata[j][k] > permdata[j][i])
				{
					sign *= -1;
				}
			}
		}
	}
	return sign;
}

int dfs(int i){	
	if(i==dimension+1){
		for(int i=1;i<=4;++i){
			for(int j=1;j<=dimension;++j){
				printf("%2d ", indexSystem[j][i]);
			}
			printf("\n");
		}
		printf("\n");
		cnt+=get_sign(indexSystem);
		return 0;
	}

	set<Index>::iterator iter;
	iter=indexSet[i].begin();
	for(;!indexSet[i].empty()&&iter!=indexSet[i].end();++iter){
		if(!(l1[(*iter).cols[0]]==0
			&&l2[(*iter).cols[1]]==0&&l3[(*iter).cols[2]]==0&&l4[(*iter).cols[3]]==0))continue;
		indexSystem[i][1]=(*iter).cols[0];
		indexSystem[i][2]=(*iter).cols[1];
		indexSystem[i][3]=(*iter).cols[2];
		indexSystem[i][4]=(*iter).cols[3];
		l1[indexSystem[i][1]]=1;
		l2[indexSystem[i][2]]=1;
		l3[indexSystem[i][3]]=1;
		l4[indexSystem[i][4]]=1;
		dfs(i+1);
		l1[indexSystem[i][1]]=0;
		l2[indexSystem[i][2]]=0;
		l3[indexSystem[i][3]]=0;
		l4[indexSystem[i][4]]=0;
	}
	return 0;
}


int dfs2(int i){	
	if(i==dimension/2+1){
		for(int i=1;i<=4;++i){
			for(int j=1;j<=dimension/2;++j){
				printf("%2d ", indexSystem[j][i]);
			}
			printf("\n");
		}
		printf("\n");
		cnt+=get_sign(indexSystem);
		return 0;
	}

	set<Index>::iterator iter;
	iter=indexSet[0].begin();
	advance(iter, i);
	for(;!indexSet[0].empty()&&iter!=indexSet[0].end();++iter){
		if (i >= 1 && (*iter).cols[0] > indexSystem[i - 1][1]) continue;
		if(!(l1[(*iter).cols[0]]==0
			&&l1[(*iter).cols[1]]==0&&l2[(*iter).cols[2]]==0&&l2[(*iter).cols[3]]==0))continue;
		indexSystem[i][1]=(*iter).cols[0];
		indexSystem[i][2]=(*iter).cols[1];
		indexSystem[i][3]=(*iter).cols[2];
		indexSystem[i][4]=(*iter).cols[3];
		l1[indexSystem[i][1]]=1;
		l1[indexSystem[i][2]]=1;
		l2[indexSystem[i][3]]=1;
		l2[indexSystem[i][4]]=1;
		dfs(i+1);
		l1[indexSystem[i][1]]=0;
		l1[indexSystem[i][2]]=0;
		l2[indexSystem[i][3]]=0;
		l2[indexSystem[i][4]]=0;
	}
	return 0;
}

Index get_tran_(Index index, int tuple[], int len){
	Index new_index = index;
	for (int i=0; i < len - 1; ++i)
	{
		new_index.cols[tuple[i]] = index.cols[tuple[i+1]];
	}
	new_index.cols[tuple[len - 1]] = index.cols[tuple[0]];
	return new_index;
}

void set_index_set()
{
	Index index;
	for(int i=0;i<=m-1;++i){
		for(int j=1;j<=n-3;++j){
			index.cols[0]=i*n+j;
			index.cols[1]=i*n+j+1;
			index.cols[2]=i*n+j+2;
			index.cols[3]=i*n+j+3;
			indexSet[0].insert(index);
			/*int tuple[8][3] = {{0,1,2},{0,2,1},{0,1,3},{0,3,1},{0,2,3},{0,3,2},{1,2,3},{1,3,2}};
			for (int tuple_index=0; tuple_index < 8; ++tuple_index){
				indexSet[0].insert(get_tran_(index, tuple[tuple_index], 3));
			}*/
			int tuple2[3][2] = {{0,1},{0,2},{0,3}};
			int tuple3[3][2] = {{2,3},{1,3},{1,2}};
			for (int tuple_index=0; tuple_index < 3; ++tuple_index){
				Index temp_index = get_tran_(index, tuple2[tuple_index], 2);
				indexSet[0].insert(get_tran_(temp_index, tuple3[tuple_index], 2));
			}
			/*int tuple4[6][2] = {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3}};
			for (int tuple_index=0; tuple_index < 6; ++tuple_index){
				indexSet[0].insert(get_tran_(index, tuple4[tuple_index], 2));
			}*/
			//int tuple5[6][4] = {{0,1,2,3},{0,1,3,2},{0,2,1,3},{0,2,3,1},{0,3,1,2},{0,3,2,1}};
			/*int tuple5[2][4] = {{0,1,3,2},{0,2,3,1}};
			for (int tuple_index=0; tuple_index < 2; ++tuple_index){
				indexSet[0].insert(get_tran_(index, tuple5[tuple_index], 4));
			}*/
		}
	}
	for(int i=0;i<=m-4;++i){
		for(int j=1;j<=n;++j){
			index.cols[0]=i*n+j;
			index.cols[1]=i*n+j+1*n;
			index.cols[2]=i*n+j+2*n;
			index.cols[3]=i*n+j+3*n;
			indexSet[0].insert(index);
			/*int tuple[8][3] = {{0,1,2},{0,2,1},{0,1,3},{0,3,1},{0,2,3},{0,3,2},{1,2,3},{1,3,2}};
			for (int tuple_index=0; tuple_index < 8; ++tuple_index){
				indexSet[0].insert(get_tran_(index, tuple[tuple_index], 3));
			}*/
			int tuple2[3][2] = {{0,1},{0,2},{0,3}};
			int tuple3[3][2] = {{2,3},{1,3},{1,2}};
			for (int tuple_index=0; tuple_index < 3; ++tuple_index){
				Index temp_index = get_tran_(index, tuple2[tuple_index], 2);
				indexSet[0].insert(get_tran_(temp_index, tuple3[tuple_index], 2));
			}
			/*int tuple4[6][2] = {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3}};
			for (int tuple_index=0; tuple_index < 6; ++tuple_index){
				indexSet[0].insert(get_tran_(index, tuple4[tuple_index], 2));
			}*/
			//int tuple5[6][4] = {{0,1,2,3},{0,1,3,2},{0,2,1,3},{0,2,3,1},{0,3,1,2},{0,3,2,1}};
			/*int tuple5[2][4] = {{0,1,3,2},{0,2,3,1}};
			for (int tuple_index=0; tuple_index < 2; ++tuple_index){
				indexSet[0].insert(get_tran_(index, tuple5[tuple_index], 4));
			}*/
		}
	}
	for(int i=1;i<=dimension;++i){
		set<Index>::iterator iter;
		iter=indexSet[0].begin();
		for(;!indexSet[0].empty()&&iter!=indexSet[0].end();++iter){
			if((*iter).cols[0]==i){
				indexSet[i].insert(*iter);
			}
		}
	}
}


void set_index_set2()
{
	Index index;
	for(int i=0;i<=m-1;++i){
		for(int j=1;j<=n-3;++j){
			index.cols[0]=i*n+j;
			index.cols[1]=i*n+j+1;
			index.cols[2]=i*n+j+2;
			index.cols[3]=i*n+j+3;
			indexSet[0].insert(index);
			int tuple2[1][2] = {{0,2}};
			int tuple3[1][2] = {{1,3}};
			for (int tuple_index=0; tuple_index < 1; ++tuple_index){
				Index temp_index = get_tran_(index, tuple2[tuple_index], 2);
				indexSet[0].insert(get_tran_(temp_index, tuple3[tuple_index], 2));
			}
		}
	}
	for(int i=0;i<=m-4;++i){
		for(int j=1;j<=n;++j){
			index.cols[0]=i*n+j;
			index.cols[1]=i*n+j+1*n;
			index.cols[2]=i*n+j+2*n;
			index.cols[3]=i*n+j+3*n;
			indexSet[0].insert(index);
			int tuple2[1][2] = {{0,2}};
			int tuple3[1][2] = {{1,3}};
			for (int tuple_index=0; tuple_index < 1; ++tuple_index){
				Index temp_index = get_tran_(index, tuple2[tuple_index], 2);
				indexSet[0].insert(get_tran_(temp_index, tuple3[tuple_index], 2));
			}
		}
	}
	set<Index>::iterator iter;
	iter=indexSet[0].begin();
	for(;!indexSet[0].empty()&&iter!=indexSet[0].end();++iter){
		cout<< (*iter).cols[0]<<(*iter).cols[1]<<(*iter).cols[2]<<(*iter).cols[3]<<endl;
	}
}

int main(){
	freopen("data1.txt","r",stdin);
	freopen("out.txt","w",stdout);
	scanf("%d%d", &m, &n);
	dimension = m*n;
	int temp;
	
	memset(l1,0,sizeof(l1));	
	memset(l2,0,sizeof(l2));	
	memset(l3,0,sizeof(l3));	
	memset(l4,0,sizeof(l4));
	cnt=0;
	set_index_set2();
	dfs2(1);
	printf("%d\n",cnt);
	fclose(stdin);  
	fclose(stdout);  
	return 0;
}
#endif