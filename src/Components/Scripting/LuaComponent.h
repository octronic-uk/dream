/*
 * LuaComponent.h
 *
 * Created: 26/11/2016 2016 by Ashley
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
#ifndef LUACOMPONENT_H
#define LUACOMPONENT_H

#include <map>
#include <iostream>

#include <luabind/luabind.hpp>

extern "C" {
  //#include "lua.h"
  #include "lualib.h"
  //#include "lauxlib.h"
}

#include "../../DreamEngine.h"

namespace Dream {
    using namespace std;

    class LuaComponent {
    private:
        lua_State *mState;
        map<SceneObject*, LuaScriptInstance*> *mScriptMap;
    public:
        LuaComponent();
        ~LuaComponent();

        void setLuaScriptMap(map<SceneObject*,LuaScriptInstance*>*);

        void bindAssetManagerClass();
        void bindComponentsClass();
        void bindProjectClass();
        void bindSceneClass();

        bool init();

        bool loadScriptsFromMap();
        bool loadScript(LuaScriptInstance*);
        void stackDump();
        bool update();
        bool executeScriptUpdate(SceneObject*, LuaScriptInstance*);
        bool executeScriptKeyHandler(SceneObject*, LuaScriptInstance*);

    }; // End of LuaComponent

} // End of Dream

#endif // LUACOMPONENT_H
