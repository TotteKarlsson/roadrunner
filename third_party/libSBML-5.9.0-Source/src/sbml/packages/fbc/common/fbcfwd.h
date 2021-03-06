/**
 * @file    fbcfwd.h
 * @brief   SBML FBC C structure declarations
 * @author  Frank T. Bergmann
 *
 *<!---------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright 2009-2013 California Institute of Technology.
 * 
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 *------------------------------------------------------------------------- -->
 */


#ifndef fbcfwd_h__
#define fbcfwd_h__


/**
 * Forward declaration of all opaque C types.
 *
 * Declaring all types up-front avoids "redefinition of type 'Foo'" compile
 * errors and allows our combined C/C++ headers to depend minimally upon
 * each other.  Put another way, the type definitions below serve the same
 * purpose as "class Foo;" forward declarations in C++ code.
 */

#ifdef __cplusplus
#  define CLASS_OR_STRUCT class
#else
#  define CLASS_OR_STRUCT struct
#endif  /* __cplusplus */

LIBSBML_CPP_NAMESPACE_BEGIN

typedef CLASS_OR_STRUCT Association      Association_t;
typedef CLASS_OR_STRUCT FluxBound        FluxBound_t;
typedef CLASS_OR_STRUCT FluxObjective    FluxObjective_t;
typedef CLASS_OR_STRUCT GeneAssociation  GeneAssociation_t;
typedef CLASS_OR_STRUCT Objective        Objective_t;

LIBSBML_CPP_NAMESPACE_END

#undef CLASS_OR_STRUCT


#endif  /* fbcfwd_h__ */
