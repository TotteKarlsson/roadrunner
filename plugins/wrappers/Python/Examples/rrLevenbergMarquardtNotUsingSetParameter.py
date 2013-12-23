import rrPlugins as rrp
import ctypes as ct
import pprint

def pluginIsProgressing(msg, lm):
    lmObject = ct.cast(lm, ct.py_object).value
    print 'Norm = ' + `lmObject.getProperty("Norm")` #Retrieve plugin parameters
    print 'Nr of Iterations = ' + `lmObject.getProperty("NrOfIter")`


lm = rrp.Plugin ("rrp_lm")


lm.printflags = 1

experimentalData = lm.loadDataSeries ("testData.dat")

lm.ExperimentalData = experimentalData;
lm.SBML = lm.readAllText("sbml_test_0001.xml")

# Add the parameters that we're going to fit and the initial value
lm.InputParameterList = ["k1", 0.2]

lm.FittedDataSelectionList = "[S1] [S2]"
lm.ExperimentalDataSelectionList = "[S1] [S2]"

lm.OnProgress (pluginIsProgressing)

# Execute lmfit plugin
res = lm.execute()

# Get the experimental data as a numpy array
experimentalData = experimentalData.AsNumpy

# Get the fitted and residual data
fittedData = lm.FittedData.AsNumpy
residuals  = lm.Residuals.AsNumpy

rrp.plot (fittedData[:,[0,1]], myColor="blue", myLinestyle="-", myMarker="", myLabel="S1 Fitted")
rrp.plot (fittedData[:,[0,2]], myColor="blue", myLinestyle="-", myMarker="", myLabel="S2 Fitted")
rrp.plot (residuals[:,[0,1]], myColor="blue", myLinestyle="None", myMarker="x", myLabel="S1 Residual")
rrp.plot (residuals[:,[0,2]], myColor="red", myLinestyle="None", myMarker="x", myLabel="S2 Residual")
rrp.plot (experimentalData[:,[0,1]], myColor="red", myLinestyle="", myMarker="*", myLabel="S1 Data")
rrp.plot (experimentalData[:,[0,2]], myColor="blue", myLinestyle="", myMarker="*", myLabel="S2 Data")

rrp.plt.show()


