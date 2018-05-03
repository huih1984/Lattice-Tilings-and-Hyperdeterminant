#pragma once
#include "inferior_datum_mutiplicity.h"
#define ASIZE 10
#define REMSIZE 10
class CPreimage
{
public:
	CPreimage(void);
	~CPreimage(void);
	FILE* reminderf, *basef ;
	bool Fetch_smallarray_end;
	unsigned int imagehashkey;
	Inferior_datum_mutiplicity smallarray[ASIZE];
	Inferior_datum_mutiplicity reminderarray[REMSIZE];
	int couldstartremindermulti;
	unsigned int index;
	unsigned int reminderindex;
	Inferior_datum_mutiplicity multival;
	void multiple(Inferior_datum_mutiplicity& left,Inferior_datum_mutiplicity& right, Inferior_datum_mutiplicity& val);
	void get_smallarray(Inferior_datum_mutiplicity&  val);
	void print_reminder_collection(Inferior_datum_mutiplicity& val);
	void fetch_some_reminder(Inferior_datum_mutiplicity& val);
};
