#include "PhaseResponse.hxx"
#include "Defines.hxx"
#include "Complex.hxx"
#include "ComplexArray.hxx"
#include "Polar.hxx"
#include <FL/fl_draw.H>

#include <stdio.h>

void Fl_Phase_Response::draw(void)
{
	fl_color(color());
	fl_rectf(x(),y(),w(),h());
	fl_color(labelcolor());
	double d = PI/(double)w();
	double ang ;
	int cx,cxw = x()+w();
	int ch = y()+h()/2;
	int i,v,pv;
	ang = 0;
	double max = 0;

	double scale = double(h())/65536.;
	int py = 0;

	for (cx=x();cx<cxw;cx++)
	{
		double pha = 0.;
		Complex_ omega(Polar(ang,1.));
		for (i=0;i< (*mZeros).size;i++)
		{
			Complex_ d = (*mZeros)[i] - omega;
			pha += d.Angle();
		}
		for (i=0;i<(*mPoles).size;i++)
		{
			Complex_ d = (*mPoles)[i] - omega;
			pha -= d.Angle();
		}
		short v = int(32768.*pha/PI);
		
		int yy = ch-v*scale;
		//if (cx<x()+4){
		//	printf("%f %d %d\n",pha,v,yy);
		//}
		if (cx!=x()) {
			if ((v>15000 && pv<-15000) || (v<-15000 && pv>15000))
			{
				if (v>15000)
				{
					fl_line(cx-1,py,cx,y()+h());
					fl_line(cx-1,y(),cx,yy);
				}else{
					fl_line(cx-1,py,cx,y());
					fl_line(cx-1,y()+h(),cx,yy);
				}
			}else{
				fl_line(cx-1,py,cx,yy);
			}
		}
		py = yy;
		pv = v;
		
		ang += d;
	}
}
