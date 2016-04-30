/*
* WavAudio
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

#ifndef WAVAUDIO_H
#define WAVAUDIO_H

#include "../Audio.h"

#define RESOURCE_FORMAT_WAV  "wav"

namespace Dream {
	namespace Resource {
		namespace Audio {
			namespace Wav {
				class WavAudio : public Dream::Resource::Audio::Audio {
				private:
					std::string mWavPath;
				public:
					WavAudio(void);
					WavAudio(nlohmann::json);
					~WavAudio(void);
					void generateAbsolutePaths(std::string,std::string);
					ResourceInstance* createInstance();
				};
			}
		}
	}
}
#endif // End of WAVAUDIO_H
