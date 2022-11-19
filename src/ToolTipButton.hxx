#ifndef __ToolTipButton__
#define __ToolTipButton__

#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>

class ToolTipButton:public Fl_Button
{
public:
	ToolTipButton(int X,int Y,int W,int H)
	:Fl_Button(X,Y,W,H)
	{
	}
	ToolTipButton(int X,int Y,int W,int H,char* L)
	:Fl_Button(X,Y,W,H,L)
	{
	}
	static Fl_Box* tooltipBox;
	static ToolTipButton* current;
	static int xlimit;

	int handle(int e);
};

#endif
