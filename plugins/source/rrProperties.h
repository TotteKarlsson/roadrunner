/**
 * @file rrProperties.h
 * @brief Properties implementation - a container for Plugin parameters
 * @author Totte Karlsson & Herbert M Sauro
 *
 * <--------------------------------------------------------------
 * This file is part of cRoadRunner.
 * See http://code.google.com/p/roadrunnerlib for more details.
 *
 * Copyright (C) 2012-2013
 *   University of Washington, Seattle, WA, USA
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * In plain english this means:
 *
 * You CAN freely download and use this software, in whole or in part, for personal,
 * company internal, or commercial purposes;
 *
 * You CAN use the software in packages or distributions that you create.
 *
 * You SHOULD include a copy of the license in any redistribution you may make;
 *
 * You are NOT required include the source of software, or of any modifications you may
 * have made to it, in any redistribution you may assemble that includes it.
 *
 * YOU CANNOT:
 *
 * redistribute any piece of this software without proper attribution;
*/
#ifndef rrPropertiesH
#define rrPropertiesH
#include <string>
#include <list>
#include <vector>
#include <utility>
#include "rrpExporter.h"
#include "rrPropertyBase.h"
#include "rrStringList.h"
//---------------------------------------------------------------------------

namespace rrp
{
using std::vector;
using std::list;
using std::pair;
//using rr::StringList;
using std::string;

/**
    Container for Plugin parameters.
*/
class RRP_DECLSPEC Properties
{
    public:
        /**
           Create a parameter container
        */
                                                Properties(bool canClear = true);
        /**
           Deallocate memory allocated
        */
        virtual                                ~Properties();

        /**
           Copy constructor
        */
                                                Properties(const Properties& cpyMe);
        /**
           Add a parameter. By default, the container do not owe memory allocated by the parameter.
           If the ownMemory flag is set to true, the container will de allocate memory for the parameter as well
           \param para A pointer to a Plugin parameter
           \param ownMemory Boolean flag indicating if the container is owing the memory of the parameter or not.
        */
        void                                    add(PropertyBase* para, bool ownMemory = false);

        /**
           Return parameters in the container as a list of strings.
        */
        rr::StringList                          getNames() const;

        /**
           Return number of parameters in the container.
        */
        u_int                                   count() const;

        /**
           Access a parameter using operator []
           \return A const Pointer to a PropertyBase
        */
        const PropertyBase*                  operator[](const int& i) const;

        /**
           Access a parameter using operator []
           \return A Pointer to a PropertyBase
        */
        PropertyBase*                        operator[](const int& i);

        /**
           Get a parameter with a specific name
           \return A Pointer to a PropertyBase if present, NULL otherwise
        */
        PropertyBase*                        getProperty(const string& paraName);

        /**
           Get First Property. Resets the internal iterator to first element
           \return A Pointer to a PropertyBase if present, NULL otherwise
        */
        PropertyBase*                        getFirst();

        /**
           Get Next Property. Advances the internal iterator to next element
           \return A Pointer to a PropertyBase if present, NULL otherwise
        */
        PropertyBase*                        getNext();

        /**
           Get Previous Property. De Advances the internal iterator to next element
           \return A Pointer to a PropertyBase if present, NULL otherwise
        */
        PropertyBase*                        getPrevious();
        
        /**
           Get Current Property. De Advances the internal iterator to next element
           \return A Pointer to a PropertyBase if present, NULL otherwise
        */
        PropertyBase*                        getCurrent();
        
        /**
           Empty the parameter container
        */  
        bool                                    clear();

        /**
           Output the container to a output stream (ostream)
        */
        RRP_DECLSPEC
        friend ostream&                         operator <<(ostream& os, const Properties& pars);

    protected:
        /**
            The parameter container may owe the parameter, default is false. In certain ciscumstances, the container need to owe
            the memory of the parameter and will de allocate it on clear, or in the destruction of the container.
            \todo Change the container to a std::list.
        */
        vector< pair<PropertyBase*, bool> >                  mParas;
        vector< pair<PropertyBase*, bool> >::iterator        mParasIter;


        /**
            Boolean indicating if a client of the container can clear the list. True by default
        */            
        bool                                                    mCanClientClearList;
};

}

#endif
