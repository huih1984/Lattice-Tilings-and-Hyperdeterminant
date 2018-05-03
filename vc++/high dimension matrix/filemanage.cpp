
#include "stdafx.h"
#include"filemanage.h"
#include "time.h" 
char charnum[9]="0000000";
const int filescount=5;
__int64 newfilesize=0;
int dwblocksize=1000;
//当文件作为被比较文件时获得它的长度，以后修改时只是修改后面的数字而并不改变长度
//采用二分法查找，修改数字，并操作比较文件
//每次操作需要提取文件2的一行，文件1则要二分遍历，所以要提供一个函数根据文件名和字段得到位置
basenum getfilestring(FILE* fpname,int i)
{
	ASSERT(fpname);
	basl val;
	basenum bas,*pbas;
	pbas=&bas;
	rewind(fpname);
	fseek(fpname,i*43,0);
	fread(pbas,40,1,fpname);
	return bas;
}

int getfilelen(FILE* fpname)
{
	fseek(fpname,0,SEEK_END);
	return ftell(fpname)/43;    //return NULL;
}
int bigger(basenum a,basenum b)
{
	for(int i=0;i<32;i++)
	{
		if(a.g[i]<b.g[i])
			return -1;
		else if(a.g[i]>b.g[i])
			return 1;
	}
	return 0;
}
bool findposition(basenum*destp,int starti,int filelen,basenum sourcebase,int & placei)//如果为真则将placei返回的是合并的位置，返回为假，在placei后插入数字
{

	int midnum=0;
	basenum basleft,*pbasleft,basright,*pbasright;
	pbasleft=&basleft;
	pbasright=&basright;
	filelen--;
	while(starti<filelen)
	{
		/*rewind(destp);
		fseek(fpname,starti*43,0);
		fread(pbasleft,40,1,fpname);
		rewind(destp);
		fseek(fpname,filelen*43,0);
		fread(pbaseright,40,1,fpname);*/
		if(bigger(basleft,sourcebase)==1)
		{
		}
	}
	return false;
}
void refinefile(char *destfile,char *sourcefile)
{
	FILE*pdest=fopen(destfile,"r");
	FILE*psource=fopen(sourcefile,"r");
	int destlen=getfilelen(pdest);
	int sourcelen=getfilelen(psource);
	basenum *desthead=new basenum[destlen];
	rewind(pdest);
	fread(desthead,43,destlen,pdest);


	basenum *sourcehead=new basenum[sourcelen];
	rewind(psource);
	fread(sourcehead,43,sourcelen,psource);

	long h;
	char *endstr;
	int starti=0,placei;
	for(int i=0;i<sourcelen;i++)
	{

		h=strtol(sourcehead[i].h,&endstr,10);
		if(h!=0)
		{
			if(findposition(desthead,starti,sourcelen,sourcehead[i],placei)==true)
			{

			}
			//将sourcehead[i]中的值加给desthead【placei】，将placei+1赋给starti
		}
	}
	fclose(pdest);fclose(psource);
}

void compareFiles(char* filename[],int length){
	//if(!filename||length<=0){
	//	return;
	//}

	//FILE files[] = new FILE[length];
	//FILE result = new FILE("result.txt");

	/*for(int i=0;i<length;i++){
	files[i] = fopen(filename[i],"r");
	}*/

}



