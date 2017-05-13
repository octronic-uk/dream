/*
 * JoystickManager.h
 *
 * Created: 17/12/2016 2016 by Ashley
 *
 * Copyright 2016 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */
#ifndef JOYSTICKMANAGER_H
#define JOYSTICKMANAGER_H

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <DreamCore.h>

using namespace Dream;
namespace DreamSDL
{
    using namespace std;
    class SDLJoystickManager
    {
    private:
      static bool IsInitialised;
    public:
        static vector<SDL_GameController*> ControllerHandles;
        static vector<SDL_Joystick*> JoystickHandles;
        static void init();
        static void destroy();
    };
}
#endif // JOYSTICKMANAGER_H
