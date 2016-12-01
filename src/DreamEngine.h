#ifndef DREAMENGINE_H
#define DREAMENGINE_H

#include <iostream>
#include <thread>

#include <SDL2/SDL.h>

#include "ArgumentParser.h"
#include "Time.h"
#include "FileReader.h"
#include "Scene.h"
#include "Project.h"
#include "AssetInstance.h"
#include "AssetManager.h"

#include "Components/Graphics/Camera.h"
#include "Components/Animation/AnimationComponent.h"
#include "Components/Audio/AudioComponent.h"
#include "Components/Physics/PhysicsComponent.h"
#include "Components/Graphics/GraphicsComponent.h"

namespace Dream {
  class DreamEngine {
  private:
    Time *mTime;
    Project *mProject;
    Scene* mActiveScene;
    AssetManager *mAssetManager;
    Camera *mCamera;
    AudioComponent *mAudioComponent;
    GraphicsComponent *mGraphicsComponent;
    PhysicsComponent *mPhysicsComponent;
    AnimationComponent *mAnimationComponent;
    bool mDone;
    SDL_Event mEvent;
  public:
    DreamEngine(void);
    ~DreamEngine(void);
    bool initSDL();
    bool loadFromArgumentParser(ArgumentParser*);
    bool loadProjectFromFileReader(string projectPath, FileReader*);
    bool loadScene(Scene*);
    Project* getProject();
    void setProject(Project*);
    bool isProjectLoaded();

    bool createAssetManager();
    AssetManager* getAssetManager();

    bool bootstrap();
    bool update();
    void setTime(Time*);
    Time* getTime();
    void setDone(bool);
    bool isDone();

    void   setActiveScene(Scene*);
    Scene *getActiveScene();
    bool   hasActiveScene();
    bool   initActiveScene();

    void setAssetManager(AssetManager*);
    Camera* getCamera();
    void setCamera(Camera*);

    void updateComponents();
    bool createComponents();
    void destroyComponents();
    bool createAnimationComponent();
    bool createAudioComponent();
    bool createPhysicsComponent();
    bool createGraphicsComponent();

    AnimationComponent* getAnimationComponent();
    void setAnimationComponent(AnimationComponent*);

    AudioComponent* getAudioComponent();
    void setAudioComponent(AudioComponent*);

    PhysicsComponent* getPhysicsComponent();
    void setPhysicsComponent(PhysicsComponent*);

    GraphicsComponent* getGraphicsComponent();
    void setGraphicsComponent(GraphicsComponent*);

    map<SceneObject*,LuaScriptInstance*>* getLuaScriptMap();
    SDL_Event getSDL_Event();

  }; // End of Dream
} // End of Dream

#endif // DREAMENGINE_H
