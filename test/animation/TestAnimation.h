/*
* Dream::Animation::Test::TestAnimation
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

#ifndef TESTANIMATION_H
#define TESTANIMATION_H

#include "../../src/unit/Unit.h"

namespace Dream  {
	namespace Animation {
		namespace Test {
			class TestAnimation : public DreamUnit::Unit{
			public:
				TestAnimation();
				~TestAnimation();
				void run();
			//protected:
			//private:
			}; // End of TestAnimation
		} // End of Test
	} // End of Animation
} // End of Dream

#endif // End of TESTANIMATION_H