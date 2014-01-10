import ctypes
from rrPlugins_CAPI import *
import rrPlugins as rrp

#Get a lmfit plugin object
lm = rrp.Plugin("rrp_lm")

#========== EVENT FUNCTION SETUP ===========================
def pluginIsProgressing(lmP):
    # The plugin don't know what a python object is.
    # We need to cast it here, to a proper python object
    lmObject = cast(lmP, ctypes.py_object).value
    print 'Iterations = ' + `lmObject.getProperty("NrOfIter")` \
        + '\tNorm = ' + `lmObject.getProperty("Norm")`

progressEvent =  NotifyEventEx(pluginIsProgressing)

#The ID of the plugin is passed as the last argument in the assignOnProgressEvent. 
#The plugin ID is later on retrieved in the plugin Event handler, see above
theId = id(lm)
assignOnProgressEvent(lm.plugin, progressEvent, theId)
#============================================================

#Setup lmfit properties.
lm.setProperty("SBML", lm.readAllText("lmFitTestModel.xml"))
experimentalData = lm.loadDataSeries ("testData.dat")
lm.setProperty("ExperimentalData", experimentalData)

# Add the parameters that we're going to fit and a initial 'start' value
lm.setProperty("InputParameterList", ["k1", 5.2])
lm.setProperty("FittedDataSelectionList", "[S1] [S2]")
lm.setProperty("ExperimentalDataSelectionList", "[S1] [S2]")

# Start minimization
lm.execute()

print 'Minimization finished. \n==== Result ====' 
print getPluginResult(lm.plugin)

# Get the experimental data as a numpy array
experimentalData = experimentalData.AsNumpy

# Get the fitted and residual data
fittedData = lm.getProperty ("FittedData").AsNumpy
residuals  = lm.getProperty ("Residuals").AsNumpy

rrp.plot(fittedData         [:,[0,1]], "blue", "-",    "",    "S1 Fitted")
rrp.plot(fittedData         [:,[0,2]], "blue", "-",    "",    "S2 Fitted")
rrp.plot(residuals          [:,[0,1]], "blue", "None", "x",   "S1 Residual")
rrp.plot(residuals          [:,[0,2]], "red",  "None", "x",   "S2 Residual")
rrp.plot(experimentalData   [:,[0,1]], "red",  "",     "*",   "S1 Data")
rrp.plot(experimentalData   [:,[0,2]], "blue", "",     "*",   "S2 Data")
rrp.plt.show()