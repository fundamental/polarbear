#ifndef __ComplexExtArray__
#define __ComplexExtArray__

#include "ComplexExt.hxx"
#include "Selection.hxx"

class Complex;
class ComplexArray;

class ComplexExtArray
{
public:
	ComplexArray* array;
	ComplexExt data[256];
	Selection selection;
	Selection moveSelection;
	
	ComplexExtArray(ComplexArray* a = 0);
	int Size(void);
	void Select(int i,bool val = true);
	void SelectAll(void);
	void UnselectAll(void);
	int Selected(int i);
	int Mirror(int i);
	void Add(const Complex& c,bool mirror = 0);
	void MoveSelectedPolar(double ang,double mag);
	void MoveSelected(double dx,double dy);
	const Complex& GetComplex(int i);
	void RemoveSelected(void);
	void SetMoveSelection(bool imPositive);
};

#endif
