/*这个文件处理下表纵向遍历的方法进行求解，并用set进行集合的处理*/
#include "StdAfx.h"
#include<set>
#include<list>
#include<iostream>
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

using namespace std;
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

int indexi[MAXN];
int indexj[MAXN];
int indexk[MAXN];
int indexl[MAXN];
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
		if((*iter).w!=i||l1[(*iter).w]==1
			||l2[(*iter).x]==1||l3[(*iter).y]==1||l4[(*iter).z]==1)continue;
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

int f1(){
	int m1=m%4;
	if(m1==0){
		return 0;
	}else return 1;
}

int f2(){
	int m1=m%4;
	if(m1<=1){
		return 0;
	}else return 1;
}

int f3(){
	int m1=m%4;
	if(m1<=2){
		return 0;
	}else return 1;
}

int mainnn(){
	freopen("data1.txt","r",stdin);
	freopen("out1.txt","w",stdout);
	while(scanf("%d %d",&m,&n)!=EOF&&m&&n){
		dimension = m*n;
		int temp;
		for(int i=0;i<MAXN;++i){
			indexSet[i].clear();
		}
		Index index;
		int i=0,j=0,k=0,l=0;
		for(int i1=0;i1<=m/4+f1()-1;++i1){
			for(int j1=0;j1<=n/4-1;++j1){

				indexi[i++]=1+ 16*i1*n/4+4*j1;
				indexj[j++]=2+ 16*i1*n/4+4*j1;
				indexk[k++]=3+ 16*i1*n/4+4*j1;
				indexl[l++]=4+ 16*i1*n/4+4*j1;

			}
		}

		for(int i2=0;i2<=m/4+f2()-1;++i2){
			for(int j2=0;j2<=n/4-1;++j2){
				indexj[j++]=1+n+ 16*i2*n/4+4*j2;
				indexi[i++]=4+n+ 16*i2*n/4+4*j2;
				indexk[k++]=2+n+ 16*i2*n/4+4*j2;
				indexl[l++]=3+n+ 16*i2*n/4+4*j2;

			}
		}
		for(int i3=0;i3<=m/4+f3()-1;++i3){
			for(int j3=0;j3<=n/4-1;++j3){

				indexj[j++]=4+2*n+ 16*i3*n/4+4*j3;
				indexi[i++]=3+2*n+ 16*i3*n/4+4*j3;
				indexk[k++]=1+2*n+ 16*i3*n/4+4*j3;
				indexl[l++]=2+2*n+ 16*i3*n/4+4*j3;
			}
		}
		for(int i4=0;i4<=m/4-1;++i4){
			for(int j4=0;j4<=n/4-1;++j4){
				indexj[j++]=3+3*n+ 16*i4*n/4+4*j4;
				indexi[i++]=2+3*n+ 16*i4*n/4+4*j4;
				indexk[k++]=4+3*n+ 16*i4*n/4+4*j4;
				indexl[l++]=1+3*n+ 16*i4*n/4+4*j4;
			}
		}

		for(i=0;i<dimension/4;++i){
			for(j=0;j<dimension/4;++j){
				for(k=0;k<dimension/4;++k){
					for(l=0;l<dimension/4;++l){
						index.w=indexi[i];
						index.x=indexj[j];
						index.y=indexk[k];
						index.z=indexl[l];
						indexSet[0].insert(index);
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

		//for(int i=0;i<=(m-4)*n;i=i+n){
		//	for(int j=1;j<=n;++j){
		//		index.w=i+j;
		//		index.x=i+j+1*n;
		//		index.y=i+j+2*n;
		//		index.z=i+j+3*n;
		//		indexSet[0].insert(index);
		//		temp = index.w;
		//		index.w=index.x;index.x=index.y;index.y=index.z;index.z=temp;
		//		indexSet[0].insert(index);
		//		temp = index.w;
		//		index.w=index.x;index.x=index.y;index.y=index.z;index.z=temp;
		//		indexSet[0].insert(index);
		//		temp = index.w;
		//		index.w=index.x;index.x=index.y;index.y=index.z;index.z=temp;
		//		indexSet[0].insert(index);
		//		
		//		index.w=i+j+1;
		//		index.x=i+j+1*n-1;
		//		index.y=i+j+2*n-1;
		//		index.z=i+j+3*n-1;
		//		indexSet[0].insert(index);
		//		temp = index.w;
		//		index.w=index.x;index.x=index.y;index.y=index.z;index.z=temp;
		//		indexSet[0].insert(index);
		//		temp = index.w;
		//		index.w=index.x;index.x=index.y;index.y=index.z;index.z=temp;
		//		indexSet[0].insert(index);
		//		temp = index.w;
		//		index.w=index.x;index.x=index.y;index.y=index.z;index.z=temp;
		//		indexSet[0].insert(index);
		//	}
		//}

		for(int i=1;i<=dimension;++i){
			set<Index>::iterator iter;
			iter=indexSet[0].begin();
			for(;!indexSet[0].empty()&&iter!=indexSet[0].end();++iter){
				if((*iter).w==i){
					indexSet[i].insert(*iter);
				}
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