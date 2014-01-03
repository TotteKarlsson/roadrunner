#ifndef rrRRAutoH
#define rrRRAutoH
#include "rrProperty.h"
#include "rrRoadRunner.h"
#include "rrLogger.h"
#include "rrAutoData.h"
#include "rrRRAutoSetupControl.h"
#include "rrRRAutoExporter.h"
//---------------------------------------------------------------------------

#define autoCallConv __cdecl

//This class serves as a RoadRunner-Auto interface. It needs a roadrunner instance on creation
namespace rrauto
{

using std::string;
using rr::RoadRunner;
enum ScanDirection {sdPositive, sdNegative};

class RRA_DECLSPEC RRAuto
{
    public:
                                    RRAuto(RoadRunner* rr, AutoData& data);
                                   ~RRAuto();
        AutoData&                   getAutoData();
        string                      getTempFolder();
        void                        setFort2File(const string& text);
        bool                        setupUsingCurrentModel();
        string                      getConstantsAsString();
        bool                        run();
        bool                        reset();
        bool                        selectParameter(const string& para);
        bool                        setStartParameterValue(const double& val);
        bool                        setEndParameterValue(const double& val);
        bool                        setScanDirection(ScanDirection val);
        void                        assignRoadRunner(RoadRunner* rrInstance);

    private:
        static string               mSelectedParameter;
        AutoData&                   mAutoData;

        static RoadRunner*          mRR;         //Static so we can access this in autos callback
        static SetupControl         mAutoSetup;  //Auto constants, parameters

        string                      mFort2File;
        static int  autoCallConv    ModelInitializationCallback(long ndim, double t, double* u, double* par);
        static void autoCallConv    ModelFunctionCallback(const double* oVariables, const double* par, double* oResult);
};


}

namespace rrp
{

template<>
inline string getParaType<rrauto::ScanDirection>(const rrauto::ScanDirection& a)
{
    return "ScanDirection";
}
}

#endif