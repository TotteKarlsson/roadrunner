/**
 * @file rrCapabilities.h
 * @brief Container for Capabilities
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
#ifndef rrCapabilitiesH
#define rrCapabilitiesH
#include <vector>
#include "rrPluginsAPIExporter.h"
#include "rrCapability.h"
#include "rrStringList.h"
//---------------------------------------------------------------------------

namespace rrp
{
using rr::StringList;
using std::vector;

/**
    Container for Capabilitities
*/
class PLUGINS_API_DECLSPEC Capabilities
{
    public:
        /**
            Create a Capabilitities container
        */
                                        Capabilities();//const string& name, const string& description);

        /**
            De allocate memory associated with the capabilities.
        */
                                       ~Capabilities();

        /**
            Add a capability
        */
        void                            add(Capability& capability);

        /**
            Get capabilities as a list of strings
        */
        string                          info() const;

        /**
            Get number of capabilties.
        */
        u_int                           count();

        /**
            Clear the capabilties container.
        */
        void                            clear();

        /**
            Get a pointer to a capability using operator []
        */
        Capability*                     operator[](int i);

        /**
            Get a pointer to a capability using a vaspability name.
        */
        Capability*                     get(const string& capName);

        /**
            Set a parameter value in a capability.
        */
        bool                            setParameter(const string& name, const string& value);

        /**
            Get capabilities as a list of strings
        */
        StringList                      asStringList();

        /**
            Get capabilities as XML
        */
        string                          asXML();

        /**
            Output capabilities to a ostream
        */
        PLUGINS_API_DECLSPEC
        friend ostream&                 operator<<(ostream& stream, const Capabilities& caps);

    protected:
//        /**
//            Name of capability
//        */
//        string                          mName;
//
//        /**
//            Descritpion of a capability
//        */
//        string                          mDescription;
        mutable
        vector<Capability*>             mProperties;

};

}
#endif


