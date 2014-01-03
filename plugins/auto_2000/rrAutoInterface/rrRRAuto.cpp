#pragma hdrstop
#include "rrUtils.h"
#include "rrRRAuto.h"
#include "rrExecutableModel.h"
#include "../libAuto/vsAuto.h"
#include "rrLogger.h"
#include "rrStringList.h"
#include "rrPluginUtils.h"

namespace rrauto
{
using namespace rr;
using namespace rrp;
using namespace autolib;

//Statics
RoadRunner*     RRAuto::mRR = NULL;
SetupControl    RRAuto::mAutoSetup;
string          RRAuto::mSelectedParameter = gEmptyString;

RRAuto::RRAuto(RoadRunner* rr, AutoData& data)
:
mAutoData(data)
{
    mRR = rr;
}

RRAuto::~RRAuto()
{}

void RRAuto::assignRoadRunner(RoadRunner* rrInstance)
{
    mRR = rrInstance;
}

AutoData& RRAuto::getAutoData()
{
    return mAutoData;
}

bool RRAuto::selectParameter(const string& para)
{
    mSelectedParameter = para;
//    mAutoSetup.mParameter = &para;
    return false;
}

bool RRAuto::setStartParameterValue(const double& val)
{
    mAutoSetup.mInputConstants.RL0 = val;
    return false;
}

bool RRAuto::setEndParameterValue(const double& val)
{
    mAutoSetup.mInputConstants.RL1 = val;
    return false;
}

bool RRAuto::setScanDirection(ScanDirection val)
{
    mAutoSetup.mDirectionPositive = (val == sdPositive) ? true : false;

    if(mAutoSetup.mDirectionPositive)
    {
        mAutoSetup.mInputConstants.DS = fabs(mAutoSetup.mInputConstants.DS);
    }
    else
    {
        mAutoSetup.mInputConstants.DS = -1 * fabs(mAutoSetup.mInputConstants.DS);
    }
    return false;
}

string RRAuto::getTempFolder()
{
    return (mRR) ? mRR->getTempFolder() : string("");
}

bool RRAuto::run()
{
    if(!mRR)
    {
        return false;
    }
    reset();
    mAutoSetup.mRunContinuation = false;

    double parValue = (mAutoSetup.mDirectionPositive) ? mAutoSetup.mInputConstants.RL0 : mAutoSetup.mInputConstants.RL1;

    mRR->setValue(mSelectedParameter, parValue);


    if(mAutoSetup.mCalculateSteadyState)
    {
        mRR->steadyState();
    }

    if(!setupUsingCurrentModel())
    {
        return false;
    }

    CallAuto();
    //Parse output;
    string f7(getFileContent(joinPath(getTempFolder(), "fort.7")));
    mAutoData.setBifurcationDiagram(f7);

    return true;
}

bool RRAuto::setupUsingCurrentModel()
{
    int ndim = mRR->getSteadyStateSelections().size();
    mAutoSetup.mInputConstants.NDIM = ndim;

    setCallbackStpnt(ModelInitializationCallback);
    setCallbackFunc2(ModelFunctionCallback);

    string temp = mAutoSetup.getConstantsAsString();
    setFort2File(temp);
    return true;
}

void RRAuto::setFort2File(const string& text)
{
    mFort2File = text;
    autolib::setFort2File(mFort2File.c_str(), mFort2File.size());
}

string RRAuto::getConstantsAsString()
{
    return mAutoSetup.getConstantsAsString();
}

//Called by Auto
int autoCallConv RRAuto::ModelInitializationCallback(long ndim, double t, double* u, double* par)
{
    ExecutableModel* lModel = mRR->getModel();

    int numBoundaries = 0;//mRR->getNumberOfBoundarySpecies();// //SelectForm.NumSelectedBoundaries;
    int numParameters = 1; // SelectForm.NumSelectedParameters;

    double* oBoundary           = new double[numBoundaries];
    double* oGlobalParameters   =  new double[numParameters];

    if (numBoundaries > 0)
    {
        //int[] oSelectedBoundary = SelectForm.SelectedBoundarySpecies;
        int* oSelectedBoundary = new int[1];
//        oSelectedBoundary[0] = 0;   //This is 'A'

        for (int i = 0; i < numBoundaries; i++)
        {
            //oBoundary[i] = Simulator.getBoundarySpeciesByIndex(oSelectedBoundary[i]);
            oBoundary[i] = mRR->getBoundarySpeciesByIndex(oSelectedBoundary[i]);
        }
    }

    if (numParameters > 0)
    {
        //int[] oSelectedParameters = SelectForm.SelectedParameters;
//        vector<int> oSelectedParameters(0);
//        double val  = mRR->getParameterDoubleValue()(mSelectedParameter);

        double val  = mRR->getValue(mSelectedParameter);

        oGlobalParameters[0] = val;

//        for (int i = 0; i < numParameters; i++)
//        {
//            oGlobalParameters[i] = mRR->getGlobalParameterByIndex(oSelectedParameters[i]);
//        }
    }

    double* oParameters = new double[numBoundaries + numParameters];
    int oParaSize = numBoundaries + numParameters;
//    Array.Copy(oBoundary, oParameters, oBoundary.Length);
    for(int i = 0; i < numBoundaries; i++)
    {
        oParameters[i] = oBoundary[i];
    }

    //Array.Copy(oGlobalParameters, 0, oParameters, oBoundary.Length, oGlobalParameters.Length);
    for(int i = 0; i < numParameters; i++)
    {
        oParameters[numBoundaries + i] = oGlobalParameters[i];
    }

    //Marshal.Copy(oParameters, 0, par, oParameters.Length);
    for(int i = 0; i < oParaSize; i++)
    {
        par[i] = oParameters[i];
    }

    //Marshal.Copy(CurrentModel.y, 0, u, Math.Min(CurrentModel.y.Length, ndim));

    int nrFloatingSpecies = lModel->getNumFloatingSpecies();
    double* floatCon = new double[nrFloatingSpecies];
    lModel->getFloatingSpeciesConcentrations(nrFloatingSpecies, NULL, floatCon);

    int nMin = min(nrFloatingSpecies, ndim);

    for(int i = 0; i < nMin; i++)
    {
        u[i] = floatCon[i];
    }
    return 0;
}


//---------------------------------------------------------------------------
//private int ModelInitializationCallback(int ndim, double t, IntPtr u, IntPtr par)
//        {
//            nDim = ndim;
//
//            int numBoundaries = SelectForm.NumSelectedBoundaries;
//            int numParameters = SelectForm.NumSelectedParameters;
//
//            var oBoundary = new double[numBoundaries];
//            var oGlobalParameters = new double[numParameters];
//
//            if (numBoundaries > 0)
//            {
//                int[] oSelectedBoundary = SelectForm.SelectedBoundarySpecies;
//                for (int i = 0; i < numBoundaries; i++)
//                {
//                    oBoundary[i] = Simulator.getBoundarySpeciesByIndex(oSelectedBoundary[i]);
//                }
//            }
//
//
//            if (numParameters > 0)
//            {
//                int[] oSelectedParameters = SelectForm.SelectedParameters;
//                for (int i = 0; i < numParameters; i++)
//                {
//                    oGlobalParameters[i] = Simulator.getGlobalParameterByIndex(oSelectedParameters[i]);
//                }
//            }
//
//            var oParameters = new double[numBoundaries + numParameters];
//
//            Array.Copy(oBoundary, oParameters, oBoundary.Length);
//            Array.Copy(oGlobalParameters, 0, oParameters, oBoundary.Length, oGlobalParameters.Length);
//
//            Marshal.Copy(oParameters, 0, par, oParameters.Length);
//
//            Marshal.Copy(CurrentModel.y, 0, u, Math.Min(CurrentModel.y.Length, ndim));
//
//            return 0;
//        }

void autoCallConv RRAuto::ModelFunctionCallback(const double* oVariables, const double* par, double* oResult)
{
    int numBoundaries = 0;
    int numParameters = 1;
    ExecutableModel* lModel = mRR->getModel();

    if (numBoundaries > 0)
    {
        vector<double> oBoundary(numBoundaries);
        for(int i = 0; i < numBoundaries; i++)
        {
            oBoundary[i] = par[i];
        }

        vector<int> oSelectedBoundary(1);
        oSelectedBoundary[0] = 0;

        for (int i = 0; i < numBoundaries; i++)
        {
            double val = oBoundary[i];
            mRR->setBoundarySpeciesByIndex(oSelectedBoundary[i], val);
        }

    }

    if (numParameters > 0)
    {
        double* oParameters = new double[numParameters];
//        Marshal.Copy(par, oParameters, numBoundaries, numParameters);
        for(int i = 0; i < numParameters; i ++)
        {
            oParameters[i] = par[i];
        }

        mRR->setValue(mSelectedParameter, oParameters[0]);
//        int[] oSelectedParameters = SelectForm.SelectedParameters;
//        for (int i = 0; i < numParameters; i++)
//        {
//            Simulator.setGlobalParameterByIndex(oSelectedParameters[i],
//                                                (double.IsNaN(oParameters[i])
//                                                     ? oSelectedParameters[i]
//                                                     : oParameters[i]));
//        }
    }
//
//
    static vector<rr::SelectionRecord> selRecs = mRR->getSteadyStateSelections();
    static rr::StringList selList = getRecordsAsStrings(selRecs);
//    vector<string> selList = mRR->getSteadyStateSelections();
//    var variableTemp = new double[CurrentModel.y.Length];

    static vector<double> variableTemp(selList.size());
    int ndim = mAutoSetup.mInputConstants.NDIM;
    int nMin = min(selList.size(), ndim);

//    Marshal.Copy(oVariables, variableTemp, 0, Math.Min(CurrentModel.y.Length, nDim));
    for (int i = 0; i < nMin; i++)
    {
        variableTemp[i] = oVariables[i];
    }
    int numFloatingSpecies = lModel->getNumFloatingSpecies();

//    bool containsNaN = ContainsNaN(variableTemp);
//    if (!containsNaN)
    {
//        CurrentModel.y = variableTemp;

        double* tempConc = new double[numFloatingSpecies];
        for(int i = 0; i < numFloatingSpecies; i++)
        {
            tempConc[i] = variableTemp[i];
        }

        lModel->setFloatingSpeciesConcentrations(numFloatingSpecies, NULL, tempConc);
    }

    Log(lDebug)<<"Eval";

    //PrintArray(CurrentModel.y, Console.Out);

    lModel->convertToAmounts();

//    CurrentModel.evalModel(CurrentModel.time, CurrentModel.y);

    double time = lModel->getTime();
    int stateVecSize = lModel->getNumFloatingSpecies() + lModel->getNumRules();
    double* dydts = new double[stateVecSize];
    lModel->evalModel(time, NULL, dydts);

    nMin = min(lModel->getNumRules(), ndim);

//    Marshal.Copy(CurrentModel.dydt, 0, oResult, Math.Min(CurrentModel.dydt.Length, nDim));
    for(int i = 0; i < 2; i++)
    {
        oResult[i] = dydts[i];
    }

}

//private void ModelFunctionCallback(IntPtr oVariables, IntPtr par, IntPtr oResult)
//        {
//            int numBoundaries = SelectForm.NumSelectedBoundaries;
//            int numParameters = SelectForm.NumSelectedParameters;
//
//            if (numBoundaries > 0)
//            {
//                var oBoundary = new double[numBoundaries];
//                Marshal.Copy(par, oBoundary, 0, numBoundaries);
//                int[] oSelectedBoundary = SelectForm.SelectedBoundarySpecies;
//                for (int i = 0; i < numBoundaries; i++)
//                {
//                    Simulator.setBoundarySpeciesByIndex(oSelectedBoundary[i],
//                                                        (double.IsNaN(oBoundary[i])
//                                                             ? oSelectedBoundary[i]
//                                                             : oBoundary[i]));
//                }
//            }
//
//            if (numParameters > 0)
//            {
//                var oParameters = new double[numParameters];
//                Marshal.Copy(par, oParameters, numBoundaries, numParameters);
//                int[] oSelectedParameters = SelectForm.SelectedParameters;
//                for (int i = 0; i < numParameters; i++)
//                {
//                    Simulator.setGlobalParameterByIndex(oSelectedParameters[i],
//                                                        (double.IsNaN(oParameters[i])
//                                                             ? oSelectedParameters[i]
//                                                             : oParameters[i]));
//                }
//            }
//
//
//            var variableTemp = new double[CurrentModel.y.Length];
//            Marshal.Copy(oVariables, variableTemp, 0, Math.Min(CurrentModel.y.Length, nDim));
//
//            bool containsNaN = ContainsNaN(variableTemp);
//            if (!containsNaN)
//            {
//                CurrentModel.y = variableTemp;
//            }
//
//            //Console.WriteLine("Eval");
//            //PrintArray(CurrentModel.y, Console.Out);
//
//            CurrentModel.convertToAmounts();
//            CurrentModel.evalModel(CurrentModel.time, CurrentModel.y);
//            //Simulator.oneStep(0.0, 0.01);
//
//            Marshal.Copy(CurrentModel.dydt, 0, oResult, Math.Min(CurrentModel.dydt.Length, nDim));
//
//
//            //PrintArray(CurrentModel.y, Console.Out);
//            //PrintArray(CurrentModel.dydt, Console.Out);
//        }


bool RRAuto::reset()
{
    //Remove temporary files
    return true;
}

} //namespace au