HANDLE getfilemaphandle(LPCWSTR strfile, DWORD &dwBlockBytes,__int64 &qwFileSize, char *&lpbMapAddress)
{
	HANDLE hFile = ::CreateFile(strfile, GENERIC_READ| GENERIC_WRITE,0, NULL, 
		OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		TRACE("创建文件对象失败,错误代码:%d\r\n", GetLastError());
		return NULL;
	}
	// 创建文件映射对象
	HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	if (hFileMap == NULL)
	{
		TRACE("创建文件映射对象失败,错误代码:%d\r\n", GetLastError());  
		return NULL;
	}
	// 得到系统分配粒度
	//SYSTEM_INFO SysInfo;
	//GetSystemInfo(&SysInfo);
	// dwGran = SysInfo.dwAllocationGranularity;
	// 得到文件尺寸
	DWORD dwFileSizeHigh;
	qwFileSize = GetFileSize(hFile, &dwFileSizeHigh);
	//qwFileSize |= (((__int64)dwFileSizeHigh) << 32);///MSDN
	// 偏移地址 
	__int64 qwFileOffset = 0;
	//dwBlockBytes = 43*10000;//文件数据分段大小
	//if (qwFileSize - qwFileOffset < dwBlockBytes)
	//	dwBlockBytes = (DWORD)qwFileSize;
	dwBlockBytes = (DWORD)qwFileSize;
	// 映射视图
	lpbMapAddress = (char *)MapViewOfFile(hFileMap,FILE_MAP_ALL_ACCESS,
		(DWORD)(qwFileOffset >> 32), (DWORD)(qwFileOffset & 0xFFFFFFF),dwBlockBytes);
		//	lpbMapAddress = (char *)MapViewOfFile(hFileMap,FILE_MAP_WRITE,
		//0,0,43*1000);
	if (lpbMapAddress == NULL)
	{
		TRACE("映射文件映射失败,错误代码:%d ", GetLastError());
		return NULL;
	} 
	// 关闭文件对象
	CloseHandle(hFile); 
	return hFileMap;
}
HANDLE getfilemaphandle(LPCWSTR strfile, DWORD &dwBlockBytes,__int64 &qwFileSize)
{
	HANDLE hFile = ::CreateFile(strfile, GENERIC_READ| GENERIC_WRITE,0, NULL, 
		OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		TRACE("创建文件对象失败,错误代码:%d\r\n", GetLastError());
		return NULL;
	}
	// 创建文件映射对象
	HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	if (hFileMap == NULL)
	{
		TRACE("创建文件映射对象失败,错误代码:%d\r\n", GetLastError());  
		return NULL;
	}
	// 得到文件尺寸
	DWORD dwFileSizeHigh;
	qwFileSize = GetFileSize(hFile, &dwFileSizeHigh);
	// 关闭文件对象
	CloseHandle(hFile); 
	return hFileMap;
}
char* getMapviewoffile(HANDLE hFileMap,__int64 qwFileOffset,DWORD& dwBlockBytes,__int64 qwFileSize)
{
	char*lpbMapAddress;
	//qwFileSize |= (((__int64)dwFileSizeHigh) << 32);///MSDN
	// 偏移地址 

	if (qwFileSize - qwFileOffset < dwBlockBytes)
		dwBlockBytes = (DWORD)qwFileSize-qwFileOffset;
	// 映射视图
	lpbMapAddress = (char *)MapViewOfFile(hFileMap,FILE_MAP_ALL_ACCESS,
		(DWORD)(qwFileOffset >> 32), (DWORD)(qwFileOffset & 0xFFFFFFF),dwBlockBytes);
	/*	lpbMapAddress = (char *)MapViewOfFile(hFileMap,FILE_MAP_WRITE,
	0,0,dwBlockBytes);*/
	if (lpbMapAddress == NULL)
	{
		TRACE("映射文件映射失败,错误代码:%d ", GetLastError());
		return NULL;
	} 
	return lpbMapAddress;
}

char* getnewMapviewoffile(HANDLE hFileMap,__int64 &qwFileOffset,DWORD dwBlockBytes,__int64 qwFileSize)
{
	char*lpbMapAddress;
	//qwFileSize |= (((__int64)dwFileSizeHigh) << 32);///MSDN
	// 偏移地址 

	//dwBlockBytes = 43*1000;//文件数据分段大小
	if (qwFileSize - qwFileOffset < dwBlockBytes)
		dwBlockBytes = (DWORD)qwFileSize-qwFileOffset;
	// 映射视图
	lpbMapAddress = (char *)MapViewOfFile(hFileMap,FILE_MAP_ALL_ACCESS,
		(DWORD)(qwFileOffset >> 32), (DWORD)(qwFileOffset & 0xFFFFFFF),dwBlockBytes);
	/*	lpbMapAddress = (char *)MapViewOfFile(hFileMap,FILE_MAP_WRITE,
	0,0,dwBlockBytes);*/
	if (lpbMapAddress == NULL)
	{
		TRACE("映射文件映射失败,错误代码:%d ", GetLastError());
		return NULL;
	} 
	return lpbMapAddress;
}

