#include "ComplexExtArray.hxx"
#include "ComplexArray.hxx"
#include "Polar.hxx"

ComplexExtArray::ComplexExtArray(ComplexArray* a)
{
	array = a;
}

int ComplexExtArray::Size(void)
{
	return array->size;
}

int ComplexExtArray::Mirror(int i)
{
	return data[i].mirror;
}

void ComplexExtArray::Select(int i,bool val)
{
	if (val)
	{
		if (!Selected(i))
		{
			selection.Add(i);
			data[i].selected = 1;
		}
	}else{
		if (Selected(i))
		{
			selection.Remove(i);
			data[i].selected = 0;
		}
	}
}

void ComplexExtArray::SelectAll(void)
{
	int i;
	for (i=0;i<array->size;i++)
		Select(i,1);
}

void ComplexExtArray::UnselectAll(void)
{
	int i;
	for (i=0;i<selection.size;i++)
		data[selection.data[i]].selected = 0;
	selection.size = 0;
}

int ComplexExtArray::Selected(int i)
{
	return data[i].selected;
}

void ComplexExtArray::Add(const Complex_& c,bool mirror)
{
	int id = Size();
	data[id].selected = 0;
	array->Add(c);
	if (mirror)
	{
		array->Add(Complex_(c.re,-c.im));
		data[id].mirror = id+1;
		data[id+1].mirror = id;
		data[id+1].selected = 0;
	}else{
		data[id].mirror = -1;
	}
}

void ComplexExtArray::MoveSelected(double dx,double dy)
{
	int i;
	for (i=0;i<moveSelection.size;i++)
	{
		int j = moveSelection.data[i];
		int m = data[j].mirror;
		array->data[j].re += dx;
		array->data[j].im += dy;
		if (m!=-1)
		{
			array->data[m].re = array->data[j].re;
			array->data[m].im = -array->data[j].im;
		}
	}
}

void ComplexExtArray::MoveSelectedPolar(double ang,double mag)
{
	int i;
	for (i=0;i<moveSelection.size;i++)
	{
		int j = moveSelection.data[i];
		int m = data[j].mirror;
		Polar p(array->data[j]);
		p.ang += ang;
		p.mag += mag;
		array->data[j] = Complex_(p);
		if (m!=-1)
		{
			array->data[m].re = array->data[j].re;
			array->data[m].im = -array->data[j].im;
		}
	}
}

const Complex_& ComplexExtArray::GetComplex(int i)
{
	return array->data[i];
}

void ComplexExtArray::RemoveSelected(void)
{
	int i,j;

	int newid[256];

	selection.size = 0;

	j = 0;
	for (i=0;i<array->size;i++)
	{
		newid[i] = -1;
		if (!data[i].selected)
		{
			if (i!=j)
			{
				data[j] = data[i];
				array->data[j] = array->data[i];
				newid[i] = j;
			}
			j++;
		}
	}
	array->size = j;
	for (i=0;i<array->size;i++)
	{
		if (data[i].mirror!=-1)
		{
			if (newid[data[i].mirror]!=-1)
			{
				data[i].mirror = newid[data[i].mirror];
			}
		}
	}		
}

void ComplexExtArray::SetMoveSelection(bool imPositive)
{
	moveSelection.size = 0;
	int j;
	for (j=0;j<selection.size;j++)
	{
		int k = selection.data[j];
		int m = data[k].mirror;
		if (m==-1 || 
			(GetComplex(k).im>=0) == (imPositive)
		) {
			moveSelection.Add(k);
		}
	}
}
