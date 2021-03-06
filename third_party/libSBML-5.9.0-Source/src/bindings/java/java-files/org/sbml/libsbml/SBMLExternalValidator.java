/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.sbml.libsbml;

public class SBMLExternalValidator extends SBMLValidator {
   private long swigCPtr;

   protected SBMLExternalValidator(long cPtr, boolean cMemoryOwn)
   {
     super(libsbmlJNI.SBMLExternalValidator_SWIGUpcast(cPtr), cMemoryOwn);
     swigCPtr = cPtr;
   }

   protected static long getCPtr(SBMLExternalValidator obj)
   {
     return (obj == null) ? 0 : obj.swigCPtr;
   }

   protected static long getCPtrAndDisown (SBMLExternalValidator obj)
   {
     long ptr = 0;

     if (obj != null)
     {
       ptr             = obj.swigCPtr;
       obj.swigCMemOwn = false;
     }

     return ptr;
   }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        libsbmlJNI.delete_SBMLExternalValidator(swigCPtr);
      }
      swigCPtr = 0;
    }
    super.delete();
  }

  
/**
   * Constructor.
   * @internal
   */ public
 SBMLExternalValidator() {
    this(libsbmlJNI.new_SBMLExternalValidator__SWIG_0(), true);
  }

  
/**
   * Copy constructor.
   * @internal
   */ public
 SBMLExternalValidator(SBMLExternalValidator arg0) {
    this(libsbmlJNI.new_SBMLExternalValidator__SWIG_1(SBMLExternalValidator.getCPtr(arg0), arg0), true);
  }

  
/**
   * Creates and returns a deep copy of this converter.
   <p>
   * @return a (deep) copy of this converter.
   * @internal
   */ public
 SBMLValidator cloneObject() {
    long cPtr = libsbmlJNI.SBMLExternalValidator_cloneObject(swigCPtr, this);
    return (cPtr == 0) ? null : new SBMLValidator(cPtr, true);
  }

  
/**
   * the actual conversion 
   <p>
   * @return status code represeting success/failure/conversion impossible
   * @internal
   */ public
 long validate() {
    return libsbmlJNI.SBMLExternalValidator_validate(swigCPtr, this);
  }

  
/**
   * Returns the program name of the validator to be run
   <p>
   * @return the program name of the validator to be run
   * @internal
   */ public
 String getProgram() {
    return libsbmlJNI.SBMLExternalValidator_getProgram(swigCPtr, this);
  }

  
/**
   * Sets the name of the program to run
   <p>
   * @param program the program to be started
   * @internal
   */ public
 void setProgram(String program) {
    libsbmlJNI.SBMLExternalValidator_setProgram(swigCPtr, this, program);
  }

  
/**
   * Returns the output file name (this is the file the external program will write)
   <p>
   * @return the output file name
   * @internal
   */ public
 String getOutputFileName() {
    return libsbmlJNI.SBMLExternalValidator_getOutputFileName(swigCPtr, this);
  }

  
/**
   * Sets the output file name
   <p>
   * @param outputFileName the name of the output XML file
   * @internal
   */ public
 void setOutputFileName(String outputFileName) {
    libsbmlJNI.SBMLExternalValidator_setOutputFileName(swigCPtr, this, outputFileName);
  }

  
/**
   * @return the name of the SBML file (the document of this validator will be written to it)
   * @internal
   */ public
 String getSBMLFileName() {
    return libsbmlJNI.SBMLExternalValidator_getSBMLFileName(swigCPtr, this);
  }

  
/**
   * Sets the filename for the temporary file to be created
   <p>
   * @param sbmlFileName the temporary name
   * @internal
   */ public
 void setSBMLFileName(String sbmlFileName) {
    libsbmlJNI.SBMLExternalValidator_setSBMLFileName(swigCPtr, this, sbmlFileName);
  }

  
/**
   * Clear all additional arguments
   * @internal
   */ public
 void clearArguments() {
    libsbmlJNI.SBMLExternalValidator_clearArguments(swigCPtr, this);
  }

  
/**
   * Adds the given argument to the list of additional arguments 
   <p>
   * @param arg the argument
   * @internal
   */ public
 void addArgument(String arg) {
    libsbmlJNI.SBMLExternalValidator_addArgument(swigCPtr, this, arg);
  }

  
/**
   * @return the number of arguments. 
   * @internal 
   */ public
 long getNumArguments() {
    return libsbmlJNI.SBMLExternalValidator_getNumArguments(swigCPtr, this);
  }

  
/**
   * Returns the argument for the given index. 
   <p>
   * @param n the zero based index of the argument. 
   <p>
   * @return the argument at the given index. 
   * @internal 
   */ public
 String getArgument(long n) {
    return libsbmlJNI.SBMLExternalValidator_getArgument(swigCPtr, this, n);
  }

}
