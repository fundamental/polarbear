#include "Complex.hxx"
#include "Polar.hxx"

#include <math.h>

Complex_::Complex_(const Polar& p)
{
	re = p.mag * cos(p.ang);
	im = p.mag * sin(p.ang);
}
