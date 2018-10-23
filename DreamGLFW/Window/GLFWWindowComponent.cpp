/*
 * GLFWWindowComponent.cpp
 *
 * Created: 11 2017 by Ashley
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

#include "GLFWWindowComponent.h"

using Dream::Constants;
using Dream::SceneRuntime;
using Dream::SceneState;

static bool WindowSizeChanged = false;

namespace DreamGLFW
{
    void
    FramebufferSizeCallback
    (GLFWwindow* windows98, int w, int h)
    {
        WindowSizeChanged = true;
    }

    GLFWWindowComponent::GLFWWindowComponent
    () : IWindowComponent()
    {
        auto log = spdlog::get("GLFWWindowComponent");
        if (log == nullptr)
        {
            log = spdlog::stdout_color_mt("GLFWWindowComponent");
        }
        log->info("Constructing" );
        mName = "Dream";
    }

    GLFWWindowComponent::~GLFWWindowComponent
    ()
    {
        auto log = spdlog::get("GLFWWindowComponent");
        log->info("Destructing" );
        glfwTerminate();
        mWindow = nullptr;
    }

    bool
    GLFWWindowComponent::init
    ()
    {
        if (!initGLFW())
        {
            return false;
        }

        if (!initGL())
        {
            return false;
        }

        return true;
    }

    void
    GLFWWindowComponent::bindDefaultFrameBuffer
    ()
    {
        glBindFramebuffer(GL_FRAMEBUFFER,0);
    }

    bool
    GLFWWindowComponent::initGLFW
    ()
    {
        auto log = spdlog::get("GLFWWindowComponent");
        log->debug("Initialising GLFW");

        /* Initialize the library */
        if (!glfwInit())
        {
            log->error("FAILED @ Initialising GLFW");
            return false;
        }

        /* Create a windowed mode window and its OpenGL context */
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        mWindow = glfwCreateWindow(mWidth, mHeight, mName.c_str(), nullptr,nullptr);

        if (mWindow == nullptr)
        {

            log->error("FAILED @ Make Window");
            glfwTerminate();
            return false;
        }

        // Resize callback
        glfwSetFramebufferSizeCallback(mWindow, FramebufferSizeCallback);
        glfwSwapInterval(0);
        return true;
    }

    bool
    GLFWWindowComponent::initGL
    ()
    {
        auto log = spdlog::get("GLFWWindowComponent");
        log->debug("Initialising GLFW::OpenGL");
        /* Make the window's context current */
        glfwMakeContextCurrent(mWindow);

        return true;
    }

    void
    GLFWWindowComponent::updateComponent
    ()
    {
        auto log = spdlog::get("GLFWWindowComponent");
        auto asr = mActiveSceneRuntime;
        if (asr == nullptr)
        {
            log->error("No active scene runtime");
            return;
        }

        if(glfwWindowShouldClose(mWindow))
        {
           mActiveSceneRuntime->setState(Dream::SCENE_STATE_STOPPED);
        }

        if (WindowSizeChanged)
        {
            getCurrentDimensions();
            WindowSizeChanged = false;
        }

    }

    void GLFWWindowComponent::getCurrentDimensions()
    {
        glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);
        cout << "Framebuffer Size Changed: " << mWidth << "," << mHeight << endl;
        mSizeHasChanged = true;
    }

    void GLFWWindowComponent::swapBuffers()
    {
        if (mWindow != nullptr)
        {
            glfwSwapBuffers(mWindow);
        }

        glfwPollEvents();
    }

} // End of Dream

