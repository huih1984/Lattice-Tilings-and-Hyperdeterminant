#include "StdAfx.h"
#include "Preimage.h"
#include "inferior_datum_mutiplicity.h"
CPreimage::CPreimage(void)
{
	index = 0;
	reminderindex = 0;
	couldstartremindermulti =0;
	Fetch_smallarray_end = false;
	reminderf = fopen("f:/outputremindermulti.txt", "w+");
	basef = fopen("f:/basecollection.txt", "w+");
}

CPreimage::~CPreimage(void)
{
	fclose(reminderf);
	reminderf = NULL;
	fclose(basef);
	basef= NULL;
}


 void CPreimage::multiple(Inferior_datum_mutiplicity& left,Inferior_datum_mutiplicity& right, Inferior_datum_mutiplicity& val)
{
	memcpy(&val, &left, sizeof(left));
	for(int i = 0; i < innerdim; ++i)
	{
		for(int j = 0; j < outerdim; ++j)
		{
			val.matrix[i][j] = right.matrix[i][val.matrix[i][j]];
		}
	}
}

 void CPreimage::get_smallarray(Inferior_datum_mutiplicity&  val)
 {
	 memcpy(&smallarray[index++], &val, sizeof(Inferior_datum_mutiplicity));
	 if(index == ASIZE)
	 {
		 Inferior_datum_mutiplicity multival;
		 multiple(smallarray[1], smallarray[4], multival);
		 smallarray[0].conv_to_regular();
		 smallarray[1].conv_to_regular();

		 multival.conv_to_regular();
		 smallarray[0].printme(basef);
		 smallarray[1].printme(basef);
		 multival.printme(basef);
		 Fetch_smallarray_end = true;
		 //system("pause");
	 }
 }
//将剩余集中取某个或者某些元素，乘上所有元素，检验是否会落到原集中
 void CPreimage::print_reminder_collection(Inferior_datum_mutiplicity& val)
 {

	 if(couldstartremindermulti == 0)
	 {
		 for(int i = 0; i < REMSIZE; ++i)
		 {
			 for(int j = i+1; j < REMSIZE; ++j)
			 {
				 multiple(reminderarray[i], reminderarray[j], multival);
				 multival.conv_to_regular();
				 multival.printme(reminderf);
			 }
		 }
		 couldstartremindermulti = 1;
	 }
	 else if(couldstartremindermulti == 1)
	 {
		 for(int i = 0; i < REMSIZE; ++i)
		 {
			 multiple(reminderarray[i], val, multival);
			 multival.conv_to_regular();
			 multival.printme(reminderf);
		 }
	 }
 }

 void CPreimage::fetch_some_reminder(Inferior_datum_mutiplicity& val)
 {
	 memcpy(&reminderarray[reminderindex++], &val, sizeof(Inferior_datum_mutiplicity));
 }

 //