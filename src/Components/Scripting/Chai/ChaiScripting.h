/*
* Dream::Scripting::ChaiScripting
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

#ifndef CHAISCRIPTING_H
#define CHAISCRIPTING_H

#include <iostream>
#include "../ScriptingComponentInterface.h"
#include "../../Input/InputComponentInterface.h"
#include "../../../Asset/Instances/Script/Chai/ChaiScriptInstance.h"

#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>

namespace Dream     {
namespace Components   {
namespace Scripting {
namespace Chai      {
	
	class ChaiScripting : public ScriptingComponentInterface {
	public:
		ChaiScripting(void);
		~ChaiScripting(void);
		bool init();
		void update(Dream::Scene::Scene*);
		void handleInputs(Dream::Scene::SceneObject*);
	}; // End of ChaiScripting
	
} // End of Chai
} // End Scripting
} // End of Components
} // End of Dream

#endif // End of CHAISCRIPTING_H