/*
* Chai::ChaiScriptInstance
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CHAISCRIPTINSTANCE_H
#define CHAISCRIPTINSTANCE_H

#include <iostream>
#include <chaiscript/chaiscript.hpp>
#include "../../ResourceInstance.h"

namespace Dream      {
namespace Resource   {
namespace Instance   {
namespace Script     {
namespace ChaiScript {
    
    class ChaiScriptInstance : public Dream::Resource::Instance::ResourceInstance {
    private:
        std::string mObjectUUID;
        chaiscript::ChaiScript* mEngine;
    public:
        ChaiScriptInstance(chaiscript::ChaiScript*);
        ChaiScriptInstance(chaiscript::ChaiScript*, std::string, const std::string, bool);
        ~ChaiScriptInstance(void);

        void setScriptFromFile(const std::string);
        void setScriptFromString(const std::string);

        std::string getObjectUUID();
        void setObjectUUID(std::string);

    }; // End of ChaiScriptInstance
    
} // End of ChaiScript
} // End of Script
} // End of Instance
} // End of Resource
} // End of Dream

#endif // End of CHAISCRIPT_H