/*
* OALAudio
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

#ifndef AUDIOPLAYBACK_H
#define AUDIOPLAYBACK_H
#include <AL/al.h>
#include <AL/alc.h>
#include "AudioInterface.h"

namespace Dream {
	namespace Audio {
		class OALAudio : public AudioInterface {
		private:
			ALCdevice*  mDevice;
			ALCcontext* mContext;
		public:
			OALAudio(void);
			~OALAudio(void);

			ALuint generateBuffer();
			void playSource(ALuint);
			void stopSource(ALuint);
			void pauseSource(ALuint);
		}; // End of OALAudio
	} // End of Audio
} // End of Dream

#endif // End of AUDIOPLAYBACK_H