HANDLE getfilemaphandle(HANDLE &hFile,LPCWSTR strfile, DWORD &dwBlockBytes,__int64 &qwFileSize, char *&lpbMapAddress)
{
	hFile = ::CreateFile(strfile, GENERIC_READ| GENERIC_WRITE,0, NULL, 
		OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		TRACE("创建文件对象失败,错误代码:%d\r\n", GetLastError());
		return NULL;
	}


	// 创建文件映射对象
	HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	if (hFileMap == NULL)
	{
		TRACE("创建文件映射对象失败,错误代码:%d\r\n", GetLastError());  
		return NULL;
	}
	// 得到系统分配粒度
	//SYSTEM_INFO SysInfo;
	//GetSystemInfo(&SysInfo);
	// dwGran = SysInfo.dwAllocationGranularity;
	// 得到文件尺寸
	DWORD dwFileSizeHigh;
	qwFileSize = GetFileSize(hFile, &dwFileSizeHigh);
	qwFileSize |= (((__int64)dwFileSizeHigh) << 32);///MSDN
	// 偏移地址 
	__int64 qwFileOffset = 0;
	dwBlockBytes = 64*1024;//文件数据分段大小
	if (qwFileSize - qwFileOffset < dwBlockBytes)
		dwBlockBytes = (DWORD)qwFileSize;
	dwBlockBytes = (DWORD)qwFileSize;
	// 映射视图
	lpbMapAddress = (char *)MapViewOfFile(hFileMap,FILE_MAP_ALL_ACCESS,
		(DWORD)(qwFileOffset >> 32), (DWORD)(qwFileOffset & 0xFFFFFFFF),dwBlockBytes);
	if (lpbMapAddress == NULL)
	{
		TRACE("映射文件映射失败,错误代码:%d ", GetLastError());
		return NULL;
	} 
	return hFileMap;
}
__int64 getallfilesize(handle han[])
{
	__int64 val=0;
	for(int i=0;i<filescount;i++)
	{
		val=val+han[i].qwFileSize;
	}
	return val;
}
HANDLE getnewfilemaphandle( HANDLE &hFile,  LPCWSTR strfile, char *&lpbMapAddress,__int64 qwFileSize)
{
	hFile = ::CreateFile(strfile, GENERIC_READ| GENERIC_WRITE,0, NULL, 
		CREATE_ALWAYS, FILE_FLAG_RANDOM_ACCESS, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		TRACE("创建文件对象失败,错误代码:%d\r\n", GetLastError());
		return NULL;
	}
	// 创建文件映射对象
	HANDLE hFileMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE, 
		0, qwFileSize, NULL);
	if (hFileMap == NULL)
	{
		TRACE("创建文件映射对象失败,错误代码:%d\r\n", GetLastError());  
		return NULL;
	}
	// 映射视图
	lpbMapAddress = (char *)MapViewOfFile(hFileMap,FILE_MAP_WRITE,
		0,0,0);
	if (lpbMapAddress == NULL)
	{
		TRACE("映射文件映射失败,错误代码:%d ", GetLastError());
		return NULL;
	} 
	return hFileMap;
}
HANDLE getnewfilemaphandle( HANDLE &hFile,  LPCWSTR strfile, __int64 qwFileSize)
{
	hFile = ::CreateFile(strfile, GENERIC_READ| GENERIC_WRITE,0, NULL, 
		CREATE_ALWAYS, FILE_FLAG_RANDOM_ACCESS, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		TRACE("创建文件对象失败,错误代码:%d\r\n", GetLastError());
		return NULL;
	}
	// 创建文件映射对象
	HANDLE hFileMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE, 
		(DWORD)(qwFileSize >> 32), (DWORD)(qwFileSize & 0xFFFFFFFF), NULL);
	if (hFileMap == NULL)
	{
		TRACE("创建文件映射对象失败,错误代码:%d\r\n", GetLastError());  
		return NULL;
	}
	// 映射视图
	return hFileMap;
}

int smaller(char*lpmapaddress1,char*lpmapaddress2)
{
	for(int i=0;i<32;i++)
	{
		if(*(lpmapaddress1+i)<*(lpmapaddress2+i))
			return 1;
		else if(*(lpmapaddress1+i)>*(lpmapaddress2+i))
			return -1;
	}
	return 0;
}

