#include "Complex.hxx"
#include "Polar.hxx"

#include <math.h>

Polar::Polar(const Complex& p)
{
	ang = atan2(p.im,p.re);
	mag = sqrt(p.im*p.im + p.re*p.re);
}

Polar Polar::operator + (const Polar& a) const
{
	Complex c(*this);
	Complex d(a);
	Complex e = c+d;
	Polar v(e);
	return v;
}
