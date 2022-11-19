#ifndef __Selection__
#define __Selection__

class Selection
{
public:
	int data[256];
	int size;
	Selection()
	{
		size  = 0;
	}
	void Add(int id)
	{
		data[size++] = id;
	}
	void Remove(int id)
	{
		int i;
		for (i=0;i<size;i++)
		{
			if (data[i]==id)
			{
				size--;
				if (size>=0)
					data[i]=data[size];
				return;
			}
		}
	}
};

#endif
