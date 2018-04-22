/*这个文件处理下表纵向遍历的方法进行求解，并用set进行集合的处理*/
#include<set>
#include<list>
#include<iostream>
#include"stdafx.h"
using namespace std;

#define SUBFUN
#ifndef SUBFUN

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

int m,n,dimension;

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

bool notreg(){
	int indexSystemTemp[MAXN][5];
	memcpy(indexSystemTemp,indexSystem,sizeof(indexSystem));
	for(int j=1;j<=dimension;++j){
		for(int i=1;i<=4;++i){
			for(int k=i+1;k<=4;++k){
				if(indexSystemTemp[j][k]<
					indexSystemTemp[j][i]){
						int temp=indexSystemTemp[j][k];
						indexSystemTemp[j][k]=indexSystemTemp[j][i];
						indexSystemTemp[j][i]=temp;
				}
			}
		}
	}
	for(int j=1;j<=dimension;++j){
		for(int k=j+1;k<=dimension;++k){
			bool eq=true;
			for(int i=1;i<=4;++i){
				if(indexSystemTemp[j][i]!=indexSystemTemp[k][i]){
					eq=false;
				}
			}
			if(eq){
				return true;
			}

		}
	}
	return false;
}

int dfs(int i){	
	if(i==dimension+1){
		if(!notreg()){
			for(int i=1;i<=4;++i){
				for(int j=1;j<=dimension;++j){
					printf("%2d ", indexSystem[j][i]);
				}
				printf("\n");
			}
			printf("\n");
			cnt+=get_sign(indexSystem);
		}
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

void insertAll24(Index index){
	int i[5];
	i[1]=index.w;
	i[2]=index.x;
	i[3]=index.y;
	i[4]=index.z;
	for(int s=1;s<=4;++s){
		for(int ss=1;ss<=4;++ss){
			for(int sss=1;sss<=4;++sss){
				for(int ssss=1;ssss<=4;++ssss){
					if(i[ss]!=i[s]&&i[sss]!=i[ss]&&i[sss]!=i[s]&&i[ssss]!=i[sss]&&i[ssss]!=i[ss]&&i[ssss]!=i[s]){
						index.w=i[s];
						index.x=i[ss];
						index.y=i[sss];
						index.z=i[ssss];
						indexSet[0].insert(index);
					}
				}                                                                                                                                                                                                                                                                                                                                                                                                                                                          
			}
		}
	}
}

void insertAll4(Index index)
{
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

int main(){
	freopen("out3.txt","w",stdout);
	dimension=8;
	int temp;
	Index index;
	/*index.w=1;index.x=5;index.y=3;index.z=4;
	insertAll24(index);
	index.w=2;index.x=6;index.y=7;index.z=8;
	insertAll24(index);
	index.w=6;index.x=1;index.y=3;index.z=4;
	insertAll24(index);
	index.w=8;index.x=7;index.y=5;index.z=2;
	insertAll24(index);
	index.w=1;index.x=7;index.y=3;index.z=4;
	insertAll24(index);
	index.w=2;index.x=6;index.y=8;index.z=5;
	insertAll24(index);
	index.w=1;index.x=8;index.y=3;index.z=4;
	insertAll24(index);
	index.w=2;index.x=5;index.y=6;index.z=7;
	insertAll24(index);*/
	index.w=1;index.x=3;index.y=4;index.z=5;
	insertAll4(index);
	index.w=2;index.x=5;index.y=6;index.z=7;
	insertAll4(index);
	index.w=3;index.x=1;index.y=7;index.z=4;
	insertAll4(index);
	index.w=4;index.x=6;index.y=3;index.z=1;
	insertAll4(index);
	index.w=5;index.x=8;index.y=2;index.z=6;
	insertAll4(index);
	index.w=6;index.x=7;index.y=8;index.z=2;
	insertAll4(index);
	index.w=7;index.x=2;index.y=5;index.z=8;
	insertAll4(index);
	index.w=8;index.x=4;index.y=1;index.z=3;
	insertAll4(index);
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