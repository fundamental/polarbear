#ifndef __CoefArray__
#define __CoefArray__

typedef double Coef;

class CoefArray
{
public:
	Coef data[256];
	int size;

	CoefArray()
	{
		size = 0;
	}

	void Add(const Coef& c)
	{
		data[size++] = c;
	}
	
	Coef& operator [] (int i){
		return data[i];
	}
};

#endif
