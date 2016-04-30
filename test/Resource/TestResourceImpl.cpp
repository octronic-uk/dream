/*
* TestResourceImpl
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

#include "TestResourceImpl.h"

namespace Dream {
	namespace Resource {
		namespace Test {
			TestResourceImpl::TestResourceImpl() : Dream::Resource::Resource() {}
			TestResourceImpl::~TestResourceImpl(void) {}
			void TestResourceImpl::generateAbsolutePaths(std::string projDir, std::string projUUID) {}
			ResourceInstance* TestResourceImpl::createInstance() {return NULL;}
		}
	}
} // End of Dream
