#pragma hdrstop
#include "rrRoadRunnerData.h"
#include "rrLogger.h"
#include "rrAutoWorker.h"
#include "rrAutoPlugin.h"
#include "lmfit/lmmin.h"
#include "rrStringUtils.h"
#include "rrUtils.h"
#include "rrc_api.h"
#include "rrc_utilities.h"
//---------------------------------------------------------------------------

namespace autoplugin
{

using namespace rr;

AutoWorker::AutoWorker(AutoPlugin& host)
:
threadEnterCB(NULL),
threadExitCB(NULL),
mUserData(NULL),
mTheHost(host),
mAutoData(mTheHost.getAutoData()),
mRRAuto(mTheHost.getRRAuto())
{}

AutoWorker::~AutoWorker()
{}

bool AutoWorker::isRunning()
{
    return mThread.isRunning();
}

void AutoWorker::assignCallBacks(ThreadCB fn1, ThreadCB fn2, void* userData)
{
    threadEnterCB     = fn1;
    threadExitCB      = fn2;
    mUserData         = userData;
}

bool AutoWorker::start(bool runInThread)
{
    if(runInThread)
    {
        if(mThread.isRunning())
        {
            Log(lError)<<"Tried to start an already working thread!";
            return false;
        }

        mThread.start(*this);    //Will spawn the run function below in a thread
    }
    else
    {
        run();
    }
    return true;
}

void AutoWorker::run()
{
    if(threadEnterCB)
    {
        threadEnterCB(mUserData);    //Tell anyone who wants to know
    }

    if(!setup())
    {
        Log(lError)<<"Failed to setup auto..";
        if(threadExitCB)
        {
            threadExitCB(mUserData);
        }
        return;
    }

    //Do the AUTO thing, pass data from the outside here..
    mRRAuto.selectParameter(mTheHost.mPrincipalContinuationParameter.getValue());
    double spvL = mTheHost.mPCPLowerBound.getValue();
    double spvU = mTheHost.mPCPUpperBound.getValue();

    mRRAuto.setStartParameterValue(spvL);
    mRRAuto.setEndParameterValue(spvU);
    mRRAuto.setScanDirection(sdNegative);

    ScanDirection sDirection = mTheHost.mScanDirection.getValue();

    string str = mRRAuto.getConstantsAsString();
    Log(lInfo)<<str;

    if(!mRRAuto.run())
    {
        Log(lError)<<"There was a problem running auto";
    }

    mTheHost.mBiFurcationDiagram.setValue(mAutoData.getBifurcationDiagram());
    if(threadExitCB)
    {
        threadExitCB(mUserData);
    }
}

bool AutoWorker::setup()
{
    //return false if roadrunner have not loaded a model
    if(mTheHost.mRR->isModelLoaded())
    {
        Log(lInfo)<<"Model loaded by roadrunner outside of auto..";
        return true;
    }
    else
    {
        return mTheHost.mRR->load(mTheHost.getSBML());
    }

}

}


