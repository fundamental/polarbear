#include "Complex.hxx"
#include "Polar.hxx"

#include <math.h>

Polar::Polar(const Complex_& p)
{
	ang = atan2(p.im,p.re);
	mag = sqrt(p.im*p.im + p.re*p.re);
}

Polar Polar::operator + (const Polar& a) const
{
	Complex_ c(*this);
	Complex_ d(a);
	Complex_ e = c+d;
	Polar v(e);
	return v;
}
