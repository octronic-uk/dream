/*
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

#pragma once

#include <angelscript.h>
#include "ScriptRuntime.h"
#include "ScriptCache.h"
#include "../Component.h"
#include "../../Project/ProjectDirectory.h"

namespace Dream
{
    class SceneRuntime;
    class SceneObjectRuntime;
    class Event;
    class ScriptCache;

    class ScriptPrintListener
    {
    public:
        virtual ~ScriptPrintListener();
        virtual void onPrint(const string&) = 0;
    };

    class ScriptComponent : public Component
    {
    public:

        static vector<ScriptPrintListener*> PrintListeners;
        static void AddPrintListener(ScriptPrintListener* listener);
        static asIScriptEngine* Engine;

        ScriptComponent(ProjectRuntime* runtime, ScriptCache* cache);
       ~ScriptComponent() override;

        bool init() override;

    private:
        const static string COMPONENTS_TBL;
        ScriptCache* mScriptCache;

        // API Exposure Methods ======================================================
        void debugRegisteringClass(const string& classname);
        void exposeAPI();
        void exposeClasses();
        void exposeAssetDefinition();
        void exposeAnimationRuntime();
        void exposePathRuntime();
        void exposeModelRuntime();
        void exposeCamera();
        void exposeProjectRuntime();
        void exposeProject();
        void exposeProjectDirectory();
        void exposeEvent();
        void exposeWindowComponent();
        void exposeGraphicsComponent();
        void exposeInputComponent();
        void exposeAudioComponent();
        void exposeAudioRuntime();
        void exposeLightRuntime();
        void exposeScriptRuntime();
        void exposePhysicsComponent();
        void exposePhysicsObjectRuntime();
        void exposeShaderRuntime();
        void exposeSceneRuntime();
        void exposeSceneObjectRuntime();
        void exposeTime();
        void exposeTransform();
        void exposeGLM();
        void exposeDefinition();
        void exposeNanoVG();
    };
}