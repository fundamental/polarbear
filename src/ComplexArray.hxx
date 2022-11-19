#ifndef __ComplexArray__
#define __ComplexArray__

#include "Complex.hxx"

class ComplexArray
{
public:
	Complex_ data[256];
	int size;

	ComplexArray()
	{
		size = 0;
	}
	
	void Add(const Complex_& c)
	{
		data[size++] = c;
	}
	
	Complex_& operator [] (int i){
		return data[i];
	}
};

#endif
