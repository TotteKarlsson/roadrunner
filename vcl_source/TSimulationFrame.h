#ifndef TSimulationFrameH
#define TSimulationFrameH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "mtkFloatLabeledEdit.h"
#include "mtkIntLabeledEdit.h"
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include "rrc_types.h"
#include <Vcl.CheckLst.hpp>
#include "rrStringList.h"

using namespace rr;
using namespace rrc;

typedef void __fastcall (__closure *TOnSimulationFinished)();

//---------------------------------------------------------------------------
class TSimulateFrame : public TFrame
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	mtkIntLabeledEdit *mNrOfSimulationPointsE;
	mtkFloatLabeledEdit *mEndTimeE;
	mtkFloatLabeledEdit *mStartTimeE;
	TPanel *Panel1;
	TActionList *ActionList1;
	TPanel *Panel2;
	TButton *simBtn;
	TAction *simulateA;
	TCheckListBox *SelList;
	TGroupBox *GroupBox2;
	TPanel *Panel3;
	TGroupBox *GroupBox3;
	TCheckListBox *paraListB;
	TSplitter *Splitter1;
	void __fastcall SelListClick(TObject *Sender);
	void __fastcall simulateAExecute(TObject *Sender);


    private:	// User declarations
        RRHandle					mRRHandle;
        RRJobHandle					mSimJobH;
//        void						AddItemsToListBox(const StringList& items);

        static void __stdcall	   	ThreadEnterCB(void *UserData);					//These need to be static in order to be passed to the C API thread function
        static void __stdcall	   	ThreadExitCB(void *UserData);

        void 		__fastcall     	simulationStarted();
        void 		__fastcall     	simulationFinished();

	public:		// User declarations
        void		__fastcall		callBack();
                    __fastcall     	TSimulateFrame(TComponent* Owner);
        TMemo			           *infoMemo;	//For the logger
        void			           	assignRRHandle(RRHandle aHandle);
        bool			           	loadSelectionList();
        void 					    EnableDisableSimulation();

        RRJobHandle	__fastcall		simulate();
        TOnSimulationFinished	   	onSimulationFinished;
};
//---------------------------------------------------------------------------
extern PACKAGE TSimulateFrame *SimulateFrame;
//---------------------------------------------------------------------------
#endif