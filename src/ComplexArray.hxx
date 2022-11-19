#ifndef __ComplexArray__
#define __ComplexArray__

#include "Complex.hxx"

class ComplexArray
{
public:
	Complex data[256];
	int size;

	ComplexArray()
	{
		size = 0;
	}
	
	void Add(const Complex& c)
	{
		data[size++] = c;
	}
	
	Complex& operator [] (int i){
		return data[i];
	}
};

#endif
