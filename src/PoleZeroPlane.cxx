#include "PoleZeroPlane.hxx"
#include "ComplexExtArray.hxx"
#include "Polar.hxx"

#include <FL/Fl.H>
#include <FL/fl_draw.H>

#include <stdio.h>

PoleZeroPlane::PoleZeroPlane(int X,int Y,int W,int H)
:Fl_Widget(X,Y,W,H)
{
	resize(x(),y(),w(),h());
	
	mExtArray[0]=0;
	mExtArray[1]=0;
	
	mMoveMode = kMoveModeNone;
	mAddMode = kAddModeNone;
}


void PoleZeroPlane::resize(int X,int Y,int W,int H)
{
	mXCenter = double(X+(W+1)/2);
	mYCenter = double(Y+(H+1)/2);
	mRadius  = double(W/3);

	Fl_Widget::resize(X,Y,W,H);
}

void PoleZeroPlane::UnselectAll(void)
{
	int i;
	for (i=0;i<2;i++)
	{
		mExtArray[i]->UnselectAll();
	}
	mAction = kActionUnselectAll;
	do_callback();

	redraw();
}

void PoleZeroPlane::SelectAll(void)
{
	int i;
	for (i=0;i<2;i++)
	{
		mExtArray[i]->SelectAll();
	}
	mAction = kActionSelectAll;
	do_callback();

	redraw();
}

void PoleZeroPlane::ToggleSelect(EPoleZero pz,int i)
{
	int mirror = mExtArray[pz]->Mirror(i);
	bool val;
	
	val = !mExtArray[pz]->Selected(i);

	mExtArray[pz]->Select(i,val);

	mAction = kActionSelect;
	mActionPoleZero = pz;
	mActionId = i;
	do_callback();

	if (mirror!=-1 && val != mExtArray[pz]->Selected(mirror))
	{
		mExtArray[pz]->Select(mirror,val);

		mAction = kActionSelect;
		mActionPoleZero = pz;
		mActionId = mirror;
		do_callback();
	}

	redraw();
}

void PoleZeroPlane::Select(EPoleZero pz,int i)
{
	int mirror = mExtArray[pz]->Mirror(i);

	if (!mExtArray[pz]->Selected(i))
	{
		mExtArray[pz]->Select(i);
		mAction = kActionSelect;
		mActionPoleZero = pz;
		mActionId = i;
		do_callback();
	}
	
	if (mirror!=-1)
	{
		if (!mExtArray[pz]->Selected(mirror))
		{
			mExtArray[pz]->Select(mirror);
			mAction = kActionSelect;
			mActionPoleZero = pz;
			mActionId = mirror;
			do_callback();
		}
	}

	redraw();
}

void PoleZeroPlane::RemoveSelected(void)
{
	int i;
	
	mAction = kActionRemove;
	mActionPoleZero = kPole;
	do_callback();

	mAction = kActionRemove;
	mActionPoleZero = kZero;
	do_callback();
	
	for (i=0;i<2;i++)
	{
		mExtArray[i]->RemoveSelected();
	}

	mAction = kActionRemoved;
	do_callback();
	
	redraw();
}

int PoleZeroPlane::handle(int e)
{
	if (e==FL_ENTER) return 1;
	if (e==FL_LEAVE) return 1;
	if (e==FL_FOCUS) return 1;
	if (e==FL_UNFOCUS) return 1;
	if (e==FL_KEYBOARD)
	{
		/* todo: handle keyboard events, cursors keys etc */
		return 0;
	}
	if (e==FL_PUSH)
			take_focus();
	if (e==FL_PUSH && Fl::event_clicks())
	{
		Add(EventPos(),mAddMode,Fl::event_ctrl());
		return 1;		
	}
	switch (mMoveMode)
	{
	case kMoveModeSelect:
		if (e==FL_PUSH)
		{
			Select(EventPos(),Fl::event_ctrl(),Fl::event_shift());			
			return 1;
		}
		break;
	case kMoveModeXY: case kMoveModeAng: case kMoveModeMag:
		if (e==FL_PUSH)
		{
			mPos = EventPos();
			Select(mPos,Fl::event_ctrl(),Fl::event_shift());			
			mPrevPos = mPos;
			return 1;
		}
		if (e==FL_DRAG)
		{
			mPos = EventPos();
			MoveSelected(mPos,mPrevPos,mMoveMode);
			mPrevPos = mPos;
			return 1;
		}
		break;
	default:
		return 0;
	}
	return 0;
}

void PoleZeroPlane::Add(
	const Complex& pos,const EAddMode& mode,const bool& mirror)
{
	EPoleZero pz;
	switch (mode)
	{
	case kAddModePole:
		pz = kPole;
		break;
	case kAddModeZero:
		pz = kZero;
		break;
	default:
		return;
	}
	mExtArray[pz]->Add(pos,mirror);
	mActionPoleZero = pz;
	if (mirror)
		mAction = kActionAddPair;
	else
		mAction = kActionAdd;
	
	do_callback();

	redraw();
}

