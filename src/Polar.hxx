#ifndef __Polar__
#define __Polar__

class Complex_;

class Polar
{
public:
	double ang,mag;
	Polar(double _ang = 0,double _mag = 0)
	:ang(_ang),mag(_mag)
	{
	}
	Polar(const Complex_& p);
	Polar operator - (void) const
	{
		return Polar(ang,-mag);
	}
	Polar operator * (const Polar& a) const
	{
		return Polar(ang+a.ang,mag*a.mag);
	}
	Polar operator + (const Polar& a) const;
};

#endif
