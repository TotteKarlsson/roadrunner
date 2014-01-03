#pragma hdrstop
#include <sstream>
#include "rrLogger.h"
#include "rrAutoPlugin.h"
#include "rrc_api.h"
#include "rrc_utilities.h"
#include "rrRoadRunner.h"
#include "rrRoadRunnerData.h"
#include "rrUtils.h"
//---------------------------------------------------------------------------
namespace autoplugin
{

//using namespace rr;
using namespace rrc;
using namespace std;

AutoPlugin::AutoPlugin(rr::RoadRunner* aRR)
:
CPPPlugin("Auto-2000", "Bifurcation", aRR, NULL),
//The Capability
mAuto(                              "Auto",               "Bifurcation"),
mTempFolder(                        "<none>",               "TempFolder",                                       "Tempfolder used by auto"),
mSBML(                              "<none>",               "SBML",                                             "SBML, i.e. the model to be used to analyze"),
mScanDirection(                     "Negative",             "ScanDirection",                                    "Direction of parameter scan"),
mPrincipalContinuationParameter(    "<none>",               "PrincipalContinuationParameter",                   "Principal Continuation Property"),
mPCPLowerBound(                     0,                      "PCPLowerBound",                                    "Principal Continuation Parameter Lower Bound"),
mPCPUpperBound(                     0,                      "PCPUpperBound",                                    "Principal Continuation Parameter Upper Bound"),
mBiFurcationDiagram(                "<none>",               "BiFurcationDiagram",                               "BifurcationDiagram"),
mAutoData(                          AutoData(),             "AutoData",                                         "Data structure holding auto data"),
mRRAuto(aRR, mAutoData.getValueReference()),
mAutoWorker(*this)
{
    //Setup the plugins capabilities
    mAuto.addProperty(&mTempFolder);
    mAuto.addProperty(&mSBML);
    mAuto.addProperty(&mAutoData);
    mAuto.addProperty(&mScanDirection);
    mAuto.addProperty(&mPrincipalContinuationParameter);
    mAuto.addProperty(&mPCPLowerBound);
    mAuto.addProperty(&mPCPUpperBound);
    mAuto.addProperty(&mBiFurcationDiagram);
    mProperties.add(mAuto);
}

AutoPlugin::~AutoPlugin()
{}

RRAuto& AutoPlugin::getRRAuto()
{
    return mRRAuto;
}

bool AutoPlugin::assignRoadRunnerInstance(RoadRunner* rr)
{
    mRRAuto.assignRoadRunner(rr);
    mRR = rr;
    return mRR ? true : false;
}

void AutoPlugin::setScanDirection(ScanDirection dir)
{
    if(dir == sdPositive)
    {
        mScanDirection.setValue("Positive");
    }
    else
    {
        mScanDirection.setValue("Negative");
    }
}

void AutoPlugin::setPrincipalContinuationParameter(const string& para)
{
    mPrincipalContinuationParameter.setValue(para);
}

void AutoPlugin::setPCPLowerBound(double val)
{
    mPCPLowerBound.setValue(val);
}

void AutoPlugin::setPCPUpperBound(double val)
{
    mPCPUpperBound.setValue(val);
}

void AutoPlugin::setSBML(const string& val)
{
    mSBML.setValue(val);
}

string AutoPlugin::getConstants()
{
    return mRRAuto.getConstantsAsString();
}

bool AutoPlugin::isWorking()
{
    return mAutoWorker.isRunning();
}

string AutoPlugin::getStatus()
{
    stringstream msg;
    msg<<Plugin::getStatus();
    msg<<"TempFolder: "<<mTempFolder<<"\n";
    msg<<"SBML: "<<mSBML<<"\n";
    AutoData* autoData = (AutoData*) (mAutoData.getValuePointer());
    msg<<"MinData"<<(*autoData)<<"\n";
    return msg.str();
}

bool AutoPlugin::resetPlugin()
{
    if(mAutoWorker.isRunning())
    {
        return false;
    }

    AutoData& data = getAutoData();
//    data.reset();
    return true;
}

AutoData& AutoPlugin::getAutoData()
{
    return *(mAutoData.getValuePointer());
}

string AutoPlugin::getTempFolder()
{
    return mTempFolder.getValue();
}

string AutoPlugin::getSBML()
{
    return mSBML.getValue();
}

string AutoPlugin::getResult()
{
    stringstream msg;
    AutoData& data = getAutoData();

    msg<<data;
    return msg.str();
}

bool AutoPlugin::setInputData(void* inputData)
{
    return true;
}

bool AutoPlugin::execute(bool inThread)
{
    Log(rr::lInfo)<<"Executing the AutoPlugin plugin";

    //go away and carry out the work in a thread
    //Assign callback functions to communicate the progress of the thread

    //mAutoWorker.assignCallBacks(mWorkStartedEvent, mWorkFinishedEvent, mUserData);
    mAutoWorker.start(inThread);
    return true;
}

// Plugin factory function
AutoPlugin* plugins_cc createPlugin(rr::RoadRunner* aRR)
{
    //allocate a new object and return it
    return new AutoPlugin(aRR);
}

const char* plugins_cc getImplementationLanguage()
{
    return "CPP";
}

}
