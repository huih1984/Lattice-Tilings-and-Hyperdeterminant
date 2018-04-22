#pragma once
#include "inferior_datum.h"

class Inferior_datum_mutiplicity;

struct Hashtable
{
	Inferior_datum_mutiplicity* address;
};


class Inferior_datum_mutiplicity :
	virtual public Inferior_datum
{
public:
	Inferior_datum_mutiplicity(void);
	virtual ~Inferior_datum_mutiplicity(void);
	void set_hashkey();

public:
	short mutiplicity;
	short genius_mutiplicity;
	short no_cal_row_sgn_mutiplicity;
	unsigned int hashkey;

	static  unsigned int hashtblsize;
	static Hashtable* tbl;
};


