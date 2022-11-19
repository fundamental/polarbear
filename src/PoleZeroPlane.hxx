#ifndef __PoleZeroPlane__
#define __PoleZeroPlane__

#include <FL/Fl_Widget.H>
#include "Complex.hxx"
#include "EMode.hxx"

class ComplexExtArray;
class PoleZeroPlane_List;

/* action set when callback is called for PoleZeroPlan */
enum EPoleZeroPlaneAction
{
	kActionAdd, /* new pole/zero added */
	kActionAddPair, /* new pole/zero conjugate pair added */
	kActionRemove, /* remove selected pole/zero */
	kActionRemoved, /* removal done */
	kActionSelectAll, /* select all */
	kActionUnselectAll, /* unselect all */
	kActionSelect, /* select/unselect pole/zero */
	kActionMove, /* move selected poles/zeros */
};

class PoleZeroPlane:public Fl_Widget
{
public:
	double mXCenter;
	double mYCenter;
	double mRadius;

	EMoveMode mMoveMode;
	EAddMode mAddMode;

	Complex mPos;
	Complex mPrevPos;
	
	ComplexExtArray *mExtArray[2];
	
	PoleZeroPlane(int X,int Y,int W,int H);

	EPoleZeroPlaneAction mAction;
	EPoleZero	mActionPoleZero;
	int mActionId;
	

	int handle(int e);
	void draw(void);

	void SetMoveMode(EMoveMode mode)
	{
		mMoveMode = mode;
	}

	void SetAddMode(EAddMode mode)
	{
		mAddMode = mode;
	}

	void SetExtArrays(const EPoleZero& pz,ComplexExtArray* ptr)
	{
		mExtArray[int(pz)] = ptr;
	}

	void SelectAll(void);
	void RemoveSelected(void);
private:
	void Add(const Complex& pos,const EAddMode& mode,const bool& mirror);
	Complex EventPos(void);
	void UnselectAll(void);
	void ToggleSelect(EPoleZero pz,int i);
	void MoveSelected(
		const Complex& pos,const Complex& prevPos,const EMoveMode& mode);
	void Select(const Complex& pos,bool toggle,bool add);
	void Select(EPoleZero pz,int i);

	void resize(int X,int Y,int W,int H);
};

#endif
