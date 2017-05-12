/*
* AudioComponent
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

#include "SDLAudioComponent.h"

#include <iostream>

namespace Dream
{
    SDLAudioComponent::SDLAudioComponent() : IAudioComponent()
    {
        // Pass
    }

    SDLAudioComponent::~SDLAudioComponent()
    {
        if (DEBUG)
        {
            cout << "AudioComponent: Destroying Object" << endl;
        }
        Mix_CloseAudio();
    }

    bool SDLAudioComponent::init()
    {
        if (DEBUG)
        {
            cout << "AudioComponent: Initialising...";
        }

        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
        {
            if (DEBUG)
            {
                cout << endl << "AudioComponent: Error SDL_OpenAudio Failed" << endl;
            }
            return false;
        }
        if (DEBUG)
        {
            cout << "Done!" << endl;
        }
        return true;
    }

    void SDLAudioComponent::update(Scene*)
    {

    }

    void SDLAudioComponent::exposeLuaApi(lua_State* state)
    {
        luabind::module(state)
        [
            luabind::class_<SDLAudioComponent>("SDLAudioComponent")
            // TODO
        ];
    }

} // End of Dream