bool getminchar(handle han[],int minfilesindex[],int minfilesnum ,basenum& val)
{
	char charnum[8];
	long h=0;char *endstr;
	for(int i=0;i<minfilesnum;i++)
	{
		for(int j=0;j<8;j++)
		{
			charnum[j]=*(han[minfilesindex[i]].lpbMapAddress+32+j);
		}
		h=h+strtol(charnum,&endstr,10);
	}
	if(h==0)
	{

		return true;
	}
	for(int i=0;i<32;i++)
	{
		val.g[i]=*(han[minfilesindex[0]].lpbMapAddress+i);
	}
	sprintf_s(val.h,"%8d",h);
	return false;
}
void writecharinnewfile(char*& MapAddress,basenum newval)
{
	for(int i=0;i<40;i++)
		*(MapAddress++)=newval.g[i];
	*(MapAddress++)=' ';
	*(MapAddress++)=13;
	*(MapAddress++)=10;
}
//bool writefileonmapaddresses(handle han[],HANDLE newfilehandle,char *newfileaddress)
/////////////读文件数据
//{
//	/********************            读文件             ***************************/  
//	//read_eh(&lpbMapAddress)读取已映射到内存的数据，
//	//并将文件指针作相应后移(lpbMapAddress++),返回指针偏移量
//	//qwFileOffset = qwFileOffset + read_eh(&lpbMapAddress); //修改偏移量
//	bool retval=true;
//	int filebeginindex[filescount];
//	int minfileindex=0;
//	int minfilesindex[filescount];
//	int minfilesnum=0;
//	for(int i=0;i<filescount;i++)
//	{
//		filebeginindex[i]=0;
//	}
//	int minfirstindex=0;
//	while(1)
//	{
//		retval=true;
//		bool isfirst=true;
//		for(int i=0;i<filescount;i++)
//		{
//			if(filebeginindex[i]==han[i].qwFileSize/43)
//			{
//				continue;
//			}
//			if(isfirst==true)
//			{
//				minfirstindex=i;
//				minfilesnum=1;
//				minfilesindex[0]=i;
//				minfileindex=i;
//				isfirst=false;
//				continue;
//			}
//			//if(LPVOID(0x026b200b)==LPVOID(han[i].lpbMapAddress))
//			//{
//			//	break;
//			//}
//			if(i!=minfileindex)
//			{
//				int compareval=smaller(han[i].lpbMapAddress,han[minfileindex].lpbMapAddress);
//				if(compareval==1)
//				{
//					minfileindex=i;
//					minfilesnum=1;
//					minfilesindex[minfilesnum-1]=i;
//				}
//				else if(compareval==0)
//				{
//					minfilesnum++;
//					minfilesindex[minfilesnum-1]=i;
//				}
//			}
//			else
//			{
//				minfilesnum=1;
//				minfilesindex[minfilesnum-1]=i;
//			}
//		}
//		//bool valisnull=false;
//		//basenum newval;
//		//		char comp[]="01230123012301452367456745674567";
//		//bool isf=true;
//		//for(int i=0;i<32;i++)
//		//{
//		//	//if(newval.g[i]!=comp[i])
//		//	if(*(han[minfilesindex[0]].lpbMapAddress+i)!=comp[i])
//		//	{
//		//	isf=false;
//		//	}
//		//}
//		//if(isf==true)
//		//{
//		//	isf=false;
//		//}
//
//
//		long h=0;char *endstr;
//		for(int i=0;i<minfilesnum;i++)
//		{
//			for(int j=0;j<8;j++)
//			{
//				charnum[j]=*(han[minfilesindex[i]].lpbMapAddress+32+j);
//			}
//			h=h+strtol(charnum,&endstr,10);
//		}
//		if(h!=0)
//		{
//			for(int i=0;i<32;i++)
//			{
//				*(newfileaddress++)=*(han[minfilesindex[0]].lpbMapAddress+i);
//			}
//			sprintf_s(charnum,"%8d",h);
//			for(int i=0;i<8;i++)
//			{
//				*(newfileaddress++)=charnum[i];
//			}
//			*(newfileaddress++)=' ';
//			*(newfileaddress++)=13;
//			*(newfileaddress++)=10;
//			newfilesize++;
//		}
//
//
//		//valisnull=getminchar(han,minfilesindex,minfilesnum,newval);
//
//
//		for(int j=0;j<minfilesnum;j++)
//		{
//			han[minfilesindex[j]].lpbMapAddress=han[minfilesindex[j]].lpbMapAddress+43;
//			filebeginindex[minfilesindex[j]]++;
//		}
//
//		for(int j=0;j<filescount;j++)
//		{
//			if(filebeginindex[j]!=han[j].qwFileSize/43)
//			{
//				//if(filebeginindex[14]>=han[i].qwFileSize/43-2)
//				//{
//				//	break;
//				//}
//				retval=false;
//
//			}
//		}
//		if (retval==true)
//			break;
//	}
//	//for(DWORD i = 0; i < han[filecount].dwBlockBytes; i++)
//	//	;
//	//han[filecount].qwFileOffset = han[filecount].qwFileOffset + han[filecount].dwBlockBytes;
//	//////释放当前映射
//	//UnmapViewOfFile(han[filecount].lpbMapAddress);
//	//if (han[filecount].qwFileSize - han[filecount].qwFileOffset < han[filecount].dwBlockBytes)
//	//	han[filecount].dwBlockBytes = (DWORD)han[filecount].qwFileSize - han[filecount].qwFileOffset;
//
//	//han[filecount].lpbMapAddress = (char *)MapViewOfFile(han[filecount].hFileMap,FILE_MAP_ALL_ACCESS,
//	//	(DWORD)(han[filecount].qwFileOffset >> 32), (DWORD)(han[filecount].qwFileOffset & 0xFFFFFFFF),han[filecount].dwBlockBytes);
//	//}
//	return retval;
//	// 关闭文件映射对象句柄
//	//CloseHandle(han.hFileMap); 
//
//	// ::SetFilePointer (hFile, qwFileSize, NULL, FILE_BEGIN);
//	//::SetEndOfFile (hFile);
//	//
//	//::CloseHandle (hFile);
//}


