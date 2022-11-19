#include "Complex.hxx"
#include "Polar.hxx"

#include <math.h>

Complex::Complex(const Polar& p)
{
	re = p.mag * cos(p.ang);
	im = p.mag * sin(p.ang);
}
