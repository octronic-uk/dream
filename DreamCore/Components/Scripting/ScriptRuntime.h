/*
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

#pragma once

#include "../SharedAssetRuntime.h"
#include "../Event.h"

namespace Dream
{
    class ScriptDefinition;
    class InputComponent;
    class NanoVGComponent;
    class SceneRuntime;
    class ScriptRuntimeState;

    class ScriptRuntime : public SharedAssetRuntime
    {
    public:
        ScriptRuntime(ScriptDefinition*,ProjectRuntime*);
        ~ScriptRuntime() override;
        bool useDefinition() override;
        ScriptRuntimeState* createState(SceneObjectRuntime*);
        void removeState(ScriptRuntimeState* state);
        string getSource() const;
        void setSource(const string& source);

        bool executeOnInit(ScriptRuntimeState* state);
        bool executeOnUpdate(ScriptRuntimeState* state);
        bool executeOnEvent(ScriptRuntimeState* state);

        bool executeOnInput(InputComponent*, SceneRuntime*);
        bool executeOnNanoVG(NanoVGComponent*, SceneRuntime*);

        void registerInputScript();
        void registerNanoVGScript();
    private:
        string mSource;
    };

    class ScriptRuntimeState
    {
    public:
        inline ScriptRuntimeState
        (ScriptRuntime* sc, SceneObjectRuntime* rt)
            : script(sc), runtime(rt)
        {

        }

        inline ~ScriptRuntimeState
        ()
        {
            if (script)
            {
                script->removeState(this);
            }
        }

        ScriptRuntime* script = nullptr;
        SceneObjectRuntime* runtime = nullptr;
        bool initialised = false;
        bool error = false;
    };
}
