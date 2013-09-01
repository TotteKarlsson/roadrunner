/*
 * TestRoadRunner.cpp
 *
 *  Created on: Jul 24, 2013
 *      Author: andy
 */

#include "TestRoadRunner.h"
#include "rrUtils.h"
#include "rrException.h"
#include "rrLogger.h"

using namespace std;

namespace rr
{


TestRoadRunner::~TestRoadRunner()
{
    delete simulation;
    delete rr;
}

TestRoadRunner::TestRoadRunner(const std::string& version, int caseNumber) :
        version(version), caseNumber(caseNumber), rr(0), simulation(0)
{
    //fileName = getModelFileName(version, caseNumber);

    home = getenv("HOME");
    dataOutputFolder = home + string("/tmp");
    string dummy;
    string logFileName;
    string settingsFileName;



    //Create a log file name
    createTestSuiteFileNameParts(caseNumber, ".log", dummy, logFileName, settingsFileName);

    //Create subfolder for data output
    dataOutputFolder = joinPath(dataOutputFolder, getTestSuiteSubFolderName(caseNumber));

    if(!createFolder(dataOutputFolder))
    {
        string msg("Failed creating output folder for data output: " + dataOutputFolder);
        throw(Exception(msg));
    }
}

bool TestRoadRunner::test(const std::string& compiler)
{
    loadSBML(compiler);

    simulate();

    return true;
}

void TestRoadRunner::loadSBML(const std::string& compiler)
{
    rr = new RoadRunner(compiler, dataOutputFolder, home + "/local/rr_support");

    simulation = new TestSuiteModelSimulation(dataOutputFolder);

    simulation->UseEngine(rr);

    //Read SBML models.....
    modelFilePath = home + "/src/sbml_test/cases/semantic";

    simulation->SetCaseNumber(caseNumber);
    createTestSuiteFileNameParts(caseNumber, "-sbml-" + version + ".xml",
            modelFilePath, modelFileName, settingsFileName);

    //The following will load and compile and simulate the sbml model in the file
    simulation->SetModelFilePath(modelFilePath);
    simulation->SetModelFileName(modelFileName);
    simulation->ReCompileIfDllExists(true);
    simulation->CopyFilesToOutputFolder();
    //setTempFolder(gRR, simulation.GetDataOutputFolder().c_str());
    //setComputeAndAssignConservationLaws(gRR, false);

    //rr->loadSBMLFromFile(fileName);

    if (!simulation->LoadSBMLFromFile())
    {
        throw Exception("Failed loading sbml from file");
    }

    //Then read settings file if it exists..
    string settingsOveride("");
    if (!simulation->LoadSettingsEx(settingsOveride))
    {
        Log(lError) << "Failed loading SBML model settings";
        throw Exception("Failed loading SBML model settings");
    }

    rr->computeAndAssignConservationLaws(false);
}

void TestRoadRunner::simulate()
{
    if(!rr->simulate2Ex(rr->getTimeStart(), rr->getTimeEnd(), rr->getNumPoints()))
    {
        throw Exception("Simulation Failed");
    }
}

void TestRoadRunner::saveResult()
{
    if (!simulation->SaveResult())
    {
        //Failed to save data
        throw Exception("Failed saving result");
    }
}

void TestRoadRunner::compareReference()
{
    if (!simulation->LoadReferenceData())
    {
        throw Exception("Failed Loading reference data");
    }

    simulation->CreateErrorData();
    bool result = simulation->Pass();
    simulation->SaveAllData();
    simulation->SaveModelAsXML(dataOutputFolder);
    if (!result)
    {
        Log(Logger::PRIO_NOTICE) << "Test failed..\n";
    }
    else
    {
        Log(Logger::PRIO_NOTICE) << "Test passed..\n";
    }
}
/*

bool RunTest(const string& version, int caseNumber)
{
    bool result = false;
    RRHandle gRR = 0;

    //Create instance..
    gRR = createRRInstanceEx(gTempFolder.c_str());

    if(gDebug && gRR)
    {
        enableLoggingToConsole();
        setLogLevel("Debug5");
    }
    else
    {
        setLogLevel("Error");
    }

    //Setup environment
    setTempFolder(gRR, gTempFolder.c_str());

    if(!gRR)
    {
        return false;
    }

    try
    {




        //Then read settings file if it exists..
        string settingsOveride("");
        if(!simulation.LoadSettings(settingsOveride))
        {
            throw("Failed loading simulation settings");
        }

        //Then Simulate model
        if(!simulation.Simulate())
        {
            throw("Failed running simulation");
        }

        //Write result
        if(!simulation.SaveResult())
        {
            //Failed to save data
            throw("Failed saving result");
        }

        if(!simulation.LoadReferenceData())
        {
            throw("Failed Loading reference data");
        }

        simulation.CreateErrorData();
        result = simulation.Pass();
        simulation.SaveAllData();
        simulation.SaveModelAsXML(dataOutputFolder);
        if(!result)
        {
            clog<<"\t\tTest failed..\n";
        }
        else
        {
            clog<<"\t\tTest passed..\n";
        }
    }
    catch(rr::Exception& ex)
    {
        string error = ex.what();
        cerr<<"Case "<<caseNumber<<": Exception: "<<error<<endl;
        result = false;;
    }

    // done with rr
    freeRRInstance(gRR);
    return result;
}

 */


} /* namespace rr */