bool writefileonmapaddresses(handle han[],handle newhan)
{
	char * comppp="01230123012301230123012345674567";
	bool retval=true;
	int minfileindex=0;
	int minfilesindex[filescount];
	int filebeginindex[filescount];
	int minfilesnum=0;
	for(int i=0;i<filescount;i++)
	{
		filebeginindex[i]=0;
	}
	int newfilebeginindex=0;
	int minfirstindex=0;
	while(1)
	{
		retval=true;
		bool isfirst=true;
		minfilesnum=0;
		for(int i=0;i<filescount;i++)
		{
			if(han[i].endoffile==true)continue;
			if(isfirst==true&&han[i].endoffile==false)
			{
				minfirstindex=i;
				minfilesnum=1;
				minfilesindex[0]=i;
				minfileindex=i;
				isfirst=false;
				continue;
			}
			if(i!=minfileindex)
			{
				int compareval=smaller(han[i].lpbMapAddress,han[minfileindex].lpbMapAddress);
				if(compareval==1)
				{
					minfileindex=i;
					minfilesnum=1;
					minfilesindex[minfilesnum-1]=i;
				}
				else if(compareval==0)
				{
					minfilesnum++;
					minfilesindex[minfilesnum-1]=i;
				}
			}
		}

		long h=0;char *endstr;
		//int i1;
		//for( i1=0;i1<32;i1++)
		//{
		//if(comppp[i1]!=*(han[minfilesindex[0]].lpbMapAddress+i1))break;
		//}
		//if(i1==32)
		//{
		//	i1=32;
		//}
		for(int i=0;i<minfilesnum;i++)
		{
			for(int j=0;j<8;j++)
			{
				charnum[j]=*(han[minfilesindex[i]].lpbMapAddress+32+j);
			}
			h=h+strtol(charnum,&endstr,10);
		}
		if(h!=0)
		{
			//如果这边溢出内存，重新映射
			if((newhan.dwBlockBytes-newhan.headuselessbits)/43==newfilebeginindex)
			{
				UnmapViewOfFile(newhan.lpbstartaddress);
				newhan.qwFileOffset=newhan.qwFileOffset+65536;
				newhan.lpbstartaddress=(LPVOID)getMapviewoffile(newhan.hFileMap,newhan.qwFileOffset,newhan.dwBlockBytes,newhan.qwFileSize);
				newhan.headuselessbits=newhan.headuselessbits+newfilebeginindex*43-65536;
				newhan.lpbMapAddress=(char*)newhan.lpbstartaddress+newhan.headuselessbits;
				newfilebeginindex=0;
			}
			for(int i=0;i<32;i++)
			{
				*(newhan.lpbMapAddress++)=*(han[minfilesindex[0]].lpbMapAddress+i);
			}
			sprintf_s(charnum,"%8d",h);
			for(int i=0;i<8;i++)
			{
				*(newhan.lpbMapAddress++)=charnum[i];
			}
			*(newhan.lpbMapAddress++)=' ';
			*(newhan.lpbMapAddress++)=13;
			*(newhan.lpbMapAddress++)=10;
			newfilebeginindex++;
			newfilesize++;
		}
		for(int j=0;j<minfilesnum;j++)
		{
			han[minfilesindex[j]].lpbMapAddress=han[minfilesindex[j]].lpbMapAddress+43;
			filebeginindex[minfilesindex[j]]++;
		}

		for(int j=0;j<filescount;j++)
		{
			if(han[j].endoffile!=true)
			{
				//如果文件接近块的长度不足容下一个43长度，重新映射，并把lpaddress指向需要指向的地方
				if((han[j].dwBlockBytes-han[j].headuselessbits)/43==filebeginindex[j])
				{
					if(han[j].qwFileSize-han[j].qwFileOffset>131072)
					{
						UnmapViewOfFile(han[j].lpbstartaddress);
						han[j].qwFileOffset=han[j].qwFileOffset+65536;
						han[j].lpbstartaddress=(LPVOID)getMapviewoffile(han[j].hFileMap,han[j].qwFileOffset,han[j].dwBlockBytes,han[j].qwFileSize);
						han[j].headuselessbits=han[j].headuselessbits+filebeginindex[j]*43-65536;
						han[j].lpbMapAddress=(char*)han[j].lpbstartaddress+han[j].headuselessbits;
						filebeginindex[j]=0;
					}
					else
					{
						UnmapViewOfFile(han[j].lpbstartaddress);
						han[j].endoffile=true;
					}
				}
				if(retval==true)
					retval=false;

			}
		}
		if (retval==true)
		{
			UnmapViewOfFile(newhan.lpbstartaddress);
			break;
		}
	}

	return retval;
}


