/*
* Dream::Scripting::ScriptingInterface
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

#ifndef SCRIPTINGINTERFACE_H
#define SCRIPTINGINTERFACE_H

#include "../PluginInterface.h"

namespace Dream {
namespace Plugins {
namespace Scripting {
	
	class ScriptingPluginInterface : public Dream::Plugins::PluginInterface {
	public:
		ScriptingPluginInterface(void);
		virtual ~ScriptingPluginInterface(void) {}
	}; // End of ScriptingPluginInterface
	
} // End of Scripting
}// End of Plugins
}// End of Dream

#endif // End of SCRIPTINGINTERFACE_H