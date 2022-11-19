#include "CoefArray.hxx"
#include "ComplexArray.hxx"
#include "ComplexExtArray.hxx"
#include "PoleZeroPlane.hxx"
#include "PoleZeroList.hxx"
#include "PoleZeroEditorGUI.hxx"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Filter.hxx"

void FindCoefs(ComplexArray& complexCoefs,ComplexArray& roots)
{
/* based on Numerical Recipes page 569 */
	int i,j,n=roots.size;

	complexCoefs[n] = -roots[0];
	complexCoefs[n-1] = Complex(1,0);
	complexCoefs.size = n+1;
	
	for (j=1;j<n;j++)
	{
		complexCoefs[n-j-1] = Complex(1,0);
		for (i=n-j;i<n;i++)
		{
			complexCoefs[i] = complexCoefs[i+1] - (roots[j]*complexCoefs[i]);
		}
		complexCoefs[n] = -(roots[j]*complexCoefs[n]);
	}
}

void FindCoefs(CoefArray& coefs,ComplexArray& roots)
{
	int i;
	ComplexArray complexCoefs;
	FindCoefs(complexCoefs,roots);
	for (i=0;i<complexCoefs.size;i++)
	{
		coefs[i] = complexCoefs[i].re;
	}
	coefs.size = complexCoefs.size;
}

class PoleZeroEditor
{
private:
	ComplexExtArray mExtArray[2];
	PoleZeroEditorGUI mGUI;
	ComplexArray mPoles;
	ComplexArray mZeros;
	CoefArray mACoefs;
	CoefArray mBCoefs;

public:
	Filter mFilter[2];
	
	PoleZeroEditor(void);	

	void Init(void);

	void Start(void);

	void PoleZeroPlaneCB(void);
	void GainSliderCB(void);

	void CalcCoefs(void);
	void UpdateCoefLists(void);
	void UpdateCoefList(Fl_Browser* b,CoefArray& coefs);
		
	static void _PoleZeroPlaneCB(PoleZeroPlane* plane,PoleZeroEditor* pThis)
	{
		pThis->PoleZeroPlaneCB();
	}
	static void _GainSliderCB(Fl_Slider* plane,PoleZeroEditor* pThis)
	{
		pThis->GainSliderCB();
	}
	
	void StartAudio(void);
	void StopAudio(void);
	
	static void _PlayButtonCB(Fl_Button* b,PoleZeroEditor* pThis)
	{
		pThis->StartAudio();
	}
	static void _StopButtonCB(Fl_Button* b,PoleZeroEditor* pThis)
	{
		pThis->StopAudio();
	}
};

PoleZeroEditor::PoleZeroEditor(void)
{
	mExtArray[kPole].array = &mPoles;
	mExtArray[kZero].array = &mZeros;

	mGUI.mAmplitudeResponse->mPoles = &mPoles;
	mGUI.mAmplitudeResponse->mZeros = &mZeros;
	mGUI.mPhaseResponse->mPoles = &mPoles;
	mGUI.mPhaseResponse->mZeros = &mZeros;

	mGUI.mPoleZeroPlane->mExtArray[0] = &mExtArray[0];
	mGUI.mPoleZeroPlane->mExtArray[1] = &mExtArray[1];
	
	mGUI.mPolesList->array = &mExtArray[kPole];
	mGUI.mZerosList->array = &mExtArray[kZero];
}

void PoleZeroEditor::Init(void)
{
	mGUI.mPoleZeroPlane->callback(
		(Fl_Callback*) _PoleZeroPlaneCB,(void*)this);

	mGUI.mGainSlider->callback(
		(Fl_Callback*) _GainSliderCB,(void*)this);
	
	mGUI.mGainSlider->value(1);

	mGUI.mMoveXYButton->value(1);
	mGUI.mMoveXYButton->do_callback();

	mGUI.mAddPoleButton->value(1);
	mGUI.mAddPoleButton->do_callback();

	mGUI.mPlayButton->activate();
	mGUI.mPlayButton->callback(
		(Fl_Callback*) _PlayButtonCB,(void*)this);
	mGUI.mStopButton->activate();
	mGUI.mStopButton->callback(
		(Fl_Callback*) _StopButtonCB,(void*)this);

	UpdateCoefLists();
}

void PoleZeroEditor::Start(void)
{
	mGUI.mWindow->show();
}

void PoleZeroEditor::StartAudio(void)
{
	mFilter[0].active = 1;
	mFilter[1].active = 1;
}

void PoleZeroEditor::StopAudio(void)
{
	mFilter[0].active = 0;
	mFilter[1].active = 0;
}


void PoleZeroEditor::GainSliderCB(void)
{
	mFilter[0].gain = mFilter[1].gain =  mGUI.mGainSlider->value();
}

