#ifndef __Complex__
#define __Complex__

#include <math.h>

class Polar;

class Complex_
{
public:
	double re,im;
	Complex_(double _re = 0.,double _im = 0.)
	:re(_re),im(_im)
	{
	}
	Complex_(const Polar& p);
	Complex_ operator - (void) const
	{
		Complex_ ret(-re,-im);
		return ret;
	}
	Complex_ operator - (const Complex_& a) const
	{
		Complex_ ret(re-a.re,im-a.im);
		return ret;
	}
	double Abs(void) const
	{
		return sqrt(re*re+im*im);
	}
	double Angle(void) const
	{
		return atan2(im,re);
	}
	Complex_ operator + (const Complex_& a) const
	{
		Complex_ ret(re+a.re,im+a.im);
		return ret;
	}
	Complex_ operator * (const Complex_& a) const
	{
		Complex_ ret(re*a.re - im*a.im , im*a.re + re * a.im);
		return ret;
	}
};

#define Complex Complex_

#endif
