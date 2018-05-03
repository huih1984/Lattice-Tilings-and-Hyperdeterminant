/*对4*2的长方块进行行元素的列轮换的交换得到的形式进行计算*/
#include"stdafx.h"
#include<set>
#include<list>
#include<iostream>

#define SUBFUN
#ifndef SUBFUN
using namespace std;
int m,n,dimension;


struct Index{
	int w; int x;int y; int z; int vis;
	//重载“<”操作符，自定义排序规则
	bool operator < (const Index &a) const
	{
		//按score从大到小排列
		return a.w<w||(a.w==w&&a.x<x)||(a.w==w&&a.x==x&&a.y<y)||(a.w==w&&a.x==x&&a.y==y&&a.z<z);
	}
	void operator = (Index a)
	{
		//按score从大到小排列
		w=a.w;x=a.x;y=a.y;z=a.z;
	}
	bool operator == (const Index &a) const
	{
		//按score从大到小排列
		return a.w==w&&a.x==x&&a.y==y&&a.z==z;
	}
};

#define MAXN 80
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
		if(!(l1[(*iter).w]==0
			&&l2[(*iter).x]==0&&l3[(*iter).y]==0&&l4[(*iter).z]==0))continue;
		indexSystem[i][1]=(*iter).w;
		indexSystem[i][2]=(*iter).x;
		indexSystem[i][3]=(*iter).y;
		indexSystem[i][4]=(*iter).z;
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

int main(){
	freopen("out4.txt","w",stdout);
	int a11[2]={1,5},a21[2]={2,6},a31[2]={3,7},a41[2]={4,8};
	Index index;
	dimension = 8;
	for(int i=0;i<2;++i){
		for(int j=0;j<2;++j){
			for(int k=0;k<2;++k){
				for(int l=0;l<2;++l){
					index.w=a11[i];
					index.x=a21[j];
					index.y=a31[k];
					index.z=a41[l];
					indexSet[0].insert(index);
					int temp;
					temp = index.w;
					index.w=index.x;index.x=index.y;index.y=index.z;index.z=temp;
					indexSet[0].insert(index);
					temp = index.w;
					index.w=index.x;index.x=index.y;index.y=index.z;index.z=temp;
					indexSet[0].insert(index);
					temp = index.w;
					index.w=index.x;index.x=index.y;index.y=index.z;index.z=temp;
					indexSet[0].insert(index);
				}
			}
		}
	}
	int a12[2]={1,6},a22[2]={2,7},a32[2]={3,8},a42[2]={4,5};
	for(int i=0;i<2;++i){
		for(int j=0;j<2;++j){
			for(int k=0;k<2;++k){
				for(int l=0;l<2;++l){
					index.w=a12[i];
					index.x=a22[j];
					index.y=a32[k];
					index.z=a42[l];
					indexSet[0].insert(index);
					int temp;
					temp = index.w;
					index.w=index.x;index.x=index.y;index.y=index.z;index.z=temp;
					indexSet[0].insert(index);
					temp = index.w;
					index.w=index.x;index.x=index.y;index.y=index.z;index.z=temp;
					indexSet[0].insert(index);
					temp = index.w;
					index.w=index.x;index.x=index.y;index.y=index.z;index.z=temp;
					indexSet[0].insert(index);
				}
			}
		}
	}
	int a13[2]={1,7},a23[2]={2,8},a33[2]={3,5},a43[2]={4,6};
	for(int i=0;i<2;++i){
		for(int j=0;j<2;++j){
			for(int k=0;k<2;++k){
				for(int l=0;l<2;++l){
					index.w=a13[i];
					index.x=a23[j];
					index.y=a33[k];
					index.z=a43[l];
					indexSet[0].insert(index);
					int temp;
					temp = index.w;
					index.w=index.x;index.x=index.y;index.y=index.z;index.z=temp;
					indexSet[0].insert(index);
					temp = index.w;
					index.w=index.x;index.x=index.y;index.y=index.z;index.z=temp;
					indexSet[0].insert(index);
					temp = index.w;
					index.w=index.x;index.x=index.y;index.y=index.z;index.z=temp;
					indexSet[0].insert(index);
				}
			}
		}
	}
	int a14[2]={1,8},a24[2]={2,5},a34[2]={3,6},a44[2]={4,7};
	for(int i=0;i<2;++i){
		for(int j=0;j<2;++j){
			for(int k=0;k<2;++k){
				for(int l=0;l<2;++l){
					index.w=a14[i];
					index.x=a24[j];
					index.y=a34[k];
					index.z=a44[l];
					indexSet[0].insert(index);
					int temp;
					temp = index.w;
					index.w=index.x;index.x=index.y;index.y=index.z;index.z=temp;
					indexSet[0].insert(index);
					temp = index.w;
					index.w=index.x;index.x=index.y;index.y=index.z;index.z=temp;
					indexSet[0].insert(index);
					temp = index.w;
					index.w=index.x;index.x=index.y;index.y=index.z;index.z=temp;
					indexSet[0].insert(index);
				}
			}
		}
	}
	for(int i=1;i<=dimension;++i){
		set<Index>::iterator iter;
		iter=indexSet[0].begin();
		for(;!indexSet[0].empty()&&iter!=indexSet[0].end();++iter){
			if((*iter).w==i){
				indexSet[i].insert(*iter);
			}
		}
	}
	memset(l1,0,sizeof(l1));	
	memset(l2,0,sizeof(l2));	
	memset(l3,0,sizeof(l3));	
	memset(l4,0,sizeof(l4));
	cnt=0;
	dfs(1);
	printf("%d\n",cnt);
	fclose(stdin);  
	fclose(stdout);  
	return 0;
}
#endif