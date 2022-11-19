#ifndef __Complex__
#define __Complex__

#include <math.h>

class Polar;

class Complex
{
public:
	double re,im;
	Complex(double _re = 0.,double _im = 0.)
	:re(_re),im(_im)
	{
	}
	Complex(const Polar& p);
	Complex operator - (void) const
	{
		Complex ret(-re,-im);
		return ret;
	}
	Complex operator - (const Complex& a) const
	{
		Complex ret(re-a.re,im-a.im);
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
	Complex operator + (const Complex& a) const
	{
		Complex ret(re+a.re,im+a.im);
		return ret;
	}
	Complex operator * (const Complex& a) const
	{
		Complex ret(re*a.re - im*a.im , im*a.re + re * a.im);
		return ret;
	}
};

#endif
