#include "ToolTipButton.hxx"
#include <stdio.h>
Fl_Box* ToolTipButton::tooltipBox = 0;
ToolTipButton* ToolTipButton::current = 0;
int ToolTipButton::xlimit = 0;
int ToolTipButton::handle(int e)
{
	if (e==FL_ENTER)
	{
		if (tooltipBox) {
			tooltipBox->hide();
			int cx = x();
			if (xlimit == 0) xlimit=tooltipBox->x()+tooltipBox->w();
			if (cx+tooltipBox->w()>xlimit)
				cx = xlimit-tooltipBox->w();
			tooltipBox->resize(cx,tooltipBox->y(),120,tooltipBox->h());
			tooltipBox->label((char*)user_data());
			tooltipBox->show();
			tooltipBox->redraw();
			current=this;
		}
		return 1;
	}
	if (e==FL_LEAVE)
	{
		if (current==this && tooltipBox) {
			tooltipBox->hide();
		}
		return 1;
	}
	return Fl_Button::handle(e);
}
