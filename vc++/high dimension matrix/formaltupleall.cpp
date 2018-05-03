//#include"stdafx.h"
//#include<fstream>
//using namespace std;
//#define offset 20
//int main(){
//	FILE* fp = fopen("regulartuple.txt","w"); 
//	fstream stream("tupleall.txt", ios::in|ios::binary);
//	int a[offset][4][8];
//	string val[offset];
//	int index=0;
//	while(!stream.eof()){
//		if(index==offset){
//			for(int i=0;i<4;++i){
//				for(int k=0;k<offset;++k){
//					for(int j=0;j<8;++j){
//						fprintf(fp,"%d",a[k][i][j]+1);
//					}
//					fprintf(fp," ");
//				}
//				fprintf(fp,"\n");
//			}
//			for(int k=0;k<offset;++k){
//				fprintf(fp,"%s",val[k].c_str());
//			}
//			fprintf(fp,"\n");
//			index=0;
//		}
//		string   line;  
//		getline(stream,   line);  
//		for(int i=0;i<4;++i){
//			for(int j=0;j<8;++j){
//				char temp=line.at(((j*4)+i));
//				a[index][i][j]=temp-'0';
//			}
//		}
//		const char * p=&line.c_str()[32];
//		val[index].clear();
//		val[index].append(p);
//		val[index].erase(val[index].end()-1);
//		index++;
//
//		//cout   <<   line;   
//	}
//	fclose(fp);
//	return 0;
//}
