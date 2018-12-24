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


#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include "GLFWWindowComponent.h"
#include "../../DreamCore/Scene/SceneRuntime.h"

using namespace Dream;

static bool WindowSizeChanged = false;

namespace DreamGLFW
{
    void
    FramebufferSizeCallback
    (GLFWwindow*, int, int)
    {
        WindowSizeChanged = true;
    }

    GLFWWindowComponent::GLFWWindowComponent
    () : WindowComponent(),
         mDPIScaleX(1.0f),
         mDPIScaleY(1.0f)
    {
        setLogClassName("GLFWWindowComponent");
        auto log = getLog();
        log->info("Constructing" );
        mName = "Dream";
    }

    GLFWWindowComponent::~GLFWWindowComponent
    ()
    {
        auto log = getLog();
        log->info("Destructing" );
        glfwTerminate();
        mWindow = nullptr;
    }

    GLFWwindow*
    GLFWWindowComponent::getGlfwWindow
    ()
    {
       return mWindow;
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
        checkGLError();
    }

    bool
    GLFWWindowComponent::initGLFW
    ()
    {
        auto log = getLog();
        log->debug("Initialising GLFW");

        /* Initialize the library */
        if (!glfwInit())
        {
            log->error("FAILED @ Initialising GLFW");
            return false;
        }

        /* Create a windowed mode window and its OpenGL context */
#ifdef WIN32
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
#endif
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
        //glfwGetMonitorContentScale(glfwGetPrimaryMonitor(),mDPIScaleX,mDPIScaleY); Requires GLFW >=3.3
        glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);
        log->debug("Queried Framebuffer size as {}x{}",mWidth,mHeight);
        return true;
    }

    bool
    GLFWWindowComponent::initGL
    ()
    {
        auto log = getLog();
        log->debug("Initialising GLFW::OpenGL");
        glfwMakeContextCurrent(mWindow);

        glewExperimental = GL_TRUE;
        GLenum glewInitResult = glewInit();

        if (glewInitResult != GLEW_OK)
        {
            log->error("GLEW failed to initialise");
            return false;
        }

        checkGLError();

        log->debug(
            "OpenGL Version {}, Shader Version {}",
            glGetString(GL_VERSION),
            glGetString(GL_SHADING_LANGUAGE_VERSION)
        );
        return true;
    }

    void
    GLFWWindowComponent::updateComponent
    (SceneRuntime* sr)
    {
        auto log = getLog();

        glfwPollEvents();

        if(glfwWindowShouldClose(mWindow))
        {
            if (sr != nullptr)
            {
                sr->setState(Dream::SCENE_STATE_TO_DESTROY);
            }
            setShouldClose(true);
            log->error("Window should close");
        }

        if (WindowSizeChanged)
        {
            getCurrentDimensions();
            WindowSizeChanged = false;
        }
    }

    void
    GLFWWindowComponent::getCurrentDimensions
    ()
    {
        auto log = getLog();
        glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);
        log->error("Framebuffer Size Changed: {}x{}", mWidth ,mHeight);
        mSizeHasChanged = true;
    }

    void
    GLFWWindowComponent::swapBuffers
    ()
    {
        if (mWindow != nullptr)
        {
            glfwSwapBuffers(mWindow);
        }
    }
}