//void getfileshandle()
//{
//	handle han[stepfilescount];
//	LPVOID pbFile[stepfilescount];
//	char filename[20];
//	char newfilename[20];
//	for(int i=0;i<filescount;i++)
//	{
//		strcpy_s(filename,"E:\\baseall");
//		char ord[10];
//		itoa(s*20+i,ord,10);
//		strcat_s(filename,ord);
//		strcat_s(filename,".txt");
//		WCHAR   wfilename[64];
//		swprintf(wfilename,L"%S ",filename); 
//		han[i].hFileMap=getfilemaphandle(wfilename,han[i].dwBlockBytes,han[i].qwFileSize,han[i].lpbMapAddress);
//		han[i].qwFileOffset=0;
//		pbFile[i]=LPVOID(han[i].lpbMapAddress);
//	}
//	__int64 allsize=getallfilesize(han);
//
//	strcpy_s(newfilename,"e:\\4-4tuple");
//	char ord[10];
//	itoa(s,ord,10);
//	strcat_s(newfilename,ord);
//	strcat_s(newfilename,".txt");
//	WCHAR   wnewfilename[64];
//	swprintf(wnewfilename,L"%S ",newfilename); 
//
//	char *newlpMapAddress=NULL;
//	HANDLE newfilehan=NULL;
//	HANDLE newfilemaphandle= getnewfilemaphandle(newfilehan,wnewfilename,newlpMapAddress,allsize);
//	LPVOID headnewlpMapAddress=newlpMapAddress;
//	while(1)
//	{
//		if(writefileonmapaddresses(han,newfilemaphandle,newlpMapAddress)==true)
//			break;
//	}
//	for(int i=0;i<stepfilescount;i++)
//	{
//		UnmapViewOfFile(pbFile[i]);
//		CloseHandle(han[i].hFileMap); 
//	}
//	// 关闭文件对象
//
//
//	//		WCHAR   *wnewfile=L"e:\\baseall.txt";
//	//	char *newlpMapAdd=NULL;
//	//	HANDLE newfilehan= getnewfilemaphandle(wnewfilename,newlpMapAdd,newfilesize);
//	//memcpy(newlpMapAdd, newlpMapAddress, newfilesize);
//	UnmapViewOfFile(headnewlpMapAddress);
//	CloseHandle(newfilemaphandle);
//	::SetFilePointer (newfilehan, newfilesize*43, NULL, FILE_BEGIN);
//	::SetEndOfFile (newfilehan);
//	CloseHandle(newfilehan); 
//}