void PoleZeroEditor::PoleZeroPlaneCB(void)
{
	ComplexExtArray** extArray = mGUI.mPoleZeroPlane->mExtArray;
	PoleZeroList *list[2];
	list[kPole] = mGUI.mPolesList;
	list[kZero] = mGUI.mZerosList;
	
	switch (mGUI.mPoleZeroPlane->mAction)
	{
		case kActionAdd:
		{
			int pz = mGUI.mPoleZeroPlane->mActionPoleZero;
			list[pz]->Add();
			list[pz]->Sync(extArray[pz]->Size()-1);
			list[pz]->redraw();
		}
		UpdateCoefLists();
		mGUI.mAmplitudeResponse->redraw();
		mGUI.mPhaseResponse->redraw();
		break;
		case kActionAddPair:
		{
			int pz = mGUI.mPoleZeroPlane->mActionPoleZero;
			list[pz]->Add();
			list[pz]->Add();
			list[pz]->Sync(extArray[pz]->Size()-2);
			list[pz]->Sync(extArray[pz]->Size()-1);
			list[pz]->redraw();
		}
		UpdateCoefLists();
		mGUI.mAmplitudeResponse->redraw();
		mGUI.mPhaseResponse->redraw();
		break;
		case kActionRemove:
		{
			int pz = mGUI.mPoleZeroPlane->mActionPoleZero;
			if (list[pz])
			{
				int j;
				for (j=extArray[pz]->Size()-1;j>=0;j--)
				{
					if (extArray[pz]->Selected(j))
					{
						list[pz]->remove(list[pz]->child(j));
						list[pz]->RecalcPositions();
					}
				}
			}
		}
		UpdateCoefLists();
		mGUI.mAmplitudeResponse->redraw();
		mGUI.mPhaseResponse->redraw();
		break;
		case kActionRemoved:
		{
			UpdateCoefLists();
		}
		break;
		case kActionSelectAll:
		case kActionUnselectAll:
		{
			int pz;
			for (pz=0;pz<2;pz++)
			{
				if (list[pz])
				{
					int j;
					for (j=0;j<extArray[pz]->Size();j++)
						list[pz]->SyncSelect(j);
				}
			}
		}
		break;
		case kActionSelect:
		{
			int pz = mGUI.mPoleZeroPlane->mActionPoleZero;
			int id = mGUI.mPoleZeroPlane->mActionId;
			if (list[pz])
			{
				list[pz]->SyncSelect(id);
			}
		}
		break;
		case kActionMove:
		{
			int pz = mGUI.mPoleZeroPlane->mActionPoleZero;
			int j;
			for (j=0;j<extArray[pz]->selection.size;j++)
			{
				list[pz]->SyncPos(extArray[pz]->selection.data[j]);
			}
		}
		UpdateCoefLists();
		mGUI.mAmplitudeResponse->redraw();
		mGUI.mPhaseResponse->redraw();
		break;
	}
}

void PoleZeroEditor::CalcCoefs(void)
{
	if (mZeros.size>0)
		FindCoefs(mACoefs,mZeros);	
	else
		mACoefs.size=1;
		mACoefs[0] = 1;
		
	if (mPoles.size>0)
		FindCoefs(mBCoefs,mPoles);
	else
		mBCoefs.size=0;
	
	mFilter[0].a = mACoefs.data;
	mFilter[1].a = mACoefs.data;
	mFilter[0].M = mACoefs.size;
	mFilter[1].M = mACoefs.size;
	mFilter[0].b = mBCoefs.data;
	mFilter[1].b = mBCoefs.data;
	mFilter[0].N = mBCoefs.size;
	mFilter[1].N = mBCoefs.size;
}

void PoleZeroEditor::UpdateCoefList(Fl_Browser* b,CoefArray& coefs)
{
	int i;
	b->clear();
	
	for (i=0;i<coefs.size;i++)
	{
		char tmp[32];
		sprintf(tmp,"%f",coefs[i]);
		b->add(tmp);
	}
}

void PoleZeroEditor::UpdateCoefLists(void)
{
	CalcCoefs();

	UpdateCoefList(mGUI.mACoefsBrowser,mACoefs);
	UpdateCoefList(mGUI.mBCoefsBrowser,mBCoefs);
}

void PoleZeroEditorGUI::SetMoveMode(EMoveMode m)
{
	mPoleZeroPlane->SetMoveMode(m);
}

void PoleZeroEditorGUI::SetAddMode(EAddMode m)
{
	mPoleZeroPlane->SetAddMode(m);
}

void PoleZeroEditorGUI::SelectAll(void)
{
	mPoleZeroPlane->SelectAll();
}

void PoleZeroEditorGUI::RemoveSelected(void)
{
	mPoleZeroPlane->RemoveSelected();
}

void PoleZeroEditorGUI::Edited(EPoleZero pz)
{
	/* called through PoleZeroList callback, which sets
	mEditedId (index in array), mEditedType (re/im), and mEditedValue (string
	in input)
	*/
	ComplexExtArray** extArray = mPoleZeroPlane->mExtArray;
	PoleZeroList* list;
	char* endptr;

	if (pz==kPole) list = mPolesList;
	if (pz==kZero) list = mZerosList;
	
	if (list->mEditedType == 0)
		(*extArray[pz]->array)[list->mEditedId].re = 
			strtod(list->mEditedValue,&endptr);
	else
		(*extArray[pz]->array)[list->mEditedId].im =
			strtod(list->mEditedValue,&endptr);

	mPoleZeroPlane->redraw();		
}

#include <FL/Fl.H>

PoleZeroEditor* editor = 0;
Filter* filter[2] = {0,0};

void init_gui(void)
{
	editor = new PoleZeroEditor;
	filter[0] = &editor->mFilter[0];
	filter[1] = &editor->mFilter[1];
}

void run_gui(void)
{
	editor->Init();
	editor->Start();

	Fl::run();

	filter[0] = 0;
	filter[1] = 0;
	delete editor;
	editor = 0;
}
