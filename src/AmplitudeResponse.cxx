#include "AmplitudeResponse.hxx"
#include "Defines.hxx"
#include "Complex.hxx"
#include "ComplexArray.hxx"
#include "Polar.hxx"
#include <FL/fl_draw.H>

void Fl_Amplitude_Response::draw(void)
{
	fl_color(color());
	fl_rectf(x(),y(),w(),h());
	fl_color(labelcolor());
	double d = PI/(double)w();
	double ang ;
	int cx,cxw = x()+w();
	int i;
	ang = 0;
	double max = 0;
	for (cx=x();cx<cxw;cx++)
	{
		double num = 1., den = 1.;
		Complex_ omega(Polar(ang,1.));
		for (i=0;i<(*mZeros).size;i++)
		{
			Complex_ d = (*mZeros)[i] - omega;
			num *= d.Abs();
		}
		for (i=0;i<(*mPoles).size;i++)
		{
			Complex_ d = (*mPoles)[i] - omega;
			den *= d.Abs();
		}
		double amp = num/den;
		if (amp>max) max = amp;
		ang += d;
	}
	ang = 0;
	double scale = h()/max;
	int py = 0;
	for (cx=x();cx<cxw;cx++)
	{
		double num = 1., den = 1.;
		Complex_ omega(Polar(ang,1.));
		for (i=0;i<(*mZeros).size;i++)
		{
			Complex_ d = (*mZeros)[i] - omega;
			num *= d.Abs();
		}
		for (i=0;i<(*mPoles).size;i++)
		{
			Complex_ d = (*mPoles)[i] - omega;
			den *= d.Abs();
		}
		double amp = num/den;
		int yy = y()+h()-scale*amp;
		if (cx!=x()) {
			fl_line(cx-1,py,cx,yy);
		}
		py = yy;
		ang += d;
	}
}