void getfileshandle()
{
////	handle newhan;
////	WCHAR   *wnewfilename=L"m:\\复件 8tuple.txt";
////	HANDLE newfilehan=NULL;
////	newfilehan = ::CreateFile(wnewfilename, GENERIC_READ| GENERIC_WRITE,0, NULL, 
////		OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
////
////	HANDLE hFileMapping1 = CreateFileMapping(newfilehan,
////NULL, 
////PAGE_READWRITE,
////0,
////(DWORD)(170000000&0xFFFFFFFF),
////NULL);
////
////HANDLE	hFile2 = ::CreateFile(L"m:\\8tuple1.txt", 
////GENERIC_WRITE | GENERIC_READ,
////FILE_SHARE_READ, 
////NULL,
////CREATE_ALWAYS, 
////FILE_FLAG_SEQUENTIAL_SCAN,
////NULL);
////
////// 以实际数据长度创建另外一个文件映射内核对象
////HANDLE hFileMapping2 = CreateFileMapping(hFile2,
////NULL, 
////PAGE_READWRITE,
////0,
////(DWORD)(170000000&0xFFFFFFFF),
////NULL);
////
////// 关闭文件内核对象
////CloseHandle(hFile2);
////PBYTE pbFile = (PBYTE)MapViewOfFile(hFileMapping1, 
////FILE_MAP_ALL_ACCESS, 
////0, 0, 170000000);
////// 将文件数据映射到进程的地址空间
////PBYTE pbFile2 = (PBYTE)MapViewOfFile(hFileMapping2, 
////FILE_MAP_ALL_ACCESS, 
////0, 0, 170000000);
////
////// 将数据从原来的内存映射文件复制到此内存映射文件
////memcpy(pbFile2, pbFile, 170000000); 
////
//////file://从进程的地址空间撤消文件数据映像
////UnmapViewOfFile(pbFile);
////UnmapViewOfFile(pbFile2);
////
////// 关闭文件映射对象
////CloseHandle(hFileMapping1);
////CloseHandle(hFileMapping2);



	handle han[filescount];
	char filename[40];
	for(int i=0;i<filescount;i++)
	{
		strcpy_s(filename,"e:\\all\\tuple");
		char ord[10];
		itoa(i,ord,10);
		strcat_s(filename,ord);
		strcat_s(filename,".txt");
		WCHAR   wfilename[64];
		swprintf(wfilename,L"%S ",filename); 
		han[i].hFileMap=getfilemaphandle(wfilename,han[i].dwBlockBytes,han[i].qwFileSize);
		han[i].qwFileOffset=0;
		han[i].endoffile=false;
	}
	bool shouldbreak=false;

	handle newhan;
	WCHAR   *wnewfilename=L"e:\\all\\tupleall.txt";
	HANDLE newfilehan=NULL;
	//__int64 allsize=getallfilesize(han);
	newhan.hFileMap = getnewfilemaphandle(newfilehan,wnewfilename,300000000);
	//newhan.qwFileSize=allsize;
	newhan.qwFileSize=300000000;
	for(int i=0;i<filescount;i++)
	{
		if(han[i].endoffile==false)
		{
			han[i].dwBlockBytes=131072;
			han[i].qwFileOffset=0;
			han[i].lpbMapAddress=getMapviewoffile(han[i].hFileMap,han[i].qwFileOffset,han[i].dwBlockBytes,han[i].qwFileSize);
			han[i].lpbstartaddress=LPVOID(han[i].lpbMapAddress);
			han[i].headuselessbits=0;
			//han[i].dwBlockBytes=43*dwblocksize;
		}
	}
	newhan.dwBlockBytes=131072;
	newhan.qwFileOffset=0;
	newhan.lpbMapAddress=getMapviewoffile(newhan.hFileMap,newhan.qwFileOffset,newhan.dwBlockBytes,newhan.qwFileSize);
	newhan.lpbstartaddress=LPVOID(newhan.lpbMapAddress);
	newhan.headuselessbits=0;
	writefileonmapaddresses(han,newhan);

	// 关闭文件对象


	//		WCHAR   *wnewfile=L"e:\\baseall.txt";
	//	char *newlpMapAdd=NULL;
	//	HANDLE newfilehan= getnewfilemaphandle(wnewfilename,newlpMapAdd,newfilesize);
	//memcpy(newlpMapAdd, newlpMapAddress, newfilesize)
	//::SetFilePointer (newfilehan, newfilesize*43, NULL, FILE_BEGIN);
	//::SetEndOfFile (newfilehan);
	//CloseHandle(newfilehan); 

}
int sgn(int* M,int m)
{

	int s=0;
	for(int i=0;i<=m-2;i++)
	{
		for(int j=i+1;j<=m-1;j++)
		{
			if(M[i]>M[j])
				s=s+1;
		}
	}
	if(s%2==0) return 1;
	else return -1;
}
int M[8]={-1};
int TYPE[4]={0};


