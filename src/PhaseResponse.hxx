#ifndef __Fl_Phase_Response__
#define __Fl_Phase_Response__

#include <FL/Fl_Widget.H>


class Fl_Phase_Response:public Fl_Widget
{
public:
	Fl_Phase_Response(int ix,int iy,int iw,int ih)
	:Fl_Widget(ix,iy,iw,ih)
	{
	}
	void draw(void);
        class ComplexArray* mPoles;
        class ComplexArray* mZeros;
};

#endif

