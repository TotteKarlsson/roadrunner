#pragma hdrstop
#include "rrRoadRunner.h"
#include "rrPluginManager.h"
#include "rrException.h"
#include "rrLogger.h"
#include "rrPlugin.h"
#include "rrUtils.h"
#include "../../source/rrAutoPlugin.h"
#include "../../rrAutoInterface/rrRRAuto.h"

using namespace rr;
using namespace rrp; //Plugin stuff
using namespace autoplugin;

int main()
{
    string tempFolder("r:/temp");
    string sbmlFile("../models/bistable.xml");

    gLog.setLevel(lInfo);
    gLog.enableConsoleLogging();
    gLog.enableFileLogging(joinPath(tempFolder, "bistable.log"));
    RoadRunner*  rri = NULL;
    try
    {
        string pluginName("rrp_auto2000");

        //A roadrunner object
        rri = new RoadRunner();
        rri->setTempFileFolder(tempFolder);

        //A Plugin manager, using a roadrunner instance
        PluginManager pm;

        //Where to load plugins?
        if(!pm.setPluginDir("../plugins"))
        {
            Log(lError)<<"The setPluginDir returned false..";
            return 0;
        }

        //Load auto plugin
        if(!pm.load(pluginName))
        {
            Log(lError)<<"Failed to load auto plugin";
            return 0;
        }

        //Get a plugin handle and get some plugin info
        AutoPlugin* autoPlugin = (AutoPlugin*) pm.getPlugin(pluginName);
        if(!autoPlugin)
        {
            Log(lError)<<"Problem..";
            throw("AutoPlugin don't exist");
        }

        Log(lInfo)<<autoPlugin->getExtendedInfo();

        //A serious client would check if these calls are succesfull or not
        autoPlugin->setParameter("ScanDirection", "Negative");
        autoPlugin->setParameter("PrincipalContinuationParameter", "k3");
        autoPlugin->setParameter("PCPLowerBound", "0.2");
        autoPlugin->setParameter("PCPUpperBound", "1.2");

        //We can set the sbml parameter here or load into the roadrunner instance first
        //autoPlugin->setParameter("SBML", getFileContent(sbmlFile).c_str());

        //The load function does throw if file is not found.. is that what we want??
        if(!rri->load(sbmlFile))
        {
            throw(rr::Exception("Failed loading sbml model"));
        }


        if(!autoPlugin->execute())
        {
            Log(lError)<<"Problem executing the Auto plugin";
            return 0;
        }

        //Wait for plugin thread to finish
        while(autoPlugin->isWorking())
        {
            Log(lInfo)<<"Auto plugin is working..";
            Sleep(50);
        }

        Log(lInfo)<<"Auto plugin is done.";
        Log(lInfo)<<autoPlugin->getResult();

        //Check plugin data..
        pm.unload(autoPlugin);
    }
    catch(Exception& ex)
    {
        Log(lError)<<ex.getMessage();
    }
    catch(Poco::Exception& ex)
    {
        Log(lError)<<"Problem..."<<ex.message();
    }
    catch(...)
    {
        Log(lError)<<"Holy cow problem...!";
    }

    delete rri;
    //pause(true);
    return 0;
}

#if defined(CG_UI)

#if defined(STATIC_APP)
    #pragma comment(lib, "roadrunner-static.lib")
    #pragma comment(lib, "rrplugins_api-static.lib")
    #pragma comment(lib, "libsbml-static.lib")
    #pragma comment(lib, "rr-libstruct-static.lib")
    #pragma comment(lib, "libxml2_xe.lib")
    #pragma comment(lib, "blas.lib")
    #pragma comment(lib, "lapack.lib")
    #pragma comment(lib, "libf2c.lib")
    #pragma comment(lib, "sundials_cvode.lib")
    #pragma comment(lib, "sundials_nvecserial.lib")

#else
    #pragma comment(lib, "roadrunner.lib")
    #pragma comment(lib, "rrplugins_api.lib")
#endif

#pragma comment(lib, "poco_foundation-static.lib")
#pragma comment(lib, "cg32.lib")
#endif