void partfile()
{

	WCHAR   *wfilename=L"e:\\all\\tupleall.txt";
	handle newfilehan;
	getfilemaphandle(wfilename,newfilehan.dwBlockBytes,newfilehan.qwFileSize,newfilehan.lpbMapAddress);

	WCHAR   *wmainfilename=L"e:\\all\\mainpart.txt";
	handle mainparthan;
	getnewfilemaphandle(mainparthan.hFileMap,wmainfilename,mainparthan.lpbMapAddress,125*1024*1024);

	WCHAR   *wplusfilename=L"e:\\all\\pluspart.txt";
	handle plusparthan;
	getnewfilemaphandle(plusparthan.hFileMap,wplusfilename,plusparthan.lpbMapAddress,125*1024*1024);

	char* plpbM=newfilehan.lpbMapAddress;
	char*plpmain=mainparthan.lpbMapAddress;
	char*plpplus=plusparthan.lpbMapAddress;
	int inewfile=0;
	int value[2];

	int alllll=0;
	while(inewfile<=newfilehan.qwFileSize/43)
	{
		if(1/*getpfa4value(newfilehan.lpbMapAddress,value)*/)
		{
			alllll=alllll+abs(value[0]/576);
			for(int i=0;i<32;i++)
			{
				*(mainparthan.lpbMapAddress++)=*(newfilehan.lpbMapAddress+i);
				if(value[1]!=0)
				{
					*(plusparthan.lpbMapAddress++)=*(newfilehan.lpbMapAddress+i);
				}
			}
			sprintf_s(charnum,"%8d",value[0]);
			for(int i=32;i<40;i++)
			{
				*(mainparthan.lpbMapAddress++)=charnum[i-32];
			}
			*(mainparthan.lpbMapAddress++)=' ';*(mainparthan.lpbMapAddress++)=13;*(mainparthan.lpbMapAddress++)=10;
			if(value[1]!=0)
			{
				sprintf_s(charnum,"%8d",value[1]);
				for(int i=32;i<40;i++)
				{
					*(plusparthan.lpbMapAddress++)=charnum[i-32];
				}
				*(plusparthan.lpbMapAddress++)=' ';*(plusparthan.lpbMapAddress++)=13;*(plusparthan.lpbMapAddress++)=10;
			}
			newfilehan.lpbMapAddress=newfilehan.lpbMapAddress+43;
			inewfile++;
		}
		else
		{
			for(int i=0;i<43;i++)
			{
				*(plusparthan.lpbMapAddress++)=*(newfilehan.lpbMapAddress+i);
			}
			newfilehan.lpbMapAddress=newfilehan.lpbMapAddress+43;
			inewfile++;
		}
	}
	UnmapViewOfFile(plpbM);UnmapViewOfFile(plpmain);UnmapViewOfFile(plpplus);

}
struct PFA
{
	int index[4];
	int val;
};
PFA pfa[70];
void evaluepfa()
{
	srand((unsigned int)time(NULL)); 
	int index=0;
	for(int i=0;i<8;i++)
	{
		for(int j=i+1;j<8;j++)
		{
			for(int k=j+1;k<8;k++)
			{
				for(int l=k+1;l<8;l++)
				{
					pfa[index].index[0]=i;
					pfa[index].index[1]=j;
					pfa[index].index[2]=k;
					pfa[index].index[3]=l;
					pfa[index].val=rand()%3;//1;;
					index++;
				}
			}
		}
	}
}

void getdetvalue()
{

	WCHAR   *wfilename=L"e:\\all\\pluspart.txt";
	handle newfilehan;
	getfilemaphandle(wfilename,newfilehan.dwBlockBytes,newfilehan.qwFileSize,newfilehan.lpbMapAddress);
	char* plpbM=newfilehan.lpbMapAddress;
	int inewfile=0;
	__int64 detvalue=0;__int64 termvalue=1;
	while(inewfile<newfilehan.qwFileSize/43)
	{


		termvalue=1;
		for(int i=0;i<8;i++)
		{

			for(int k=0;k<70;k++)
			{
				int j;
				for (j=0;j<4;j++)
				{

					if(*(newfilehan.lpbMapAddress+i*4+j)!=pfa[k].index[j]+48)break;
				}
				if(j==4)
				{
					termvalue=termvalue*pfa[k].val;
					break;
				}
			}
		}
		char *endptr;
		for(int i=32;i<40;i++)
		{
			charnum[i-32]=*(newfilehan.lpbMapAddress+i);
		}
		int scalarvalue=strtol(charnum,&endptr,10);
				//if(inewfile==newfilehan.qwFileSize/43-1)
				//{
				//	inewfile=inewfile;
				//}
		detvalue=detvalue+termvalue*scalarvalue;
		inewfile++;
		newfilehan.lpbMapAddress=newfilehan.lpbMapAddress+43;
	}
}