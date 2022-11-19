#ifndef __PoleZeroList__
#define __PoleZeroList__

#include <FL/Fl_Group.H>

#include <stdio.h>

class ComplexExtArray;

class PoleZeroList:public Fl_Group
{
public:
	ComplexExtArray* array;
	PoleZeroList(int X,int Y,int W,int H);

	void Add(void);
	void Sync(int i);
	void SyncPos(int i);
	void SyncSelect(int i);

	void Update(void);
	void UpdateSize(void);
	void RecalcPositions(void);

	int mEditedId;
	int mEditedType;
	const char* mEditedValue;
};

#endif
