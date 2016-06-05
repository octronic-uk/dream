/*
* Dream::Plugnis::Audio::OpenAL::OALAudio
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

#include <iostream>
#include <vector>

#ifdef __APPLE__
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#else
	#include <AL/al.h>
	#include <AL/alc.h>
#endif

#include "../AudioPluginInterface.h"

namespace Dream   {
namespace Plugins {
namespace Audio   {
namespace OpenAL  {
	
	class OALAudio : public AudioPluginInterface {
	private:
		std::vector<ALuint> mPlayQueue;
		std::vector<ALuint> mPauseQueue;
		std::vector<ALuint> mStopQueue;
		std::vector<ALuint> mSources;
		std::vector<ALuint> mBuffers;
		ALCdevice*  mDevice;
		ALCcontext* mContext;
	public:
		OALAudio();
		~OALAudio();
		bool init();
		void update(Scene::Scene*);
	
		void   setSourcePosision(ALuint, float, float, float);
		void   setListenerPosition(float, float, float);
		
		void   pushToPlayQueue(ALuint);
		void   pushToPauseQueue(ALuint);
		void   pushToStopQueue(ALuint);
		
	protected:
		ALuint generateBuffers(size_t);
		ALuint generateSources(size_t);
		
		void   deleteBuffers(int, ALuint);
		void   deleteSources(int, ALuint);
		
		void   playSource(ALuint);
		void   stopSource(ALuint);
		void   pauseSource(ALuint);
		
		ALenum getSourceState(ALuint);
	
		void   updatePlayQueue();
		void   updatePauseQueue();
		void   updateStopQueue();
		
		void deleteAllSources();
		void deleteAllBuffers();
		
	}; // End of OALAudio
	
} // End of OpenAL
} // End of Audio
} // End of Plugins
} // End of Dream

#endif // End of AUDIOPLAYBACK_H
