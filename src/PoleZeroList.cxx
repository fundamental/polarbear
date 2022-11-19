#include "PoleZeroList.hxx"
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <stdio.h>
#include "Complex.hxx"
#include "ComplexExtArray.hxx"

class PoleZeroTextInput:public Fl_Group
{
public:
	Fl_Input* input[2];

	static void s_input_cb(Fl_Input* inp)
	{
		PoleZeroTextInput* pzti = (PoleZeroTextInput*) inp->parent();
		PoleZeroList* list = (PoleZeroList*) pzti->parent();
		list->mEditedId = list->find(pzti);
		list->mEditedValue = inp->value();
		if (inp == pzti->input[0])
			list->mEditedType = 0; /* real */
		else
			list->mEditedType = 1; /* imag */
		list->do_callback();
	}

	PoleZeroTextInput(int X,int Y,int W,int H)
	:Fl_Group(X,Y,W,H)
	{
		int hw =w()/2;

		int cx = x();

		input[0] = new Fl_Input(cx,y(),hw,h());
		cx += hw;
		input[1] = new Fl_Input(cx,y(),hw,h());
		cx += hw;
		for (int i=0;i<2;i++)
		{
			input[i]->box(FL_FLAT_BOX);
			input[i]->when(FL_WHEN_CHANGED);
			input[i]->textfont(FL_COURIER);
			input[i]->callback((Fl_Callback*) s_input_cb);
		}
		end();
	}

	void Set(double x,double y)
	{
		char tmp[256];
		sprintf(tmp,"%8.5f",x);
		input[0]->value(tmp);
		sprintf(tmp,"%8.5f",y);
		input[1]->value(tmp);
	}
};

PoleZeroList::PoleZeroList(int X,int Y,int W,int H)
:Fl_Group(X,Y,W,H)
{
	resizable(NULL);
}

void PoleZeroList::Add(void)
{
	Fl_Widget* last = 0;
	if (children()) last = child(children()-1);

	int cy = last ? last->y()+last->h() : y();

	PoleZeroTextInput* p = new PoleZeroTextInput(x(),cy,w(),20);
	add(p);
	for (int i=0;i<2;i++)
	{
		p->input[i]->color(color());
		p->input[i]->textcolor(labelcolor());
	}
	UpdateSize();
}

void PoleZeroList::Sync(int i)
{
	SyncPos(i);
	SyncSelect(i);
}

void PoleZeroList::SyncPos(int i)
{
	PoleZeroTextInput* item = (PoleZeroTextInput*) child(i);
	Complex c = array->GetComplex(i);
	item->Set(c.re,c.im);
	SyncSelect(i);
}

void PoleZeroList::SyncSelect(int i)
{
	PoleZeroTextInput* item = (PoleZeroTextInput*) child(i);
	if (array->Selected(i))
	{
		Fl_Color c = selection_color();
		if (c==color()) c = FL_RED; /* make sure it is visible */ 
		item->input[0]->textcolor(c);
		item->input[1]->textcolor(c);
		item->redraw();
	}
	else
	{
		item->input[0]->textcolor(labelcolor());
		item->input[1]->textcolor(labelcolor());
		item->redraw();
	}
}

void PoleZeroList::Update(void)
{
	int cy = y()+h();

	int n = array->Size()-children();
	if (n>0)
	{
		while (n)
		{
			PoleZeroTextInput* p = new PoleZeroTextInput(x(),cy,w(),20);
			add(p);
			for (int i=0;i<2;i++)
			{
				p->input[i]->color(color());
				p->input[i]->textcolor(labelcolor());
			}
			cy+=20;
			n--;
		}	
	}
	else if (n<0)
	{
		while (n)
		{
			remove(child(children()-1));
			cy+=20;
			n--;
		}	
	}
	UpdateSize();
}

void PoleZeroList::UpdateSize()
{
	size(w(),children()*20);
	parent()->redraw();
}

void PoleZeroList::RecalcPositions(void)
{
	int cy = y();
	for (int i=0;i<children();i++)
	{
		child(i)->position(child(i)->x(),cy);
		cy += child(i)->h();
	}
	UpdateSize();
}