Complex PoleZeroPlane::EventPos(void)
{
	return Complex(
		(double(Fl::event_x())-mXCenter)/mRadius,
		(double(Fl::event_y())-mYCenter)/mRadius
	);
}

void PoleZeroPlane::Select(const Complex& pos,bool toggle,bool add)
{
	int found = -1;
	int foundArray = -1;
	int i,j;

	double min = 0.;

	for (i=0;i<2;i++)
	{
		for (j=0;j<mExtArray[i]->Size();j++)
		{
			Complex dif = mExtArray[i]->GetComplex(j)-pos;
			double d = dif.Abs();

			if (found==-1 || d<min)
			{
				found = j;
				foundArray = i;
				min = d;
			}
		}
	}
	if (found!=-1 && min*mRadius<5)
	{
		if (
				(!add && !toggle) &&
				(!mExtArray[foundArray]->Selected(found))
			)
			UnselectAll();
		if (toggle)
		{
			ToggleSelect((EPoleZero)foundArray,found);
		}else{
			Select((EPoleZero)foundArray,found);
		}
	}else{
		if ((!add && !toggle))
			UnselectAll();
	}
		
	for (i=0;i<2;i++)
	{
		mExtArray[i]->SetMoveSelection(pos.im>=0);
	}

	redraw();
}

void PoleZeroPlane::MoveSelected(
	const Complex& pos,const Complex& prevPos,const EMoveMode& mode)
{
	int i;
	if (mode==kMoveModeXY)
	{
		for (i=0;i<2;i++)
		{
			mExtArray[i]->MoveSelected(
				pos.re - prevPos.re,
				pos.im - prevPos.im);
		}
	}
	else
	{
		Polar posPolar(pos);
		Polar prevPosPolar(prevPos);

		if (mode==kMoveModeMag)
		{
			for (i=0;i<2;i++)
			{
				mExtArray[i]->MoveSelectedPolar(
					0,posPolar.mag - prevPosPolar.mag);
			}
		}
		else
		{
			for (i=0;i<2;i++)
			{
				mExtArray[i]->MoveSelectedPolar(
					posPolar.ang - prevPosPolar.ang, 0);
			}
		}
	}
	mAction = kActionMove;
	mActionPoleZero = kPole;
	do_callback();

	mAction = kActionMove;
	mActionPoleZero = kZero;
	do_callback();

	redraw();
}

void PoleZeroPlane::draw(void)
{
	fl_color(color());
	fl_rectf(x(),y(),w(),h());

	int i;

	fl_color(FL_BLUE);

	fl_xyline(x(),mYCenter,x()+w());
	fl_yxline(mXCenter,y(),y()+h());
	fl_circle(mXCenter,mYCenter,mRadius);

//	fl_font(FL_COURIER,10);
//	fl_color(FL_BLACK);
//	char tmp[256];
//	sprintf(tmp,"%8.5f %8.5f %s",pos.re,pos.im,mode?"pole":"zero");
//	fl_draw(tmp,x(),y()+16);

	for (i=0;i<mExtArray[0]->Size();i++)
	{
		int ix = int(mXCenter + 0.5+mExtArray[0]->GetComplex(i).re*mRadius);
		int iy = int(mYCenter + 0.5+mExtArray[0]->GetComplex(i).im*mRadius);
		if (mExtArray[0]->Selected(i)) {
			fl_color(selection_color());
		} else {
			fl_color(labelcolor());
		}

		fl_line(ix-2,iy-3,ix+2,iy-3);
		fl_line(ix-3,iy-2,ix-3,iy+2);
		fl_line(ix+3,iy-2,ix+3,iy+2);
		fl_line(ix-2,iy+3,ix+2,iy+3);
		fl_rect(ix-2,iy-2,5,5);
	}

	for (i=0;i<mExtArray[1]->Size();i++)
	{
		int ix = int(mXCenter + 0.5+mExtArray[1]->GetComplex(i).re*mRadius);
		int iy = int(mYCenter + 0.5+mExtArray[1]->GetComplex(i).im*mRadius);

		if (mExtArray[1]->Selected(i)) {
			fl_color(FL_RED);
		} else {
			fl_color(FL_BLACK);
		}

		fl_line(ix-3,iy-3,ix+3,iy+3);
		fl_line(ix-2,iy-3,ix+3,iy+2);
		fl_line(ix-3,iy-2,ix+2,iy+3);

		fl_line(ix+3,iy-3,ix-3,iy+3);
		fl_line(ix+2,iy-3,ix-3,iy+2);
		fl_line(ix+3,iy-2,ix-2,iy+3);
	}
}
