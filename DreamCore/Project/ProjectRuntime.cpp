﻿/*
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

#include "ProjectRuntime.h"

#include "Project.h"
#include "ProjectDefinition.h"
#include "Components/Storage/ProjectDirectory.h"

#include "Common/Logger.h"

#include "Scene/SceneRuntime.h"
#include "Scene/SceneDefinition.h"
#include "Scene/Entity/EntityRuntime.h"

#include "Components/AssetDefinition.h"
#include "Components/Time.h"
#include "Components/Transform.h"
#include "Components/Audio/AudioComponent.h"
#include "Components/Audio/AudioCache.h"
#include "Components/Input/InputComponent.h"

#include "Components/Task/TaskManager.h"
#include "Components/Graphics/GraphicsComponent.h"
#include "Components/Graphics/Model/ModelMesh.h"
#include "Components/Physics/PhysicsComponent.h"
#include "Components/Window/WindowComponent.h"
#include "Components/Script/ScriptComponent.h"

#include "Components/Graphics/Model/ModelCache.h"
#include "Components/Graphics/Material/MaterialCache.h"
#include "Components/Graphics/Shader/ShaderCache.h"
#include "Components/Graphics/Shader/ShaderRuntime.h"
#include "Components/Graphics/Texture/TextureCache.h"

#include <thread>

namespace octronic::dream
{
    ProjectRuntime::ProjectRuntime
    (Project* project, WindowComponent* windowComponent, StorageManager* fm)
        : Runtime(project->getDefinition()),
          mDone(false),
          mTime(nullptr),
          mProject(project),
          mAudioComponent(nullptr),
          mInputComponent(nullptr),
          mGraphicsComponent(nullptr),
          mPhysicsComponent(nullptr),
          mScriptComponent(nullptr),
          mWindowComponent(windowComponent),
          mTaskManager(nullptr),
          mStorageManager(fm),
          mAudioCache(nullptr),
          mTextureCache(nullptr),
          mMaterialCache(nullptr),
          mModelCache(nullptr),
          mShaderCache(nullptr),
          mScriptCache(nullptr)
    {
        LOG_DEBUG( "ProjectRuntime: Constructing" );
        mFrameDurationHistory.resize(MaxFrameCount);
    }

    ProjectRuntime::~ProjectRuntime
    ()
    {
        LOG_DEBUG( "ProjectRuntime: Destructing" );
        deleteCaches();
        deleteComponents();

        if (mTime != nullptr)
        {
            delete mTime;
            mTime = nullptr;
        }

    }

    WindowComponent*
    ProjectRuntime::getWindowComponent
    ()
    const
    {
        return mWindowComponent;
    }

    void
    ProjectRuntime::setDone
    (bool done)
    {
        mDone = done;
    }

    Time*
    ProjectRuntime::getTime
    ()
    const
    {
        return mTime;
    }

    bool
    ProjectRuntime::initComponents
    ()
    {
        LOG_TRACE( "ProjectRuntime: {}",__FUNCTION__ );

        mTime = new Time();

        if (!initWindowComponent())
        {
            return false;
        }

        if(!initGraphicsComponent())
        {
            return false;
        }

        if (!initInputComponent())
        {
            return false;
        }

        if(!initPhysicsComponent())
        {
            return false;
        }

        if(!initAudioComponent())
        {
            return false;
        }

        if (!initScriptComponent())
        {
            return false;
        }

        if (!initTaskManager())
        {
            return false;
        }

        LOG_DEBUG( "ProjectRuntime: Successfuly created Components." );

        return true;
    }

    bool
    ProjectRuntime::initWindowComponent
    ()
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        if (!mWindowComponent)
        {
            LOG_CRITICAL("ProjectRuntime: Window component is null");
            return false;
        }
        auto projDef = dynamic_cast<ProjectDefinition*>(mDefinition);
        projDef->lock();
        mWindowComponent->setName(projDef->getName());
        projDef->unlock();
        return true;
    }

    bool
    ProjectRuntime::initAudioComponent
    ()
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        mAudioComponent = new AudioComponent(this);
        mAudioComponent->lock();
        if (!mAudioComponent->init())
        {
            LOG_ERROR( "ProjectRuntime: Unable to initialise AudioComponent." );
            mAudioComponent->unlock();
            return false;
        }
        mAudioComponent->unlock();
        return true;
    }

    bool ProjectRuntime::initInputComponent()
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        auto projectDef = dynamic_cast<ProjectDefinition*>(mDefinition);
        projectDef->lock();
        mInputComponent = new InputComponent(this);
        projectDef->unlock();
        mInputComponent->lock();

        if (!mInputComponent->init())
        {
            LOG_ERROR( "ProjectRuntime: Unable to initialise InputComponent." );
            mInputComponent->unlock();
            return false;
        }
        mInputComponent->unlock();
        return true;
    }

    bool
    ProjectRuntime::initPhysicsComponent
    ()
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        mPhysicsComponent = new PhysicsComponent(this);
        mPhysicsComponent->lock();
        mPhysicsComponent->setTime(mTime);
        if (!mPhysicsComponent->init())
        {
            LOG_ERROR( "ProjectRuntime: Unable to initialise PhysicsComponent." );
            mPhysicsComponent->unlock();
            return false;
        }
        mPhysicsComponent->unlock();
        return true;
    }

    bool
    ProjectRuntime::initGraphicsComponent
    ()
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        mGraphicsComponent = new GraphicsComponent(this,mWindowComponent);
        mGraphicsComponent->lock();
        mGraphicsComponent->setTime(mTime);
        mGraphicsComponent->setShaderCache(mShaderCache);
        if (!mGraphicsComponent->init())
        {
            LOG_ERROR( "ProjectRuntime: Unable to initialise Graphics Component." );
            mGraphicsComponent->unlock();
            return false;
        }
        mGraphicsComponent->unlock();
        return true;
    }

    bool
    ProjectRuntime::initScriptComponent
    ()
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        mScriptComponent = new ScriptComponent(this,mScriptCache);
        mScriptComponent->lock();
        if(!mScriptComponent->init())
        {
            LOG_ERROR( "ProjectRuntime: Unable to initialise Script Engine." );
            mScriptComponent->unlock();
            return false;
        }
        mScriptComponent->unlock();
        return true;
    }

    bool
    ProjectRuntime::initTaskManager
    ()
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        mTaskManager = new TaskManager();
        return true;
    }

    bool
    ProjectRuntime::initCaches
    ()
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        mAudioCache = new AudioCache(this);
        mTextureCache = new TextureCache(this);
        mShaderCache  = new ShaderCache(this);
        mMaterialCache = new MaterialCache(this,mShaderCache,mTextureCache);
        mModelCache   = new ModelCache(this,mShaderCache, mMaterialCache);
        mScriptCache  = new ScriptCache(this);
        return true;
    }

    void
    ProjectRuntime::deleteCaches
    ()
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        if (mAudioCache != nullptr)
        {
            delete mAudioCache;
            mAudioCache = nullptr;
        }

        if (mModelCache != nullptr)
        {
            delete mModelCache;
            mModelCache = nullptr;
        }

        if (mShaderCache != nullptr)
        {
            delete mShaderCache;
            mShaderCache = nullptr;
        }

        if (mMaterialCache != nullptr)
        {
            delete mMaterialCache;
            mMaterialCache = nullptr;
        }

        if (mTextureCache != nullptr)
        {
            delete mTextureCache;
            mTextureCache = nullptr;
        }

        if (mScriptCache != nullptr)
        {
            delete mScriptCache;
            mScriptCache = nullptr;
        }
    }

    void
    ProjectRuntime::deleteComponents
    ()
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        if (mTaskManager != nullptr)
        {
            delete mTaskManager;
            mTaskManager = nullptr;
        }

        if (mAudioComponent != nullptr)
        {
            delete mAudioComponent;
            mAudioComponent = nullptr;
        }

        if (mInputComponent != nullptr)
        {
            delete mInputComponent;
            mInputComponent = nullptr;
        }

        if (mGraphicsComponent != nullptr)
        {
            delete mGraphicsComponent;
            mGraphicsComponent = nullptr;
        }

        if (mPhysicsComponent != nullptr)
        {
            delete mPhysicsComponent;
            mPhysicsComponent = nullptr;
        }

        if (mScriptComponent != nullptr)
        {
            delete mScriptComponent;
            mScriptComponent = nullptr;
        }
    }

    bool
    ProjectRuntime::isDone
    ()
    const
    {
        return mDone;
    }

    AudioComponent*
    ProjectRuntime::getAudioComponent
    ()
    const
    {
        return mAudioComponent;
    }

    PhysicsComponent*
    ProjectRuntime::getPhysicsComponent
    ()
    const
    {
        return mPhysicsComponent;
    }

    GraphicsComponent*
    ProjectRuntime::getGraphicsComponent
    ()
    const
    {
        return mGraphicsComponent;
    }

    ScriptComponent*
    ProjectRuntime::getScriptComponent
    ()
    const
    {
        return mScriptComponent;
    }

    TaskManager*
    ProjectRuntime::getTaskManager
    ()
    const
    {
        return mTaskManager;
    }

    StorageManager*
    ProjectRuntime::getStorageManager
    ()
    const
    {
        return mStorageManager;
    }

    bool
    ProjectRuntime::updateLogic
    (SceneRuntime* sr)
    {
        LOG_DEBUG("\n"
        "=======================================================================\n"
        "Update Logic called @ {}\n"
        "=======================================================================\n",
        mTime->getAbsoluteTime());
        mTime->updateFrameTime();

        mFrameDurationHistory.push_back(1000.0f/mTime->getFrameTimeDelta());

        if (mFrameDurationHistory.size() > MaxFrameCount)
        {
            mFrameDurationHistory.pop_front();
        }

        sr->createSceneTasks();
        mTaskManager->clearFences();
        mTaskManager->waitForFence();
        mGraphicsComponent->handleResize();
        sr->getCamera()->update();
        return true;
    }

    void
    ProjectRuntime::updateGraphics
    (SceneRuntime* sr)
    {
        LOG_DEBUG("\n"
        "=======================================================================\n"
        "Update Graphics called @ {}\n"
        "=======================================================================\n",
        mTime->getAbsoluteTime());
        // Draw 3D/PhysicsDebug/2D
        ModelMesh::ClearCounters();
        mGraphicsComponent->executeTaskQueue();
        mGraphicsComponent->renderGeometryPass(sr);
        mGraphicsComponent->renderShadowPass(sr);
        mGraphicsComponent->renderLightingPass(sr);
        mGraphicsComponent->executeDestructionTaskQueue();
        ShaderRuntime::InvalidateState();
        mPhysicsComponent->setCamera(sr->getCamera());
        mPhysicsComponent->drawDebug();
    }

    int
    ProjectRuntime::getWindowWidth
    ()
    const
    {
        if (mWindowComponent != nullptr)
        {
            return mWindowComponent->getWidth();
        }
        return 0;
    }

    void
    ProjectRuntime::setWindowWidth
    (int w)
    {
        if (mWindowComponent != nullptr)
        {
            mWindowComponent->setWidth(w);
        }
    }

    int
    ProjectRuntime::getWindowHeight
    ()
    const
    {
        if (mWindowComponent != nullptr)
        {
            return mWindowComponent->getHeight();
        }
        return 0;
    }

    void
    ProjectRuntime::setWindowHeight
    (int h)
    {
        if (mWindowComponent != nullptr)
        {
            mWindowComponent->setHeight(h);
        }
    }

    void
    ProjectRuntime::collectGarbage
    (SceneRuntime* rt)
    {
        LOG_DEBUG("\n"
        "=======================================================================\n"
        "CollectGarbage Called @ {}\n"
        "=======================================================================",
        mTime->getAbsoluteTime());
        rt->collectGarbage();
    }

    void
    ProjectRuntime::collectGarbage
    ()
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        for (auto rt : mSceneRuntimesToRemove)
        {
            auto itr = find(mSceneRuntimeVector.begin(),mSceneRuntimeVector.end(),rt);
            if (itr != mSceneRuntimeVector.end())
            {
                mSceneRuntimeVector.erase(itr);
            }
        }
    }

    void
    ProjectRuntime::updateAll
    ()
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        if (mSceneRuntimeVector.empty())
        {
            mTaskManager->clearFences();
            mTaskManager->waitForFence();
            mGraphicsComponent->executeTaskQueue();
            mGraphicsComponent->executeDestructionTaskQueue();
        }
        else
        {
            for (auto rt : mSceneRuntimeVector)
            {
                switch (rt->getState())
                {
                    case SceneState::SCENE_STATE_TO_LOAD:
                        constructSceneRuntime(rt);
                        break;
                    case SceneState::SCENE_STATE_LOADED:
                        break;
                    case SceneState::SCENE_STATE_ACTIVE:
                        updateLogic(rt);
                        updateGraphics(rt);
                        mWindowComponent->updateWindow(rt);
                        collectGarbage(rt);
                        break;
                    case SceneState::SCENE_STATE_TO_DESTROY:
                        destructSceneRuntime(rt);
                        break;
                    case SceneState::SCENE_STATE_DESTROYED:
                        mSceneRuntimesToRemove.push_back(rt);
                        break;
                }
            }
        }
        collectGarbage();
    }

    SceneRuntime*
    ProjectRuntime::getActiveSceneRuntime
    ()
    const
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        int nRuntimes = mSceneRuntimeVector.size();
        for (int i=0;i<nRuntimes;i++)
        {
            if (mSceneRuntimeVector.at(i)->getState() == SceneState::SCENE_STATE_ACTIVE)
            {
                return mSceneRuntimeVector.at(i);
            }
        }
        return nullptr;
    }

    SceneRuntime*
    ProjectRuntime::getSceneRuntimeByUuid
    (uint32_t uuid)
    const
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        for (auto* sr : mSceneRuntimeVector)
        {
            if (sr->getUuid() == uuid)
            {
                return sr;
            }
        }
        return nullptr;
    }

    void
    ProjectRuntime::setSceneRuntimeAsActive
    (uint32_t uuid)
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        int nRuntimes = mSceneRuntimeVector.size();
        for (int i=0;i<nRuntimes;i++)
        {
            auto srt = mSceneRuntimeVector.at(i);
            if (srt->getUuid() == uuid)
            {
                srt->setState(SceneState::SCENE_STATE_ACTIVE);
            }
            else
            {
                if (srt->getState() == SceneState::SCENE_STATE_ACTIVE)
                {
                    srt->setState(SceneState::SCENE_STATE_LOADED);
                }
            }
        }
    }

    vector<SceneRuntime*>
    ProjectRuntime::getSceneRuntimeVector
    ()
    const
    {
        return mSceneRuntimeVector;
    }

    void
    ProjectRuntime::addSceneRuntime
    (SceneRuntime* rt)
    {
        auto itr = find(mSceneRuntimeVector.begin(), mSceneRuntimeVector.end(), rt);
        if (itr == mSceneRuntimeVector.end())
        {
            mSceneRuntimeVector.push_back(rt);
        }
    }

    void
    ProjectRuntime::removeSceneRuntime
    (SceneRuntime* rt)
    {
        auto itr = find(mSceneRuntimeVector.begin(), mSceneRuntimeVector.end(), rt);
        if (itr != mSceneRuntimeVector.end())
        {
            mSceneRuntimeVector.erase(itr);
        }
    }

    bool
    ProjectRuntime::constructSceneRuntime
    (SceneRuntime* rt)
    {
        LOG_DEBUG("ProjectRuntime: Constructing Scene Runtime");
        return rt->useDefinition();
    }

    Project*
    ProjectRuntime::getProject
    ()
    const
    {
        return mProject;
    }

    InputComponent*
    ProjectRuntime::getInputComponent
    ()
    const
    {
        return mInputComponent;
    }

    bool
    ProjectRuntime::useDefinition
    ()
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        if (!initCaches())
        {
            return false;
        }

        if (!initComponents())
        {
            return false;
        }

        return true;
    }

    AudioCache*
    ProjectRuntime::getAudioCache
    ()
    const
    {
        return mAudioCache;
    }

    ShaderCache*
    ProjectRuntime::getShaderCache
    ()
    const
    {
        return mShaderCache;
    }

    MaterialCache*
    ProjectRuntime::getMaterialCache
    ()
    const
    {
        return mMaterialCache;
    }

    ModelCache*
    ProjectRuntime::getModelCache
    ()
    const
    {
        return mModelCache;
    }

    TextureCache*
    ProjectRuntime::getTextureCache
    ()
    const
    {
        return mTextureCache;
    }

    ScriptCache*
    ProjectRuntime::getScriptCache
    ()
    const
    {
        return mScriptCache;
    }

    void
    ProjectRuntime::destructSceneRuntime
    (SceneRuntime* rt, bool clearCaches)
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        rt->destroyRuntime();
        if (clearCaches)
        {
            clearAllCaches();
        }
    }

    void
    ProjectRuntime::clearAllCaches
    ()
    {
        LOG_TRACE("ProjectRuntime: {}",__FUNCTION__);
        mTaskManager->clearFences();
        mTaskManager->waitForFence();
        if (mAudioCache != nullptr)
        {
            mAudioCache->lock();
            mAudioCache->clear();
            mAudioCache->unlock();
        }

        if (mModelCache != nullptr)
        {
            mModelCache->lock();
            mModelCache->clear();
            mModelCache->unlock();
        }

        if (mShaderCache != nullptr)
        {
            mShaderCache->lock();
            mShaderCache->clear();
            mShaderCache->unlock();
        }

        if (mMaterialCache != nullptr)
        {
            mMaterialCache->lock();
            mMaterialCache->clear();
            mMaterialCache->unlock();
        }

        if (mTextureCache != nullptr)
        {
            mTextureCache->lock();
            mTextureCache->clear();
            mTextureCache->unlock();
        }

        if (mScriptCache != nullptr)
        {
            mScriptCache->lock();
            mScriptCache->clear();
            mScriptCache->unlock();
        }
    }

    AssetDefinition*
    ProjectRuntime::getAssetDefinitionByUuid
    (uint32_t uuid)
    const
    {
        if (mProject != nullptr)
        {
            return mProject->getAssetDefinitionByUuid(uuid);
        }
        return nullptr;
    }

    EntityRuntime*
    ProjectRuntime::getEntityRuntimeByUuid
    (SceneRuntime* rt, uint32_t uuid)
    const
    {
        if (rt == nullptr)
        {
            return nullptr;
        }

        return rt->getEntityRuntimeByUuid(uuid);
    }

    bool
    ProjectRuntime::hasActiveScene
    ()
    const
    {
        for (auto srt : mSceneRuntimeVector)
        {
            srt->lock();
            if (srt->getState() == SceneState::SCENE_STATE_ACTIVE) {
                srt->unlock();
                return true;
            }
            srt->unlock();
        }
        return false;
    }

    bool
    ProjectRuntime::hasLoadedScenes
    ()
    const
    {
        for (auto srt : mSceneRuntimeVector)
        {
            srt->lock();
            if (srt->getState() >= SceneState::SCENE_STATE_LOADED &&
                srt->getState() < SceneState::SCENE_STATE_DESTROYED)
            {
                srt->unlock();
                return true;
            }
            srt->unlock();
        }
        return false;
    }


    bool
    ProjectRuntime::hasSceneRuntime
    (uint32_t uuid)
    const
    {
        bool result = false;
        for (auto srt : mSceneRuntimeVector)
        {
            srt->lock();
            if (srt->getUuid() == uuid)
            {
                result = true;
            }
            srt->unlock();

            if (result) break;
        }
        return result;
    }

    deque<float> ProjectRuntime::getFrameDurationHistory() const
    {
        return mFrameDurationHistory;
    }

    float ProjectRuntime::getAverageFramerate()
    {
       float f = 0.0;
       for (const auto& dur : mFrameDurationHistory)
       {
           f += dur;
       }
       return f/MaxFrameCount;
    }

    int ProjectRuntime::MaxFrameCount = 100;
}
