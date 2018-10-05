/*
 * SceneObjectRuntime.cpp
 *
 * Created: 16 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
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

#include "SceneObjectRuntime.h"

#include <iostream>

#include "SceneObjectDefinition.h"

#include "../SceneRuntime.h"

#include "../../Common/Constants.h"

#include "../../Components/Event.h"
#include "../../Components/Path/PathInstance.h"
#include "../../Components/Audio/AudioInstance.h"
#include "../../Components/Audio/AudioComponent.h"
#include "../../Components/Graphics/Font/FontInstance.h"
#include "../../Components/Graphics/Model/AssimpModelInstance.h"
#include "../../Components/Graphics/Shader/ShaderInstance.h"
#include "../../Components/Graphics/Light/LightInstance.h"
#include "../../Components/Graphics/Sprite/SpriteInstance.h"
#include "../../Components/Physics/PhysicsObjectInstance.h"
#include "../../Components/Physics/PhysicsComponent.h"
#include "../../Components/IAssetDefinition.h"

#include "../../Components/Audio/AudioDefinition.h"

#include "../../Components/Graphics/Font/FontDefinition.h"
#include "../../Components/Graphics/Light/LightDefinition.h"
#include "../../Components/Graphics/Model/ModelDefinition.h"
#include "../../Components/Graphics/Shader/ShaderDefinition.h"
#include "../../Components/Graphics/Sprite/SpriteDefinition.h"
#include "../../Components/Graphics/Shader/ShaderCache.h"

#include "../../Components/Path/PathDefinition.h"
#include "../../Components/Physics/PhysicsObjectDefinition.h"

#include "../../Components/Scripting/ScriptDefinition.h"
#include "../../Components/Scripting/ScriptInstance.h"
#include "../../Components/Scripting/Lua/LuaComponent.h"
#include "../../Components/Scripting/Lua/LuaScriptInstance.h"

#include "../../Project/Project.h"
#include "../../Project/ProjectRuntime.h"
#include "../../Project/ProjectDefinition.h"

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

using std::vector;
using std::flush;
using std::make_shared;
using glm::vec3;

namespace Dream
{
    SceneObjectRuntime::SceneObjectRuntime(
        SceneObjectDefinition* sd,
        SceneRuntime* sr
    ):  IRuntime(sd, sd->getName(), sd->getUuid()),
        mAudioInstance(nullptr),
        mPathInstance(nullptr),
        mModelInstance(nullptr),
        mShaderInstance(nullptr),
        mLightInstance(nullptr),
        mSpriteInstance(nullptr),
        mScriptInstance(nullptr),
        mPhysicsObjectInstance(nullptr),
        mFontInstance(nullptr),
        mTransform(nullptr),
        mSceneRuntimeHandle(sr),
        mParentRuntimeHandle(nullptr),
        mLoaded(false),
        mHasFocus(false),
        mDeleted(false),
        mHidden(false),
        mFollowsCamera(false)
    {
        setLogClassName("SceneObjectRuntime");
        auto log = getLog();
        log->trace( "Constructing Object" );
        mTransform = new Transform3D();
    }

    SceneObjectRuntime::~SceneObjectRuntime
    ()
    {
        auto log = getLog();
        log->trace( "Destroying Object" );

        for (auto child : mChildRuntimes)
        {
            if (child != nullptr)
            {
                delete child;
            }
        }
        mChildRuntimes.clear();

        removeAudioInstance();
        removePathInstance();
        removeModelInstance();
        //removeShaderInstance();
        removeLightInstance();
        removeSpriteInstance();
        removeScriptInstance();
        removePhysicsObjectInstance();
        removeFontInstance();

        if (mTransform != nullptr)
        {
            delete mTransform;
            mTransform = nullptr;
        }
    }

    void
    SceneObjectRuntime::removeAudioInstance
    ()
    {
        if (mAudioInstance != nullptr)
        {
            delete mAudioInstance;
            mAudioInstance = nullptr;
        }
    }

    void
    SceneObjectRuntime::removePathInstance
    ()
    {
        if (mPathInstance != nullptr)
        {
            delete mPathInstance;
            mPathInstance = nullptr;
        }
    }

    void
    SceneObjectRuntime::removeModelInstance
    ()
    {
        auto log = getLog();
        if (mModelInstance != nullptr)
        {
            log->info("Deleting ModelInstance for {}",getNameAndUuidString());
            delete mModelInstance;
            mModelInstance = nullptr;
        }
    }

    /*
    void
    SceneObjectRuntime::removeShaderInstance
    ()
    {
        if (mShaderInstance != nullptr)
        {
            delete mShaderInstance;
            mShaderInstance = nullptr;
        }
    }
    */

    void
    SceneObjectRuntime::removeLightInstance
    ()
    {
        if (mLightInstance != nullptr)
        {
            delete mLightInstance;
            mLightInstance = nullptr;
        }
    }

    void
    SceneObjectRuntime::removeSpriteInstance
    ()
    {
        if (mSpriteInstance != nullptr)
        {
            delete mSpriteInstance;
            mSpriteInstance = nullptr;
        }
    }

    void
    SceneObjectRuntime::removeScriptInstance
    ()
    {
        if (hasScriptInstance())
        {
            auto scriptComp =
            mSceneRuntimeHandle
                ->getProjectRuntime()
                ->getScriptComponent();
            if(scriptComp != nullptr)
            {
                scriptComp->removeFromScriptMap(this);
            }
        }

        if (mScriptInstance != nullptr)
        {
            delete mScriptInstance;
            mScriptInstance = nullptr;
        }
    }

    void
    SceneObjectRuntime::removePhysicsObjectInstance
    ()
    {
        if (hasPhysicsObjectInstance())
        {
            auto physicsComp = mSceneRuntimeHandle
                ->getProjectRuntime()
                ->getPhysicsComponent();
           if (physicsComp != nullptr)
           {
                physicsComp->removePhysicsObjectInstance(getPhysicsObjectInstance());
           }
        }

        if (mPhysicsObjectInstance != nullptr)
        {
            delete mPhysicsObjectInstance;
            mPhysicsObjectInstance = nullptr;
        }
    }

    void
    SceneObjectRuntime::removeFontInstance
    ()
    {
        if (mFontInstance != nullptr)
        {
            delete mFontInstance;
            mFontInstance = nullptr;
        }
    }

    void
    SceneObjectRuntime::resetTransform
    ()
    {
        resetTranslation();
        resetRotation();
        resetScale();
    }

    void
    SceneObjectRuntime::resetTranslation
    ()
    {
        setTranslation(0.0f, 0.0f, 0.0f);
    }

    void
    SceneObjectRuntime::resetRotation
    ()
    {
        setRotation(0.0f, 0.0f, 0.0f);
    }

    void
    SceneObjectRuntime::resetScale
    ()
    {
        setScale(1.0f, 1.0f, 1.0f);
    }

    void
    SceneObjectRuntime::setTranslation
    (vec3 translation)
    {
        mTransform->setTranslation(translation);
    }

    void
    SceneObjectRuntime::setRotation
    (vec3 rotation)
    {
        mTransform->setRotation(rotation);
    }

    void
    SceneObjectRuntime::setScale
    (vec3 scale)
    {
        mTransform->setScale(scale);
    }

    void
    SceneObjectRuntime::setTranslation
    (float x, float y, float z)
    {
        mTransform->setTranslation(x,y,z);
    }

    void
    SceneObjectRuntime::setRotation
    (float x, float y, float z)
    {
        mTransform->setRotation(x,y,z);
    }

    void
    SceneObjectRuntime::setScale
    (float x, float y, float z)
    {
        mTransform->setScale(x,y,z);
    }

    vec3
    SceneObjectRuntime::getRotation
    ()
    {
        return getTransform()->getRotation();
    }

    vec3
    SceneObjectRuntime::getScale
    ()
    {
        return getTransform()->getScale();
    }

    vec3
    SceneObjectRuntime::getTranslation
    ()
    {
        return getTransform()->getTranslation();
    }

    PathInstance*
    SceneObjectRuntime::getPathInstance
    ()
    {
        return mPathInstance;
    }

    AudioInstance*
    SceneObjectRuntime::getAudioInstance
    ()
    {
        return mAudioInstance;
    }

    AssimpModelInstance*
    SceneObjectRuntime::getModelInstance
    ()
    {
        return mModelInstance;
    }

    ScriptInstance*
    SceneObjectRuntime::getScriptInstance
    ()
    {
        return mScriptInstance;
    }

    const shared_ptr<ShaderInstance>&
    SceneObjectRuntime::getShaderInstance
    ()
    {
        return mShaderInstance;
    }

    LightInstance*
    SceneObjectRuntime::getLightInstance
    ()
    {
        return mLightInstance;
    }

    bool
    SceneObjectRuntime::hasLightInstance
    ()
    {
        return mLightInstance != nullptr;
    }

    bool
    SceneObjectRuntime::hasModelInstance
    ()
    {
        return mModelInstance != nullptr;
    }

    bool
    SceneObjectRuntime::hasShaderInstance
    ()
    {
        return mShaderInstance != nullptr;
    }

    bool
    SceneObjectRuntime::hasScriptInstance
    ()
    {
        return mScriptInstance != nullptr;
    }

    bool
    SceneObjectRuntime::hasSpriteInstance
    ()
    {
        return mSpriteInstance != nullptr;
    }

    bool
    SceneObjectRuntime::hasFontInstance
    ()
    {
        return mFontInstance != nullptr;
    }

    void
    SceneObjectRuntime::addAssetDefinitionUuidToLoad
    (string def)
    {
        mAssetDefinitionUuidLoadQueue.push_back(def);
    }

    vector<string>
    SceneObjectRuntime::getAssetDefinitionUuidsToLoad
    ()
    {
        return mAssetDefinitionUuidLoadQueue;
    }

    FontInstance*
    SceneObjectRuntime::getFontInstance
    ()
    {
        return mFontInstance;
    }

    PhysicsObjectInstance*
    SceneObjectRuntime::getPhysicsObjectInstance
    ()
    {
        return mPhysicsObjectInstance;
    }

    string
    SceneObjectRuntime::getTransformType
    ()
    const
    {
        return mTransform->getTransformType();
    }

    void
    SceneObjectRuntime::setTransformType
    (string transformType)
    {
        mTransform->setTransformType(transformType);
    }

    Transform3D*
    SceneObjectRuntime::getTransform
    ()
    {
        return mTransform;
    }

    void
    SceneObjectRuntime::initialTransform()
    {
        auto log = getLog();
        Transform3D* initial = dynamic_cast<SceneObjectDefinition*>(mDefinition)->getTransform();
        if (initial->isTypeOffset())
        {
            log->debug("Inheriting Offset Transform for {}",getNameAndUuidString());
            mTransform->offsetFrom(getParentRuntime()->getTransform(), initial);
        }
        else
        {
            mTransform->setTranslation(initial->getTranslation());
            mTransform->setOrientation(initial->getOrientation());
            mTransform->setScale(initial->getScale());
        }
    }

    void
    SceneObjectRuntime::setTransform
    (Transform3D* transform)
    {
        mTransform = transform;
    }

    bool
    SceneObjectRuntime::hasFocus
    ()
    const
    {
        return mHasFocus;
    }

    void
    SceneObjectRuntime::setHasFocus
    (bool focus)
    {
        mHasFocus = focus;
    }

    SpriteInstance*
    SceneObjectRuntime::getSpriteInstance
    ()
    {
        return mSpriteInstance;
    }

    bool
    SceneObjectRuntime::getLoadedFlag
    ()
    const
    {
        return mLoaded;
    }

    void
    SceneObjectRuntime::setLoadedFlag
    (bool loaded)
    {
        mLoaded = loaded;
    }

    bool
    SceneObjectRuntime::hasEvents
    ()
    const
    {
        return mEventQueue.size() > 0;
    }

    void
    SceneObjectRuntime::addEvent
    (Event event)
    {
        auto log = getLog();
        log->debug
        (
            "Event posted from {} to {}",
            event.getSender()->getNameAndUuidString(),
            getNameAndUuidString()
        );
        mEventQueue.push_back(event);
    }

    vector<Event>
    SceneObjectRuntime::getEventQueue
    ()
    const
    {
        return mEventQueue;
    }

    void SceneObjectRuntime::clearEventQueue()
    {
        auto log = getLog();
        log->debug("Clearing event queue");
        mEventQueue.clear();
    }

    void SceneObjectRuntime::collectGarbage()
    {
        auto log = getLog();
        log->debug("Collecting Garbage {}" ,getNameAndUuidString());

        vector<SceneObjectRuntime*> toDelete;

        for (auto child : mChildRuntimes)
        {
           if (child->getDeleted())
           {
                toDelete.push_back(child);
           }
        }

        for (auto child : toDelete)
        {
            log->debug("Deleting child {}",child->getNameAndUuidString());
            mChildRuntimes.erase
            (
                find
                (
                    mChildRuntimes.begin(),
                    mChildRuntimes.end(),
                    child
                )
            );
            delete child;
        }
    }

    bool
    SceneObjectRuntime::hasPhysicsObjectInstance
    ()
    {
        return mPhysicsObjectInstance != nullptr;
    }

    bool
    SceneObjectRuntime::hasPathInstance
    ()
    {
        return mPathInstance != nullptr;
    }

    bool
    SceneObjectRuntime::hasAudioInstance
    ()
    {
        return mAudioInstance != nullptr;
    }

    void
    SceneObjectRuntime::createAssetInstances
    ()
    {
        for (string aDefUuid : mAssetDefinitionUuidLoadQueue)
        {
            createAssetInstanceFromAssetDefinitionByUuid(aDefUuid);
        }
        mAssetDefinitionUuidLoadQueue.clear();
    }

    void
    SceneObjectRuntime::createAssetInstanceFromAssetDefinitionByUuid
    (string uuid)
    {
        auto log = getLog();
        log->info("Creating asset instance from uuid {}", uuid);
        auto project = mSceneRuntimeHandle->getProjectRuntime()->getProject();
        if (project != nullptr)
        {
            auto assetDefinition = project->getProjectDefinition()->getAssetDefinitionByUuid(uuid);
            createAssetInstance(assetDefinition);
        }
    }

    void
    SceneObjectRuntime::replaceAssetUuid
    (string uuid)
    {
        auto log = getLog();
        log->info("REPLACING asset instance from uuid {}", uuid);
        auto project = mSceneRuntimeHandle->getProjectRuntime()->getProject();
        if (project != nullptr)
        {
            auto assetDefinition = project->getProjectDefinition()->getAssetDefinitionByUuid(uuid);
            createAssetInstance(assetDefinition);
        }
    }


    void
    SceneObjectRuntime::createAssetInstance
    (IAssetDefinition* definition)
    {
        auto log = getLog();
        auto project = mSceneRuntimeHandle->getProjectRuntime()->getProject();

        if (project != nullptr)
        {
            mProjectPath = project->getProjectPath();
        }

        log->debug(
            "Creating Asset Intance of: ({}) {} for {}",
            definition->getType(),
            definition->getName(),
            getNameAndUuidString()
        );

        if(definition->isTypePath())
        {
            createPathInstance(dynamic_cast<PathDefinition*>(definition));
        }
        else if (definition->isTypeAudio())
        {
            createAudioInstance(dynamic_cast<AudioDefinition*>(definition));
        }
        else if (definition->isTypeModel())
        {
            createModelInstance(dynamic_cast<ModelDefinition*>(definition));
        }
        else if (definition->isTypeScript())
        {
            createScriptInstance(dynamic_cast<ScriptDefinition*>(definition));
        }
        else if (definition->isTypeShader())
        {
            createShaderInstance(dynamic_cast<ShaderDefinition*>(definition));
        }
        else if (definition->isTypePhysicsObject())
        {
            createPhysicsObjectInstance(dynamic_cast<PhysicsObjectDefinition*>(definition));
        }
        else if (definition->isTypeLight())
        {
            createLightInstance(dynamic_cast<LightDefinition*>(definition));
        }
        else if (definition->isTypeSprite())
        {
            createSpriteInstance(dynamic_cast<SpriteDefinition*>(definition));
        }
        else if (definition->isTypeFont())
        {
            createFontInstance(dynamic_cast<FontDefinition*>(definition));
        }
        else
        {
            log->error( "Invalid Asset Instance Type {}",definition->getType() );
        }

        setLoadedFlag(true);
    }

    void
    SceneObjectRuntime::createPhysicsObjectInstance
    (PhysicsObjectDefinition* definition)
    {
        removePhysicsObjectInstance();
        auto log = getLog();
        log->debug( "Creating Physics Object Asset Instance." );
        mPhysicsObjectInstance = new PhysicsObjectInstance(
            definition,
            mSceneRuntimeHandle->getProjectRuntime()->getPhysicsComponent(),
            this
        );
        mPhysicsObjectInstance->load(mProjectPath);
    }

    void
    SceneObjectRuntime::createPathInstance
    (PathDefinition* definition)
    {
        auto log = getLog();
        log->debug( "Creating Path asset instance." );
        removePathInstance();
        mPathInstance = new PathInstance(definition,this);
        mPathInstance->load(mProjectPath);
    }

    void
    SceneObjectRuntime::createAudioInstance
    (AudioDefinition* definition)
    {
        auto log = getLog();
        auto audioComp = mSceneRuntimeHandle->getProjectRuntime()->getAudioComponent();
        if (audioComp != nullptr)
        {
            removeAudioInstance();
            log->debug( "Creating Audio asset instance." );
            mAudioInstance = audioComp->newAudioInstance(definition,this);
            mAudioInstance->load(mProjectPath);
        }
        else
        {
            log->error("Cannot create AudioInstance. AudioComponent is nullptr");
        }
    }

    void
    SceneObjectRuntime::createModelInstance
    (ModelDefinition* definition)
    {
        auto log = getLog();
        removeModelInstance();
        log->info( "Creating Model asset instance." );
        mModelInstance = new AssimpModelInstance(
            mSceneRuntimeHandle->getProjectRuntime()->getModelCache(),
            mSceneRuntimeHandle->getProjectRuntime()->getTextureCache(),
            definition,
            this
        );
        mModelInstance->load(mProjectPath);
    }

    void
    SceneObjectRuntime::createScriptInstance
    (ScriptDefinition* definition)
    {
        auto log = getLog();
        removeScriptInstance();
        log->debug( "Creating Script asset instance." );
        mScriptInstance = new LuaScriptInstance(
            definition,
            this
        );
        mScriptInstance->load(mProjectPath);
        auto scriptComp = mSceneRuntimeHandle->getProjectRuntime()->getScriptComponent();
        if (scriptComp != nullptr)
        {
            scriptComp->addToScriptMap(this,mScriptInstance);
        }
    }

    void
    SceneObjectRuntime::createShaderInstance
    (ShaderDefinition* definition)
    {
        auto log = getLog();
        //removeShaderInstance();
        log->debug( "Creating Shader asset instance." );

        auto shaderCache = mSceneRuntimeHandle->getProjectRuntime()->getShaderCache();
        mShaderInstance = shaderCache->getShader(definition->getUuid());
        if (mShaderInstance == nullptr)
        {
            mShaderInstance = make_shared<ShaderInstance>(definition,this);
            mShaderInstance->load(mProjectPath);
            shaderCache->putShader(definition->getUuid(), mShaderInstance);
        }
    }

    void
    SceneObjectRuntime::createLightInstance
    (LightDefinition* definition)
    {
        auto log = getLog();
        removeLightInstance();
        log->debug( "Creating Light Asset instance." );
        mLightInstance = new LightInstance(
            definition,
            this
        );
        mLightInstance->load(mProjectPath);
    }

    void
    SceneObjectRuntime::createSpriteInstance
    (SpriteDefinition* definition)
    {
        auto log = getLog();
        removeSpriteInstance();
        log->debug( "Creating Sprite Asset instance." );
        mSpriteInstance = new SpriteInstance
        (
            mSceneRuntimeHandle->getProjectRuntime()->getTextureCache(),
            definition,
            this
        );
        mSpriteInstance->load(mProjectPath);
    }



    void
    SceneObjectRuntime::createFontInstance
    (FontDefinition* definition)
    {
        auto log = getLog();
        removeFontInstance();
        log->debug( "Creating Font Asset instance." );
        mFontInstance = new FontInstance
        (
            mSceneRuntimeHandle->getProjectRuntime()->getFontCache(),
            definition,
            this
        );
        mFontInstance->load(mProjectPath);
    }

    quat
    SceneObjectRuntime::getOrientation
    ()
    {
       return mTransform->getOrientation();
    }

    bool
    SceneObjectRuntime::applyToAll
    (function<bool(SceneObjectRuntime*)> funk)
    {
        auto log = getLog();
        log->trace(
            "{}::applyToAll(bool) applying to {} children",
            getNameAndUuidString(),
            mChildRuntimes.size()
        );

        bool retval = funk(this);

        for (auto it = begin(mChildRuntimes); it != end(mChildRuntimes); it++)
        {
            if (*it)
            {
                retval = retval || (*it)->applyToAll(funk);
            }
        }
        return retval;
    }

    SceneObjectRuntime*
    SceneObjectRuntime::applyToAll
    (function<SceneObjectRuntime*(SceneObjectRuntime*)> funk)
    {
        auto log = getLog();
        log->trace(
            "{}::applyToAll(void*) applying to {} children",
            getNameAndUuidString(),
            mChildRuntimes.size()
        );

        SceneObjectRuntime* retval = funk(this);

        if (retval != nullptr)
        {
            return retval;
        }

        for (auto it = begin(mChildRuntimes); it != end(mChildRuntimes); it++)
        {
            if ((*it) != nullptr)
            {
                retval = (*it)->applyToAll(funk);
                if (retval != nullptr)
                {
                    return retval;
                }
            }
        }
        return nullptr;
    }

    SceneObjectRuntime*
    SceneObjectRuntime::getChildRuntimeByUuid
    (string uuid)
    {
        for (auto it = begin(mChildRuntimes); it != end(mChildRuntimes); it++)
        {
            if (*it != nullptr)
            {
                if ((*it)->hasUuid(uuid))

                {
                    return *it;
                }
            }
        }
        return nullptr;
    }

    bool
    SceneObjectRuntime::isParentOf
    (SceneObjectRuntime*  child)
    {
        for (auto it = begin(mChildRuntimes); it != end(mChildRuntimes); it++)
        {
            if ((*it) == child)
            {
                return true;
            }
        }
        return false;
    }

    void
    SceneObjectRuntime::setParentRuntime
    (SceneObjectRuntime* parent)
    {
        mParentRuntimeHandle = parent;
    }

    SceneObjectRuntime*
    SceneObjectRuntime::getParentRuntime
    ()
    {
        return mParentRuntimeHandle;
    }

    SceneRuntime*
    SceneObjectRuntime::getSceneRuntime
    ()
    {
        return mSceneRuntimeHandle;
    }

    SceneObjectDefinition*
    SceneObjectRuntime::getSceneObjectDefinition()
    {
       return dynamic_cast<SceneObjectDefinition*>(getDefinition());
    }

    void
    SceneObjectRuntime::useDefinition
    ()
    {
        auto def = dynamic_cast<SceneObjectDefinition*>(mDefinition);
        auto log = getLog();
        log->debug( "Using Definition {}", def->getNameAndUuidString());
        setName(def->getName());
        setUuid(def->getUuid());
        setFollowsCamera(def->followsCamera());
        setAssetDefinitionLoadQueue(def->getAssetDefinitionLoadQueue());
        setHasFocus(def->hasFocus());
        setHidden(def->getHidden());
        initialTransform();
        createAssetInstances();
        loadChildrenFromDefinition(def);
    }

    void
    SceneObjectRuntime::setAssetDefinitionLoadQueue
    (vector<string> loadQueue)
    {
        mAssetDefinitionUuidLoadQueue = loadQueue;
    }

    void
    SceneObjectRuntime::loadChildrenFromDefinition
    (SceneObjectDefinition* definition)
    {
        vector<SceneObjectDefinition*> definitions = definition->getChildDefinitionsList();
        for
        (auto it = begin(definitions); it != end(definitions); it++)
        {
            SceneObjectRuntime* child = new SceneObjectRuntime(*it, mSceneRuntimeHandle);
            child->setParentRuntime(this);
            child->useDefinition();
            mChildRuntimes.push_back(child);
        }
    }

    bool
    SceneObjectRuntime::followsCamera
    () const
    {
        return mFollowsCamera;
    }

    void
    SceneObjectRuntime::setFollowsCamera
    (bool followsCamera)
    {
        mFollowsCamera = followsCamera;
    }

    void
    SceneObjectRuntime::drive
    (float leftStickX, float leftStickY)
    {
        auto log = getLog();
        log->trace("Walk: ({},{}) ",leftStickX,leftStickY);

        if (leftStickX == 0.0f && leftStickY == 0.0f)
        {
            return;
        }

        // Translate to current
        mat4 mtx;
        mtx = glm::translate(mat4(1), mTransform->getTranslation());
         // Rotate to current
        mat4 rotMat = mat4_cast(mTransform->getOrientation());
        mtx = mtx * rotMat;

        // Rotate to new
        quat q;
        quat newRotation = glm::rotate(q,leftStickX,vec3(0,1,0));
        mat4 newRotMat = mat4_cast(newRotation);
        mtx = mtx * newRotMat;

        // Translate to new
        mtx = glm::translate(mtx,vec3(0,0,-leftStickY));
        mTransform->setOrientation(quat(mtx));
        mTransform->setTranslation(glm::vec3(mtx[3]));

        if (mPhysicsObjectInstance != nullptr)
        {
            auto rb = mPhysicsObjectInstance->getRigidBody();
            if (rb!=nullptr)
            {
                rb->setCenterOfMassTransform(mTransform->getTransformAsBtTransform());
            }
        }
    }

    bool SceneObjectRuntime::getDeleted() const
    {
        return mDeleted;
    }

    void SceneObjectRuntime::setDeleted(bool deleted)
    {
        mDeleted = deleted;
    }

    bool SceneObjectRuntime::getHidden() const
    {
        return mHidden;
    }

    void SceneObjectRuntime::setHidden(bool hidden)
    {
        mHidden = hidden;
    }

    void
    SceneObjectRuntime::walk
    (float leftStickX, float leftStickY)
    {

    }
}
