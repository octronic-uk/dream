
#include "QOpenGLWindowComponent.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPointF>
#include <QPainter>
#include <spdlog/spdlog.h>
#include <DreamCore.h>
#include <QChar>

WindowInputState::WindowInputState
()
    : // Init list
     mouseLastX(0),
     mouseLastY(0),
     mouseWheelLastX(0),
     mouseWheelLastY(0),
     shiftPressed(false),
     altPressed(false),
     ctrlPressed(false),
     wPressed(false),
     aPressed(false),
     sPressed(false),
     dPressed(false),
     upPressed(false),
     downPressed(false),
     leftPressed(false),
     rightPressed(false)
{

}

QOpenGLWindowComponent::QOpenGLWindowComponent
(QWidget* parent)
    : QOpenGLWidget(parent),
      IWindowComponent(),
      mProjectRuntimeHandle(nullptr),
      mGridHandle(nullptr),
      mSelectionHighlighterHandle(nullptr),
      mRelationshipTreeHandle(nullptr),
      mGridEnabled(true),
      mRelationshipTreeEnabled(true),
      mSelectionHighlighterEnabled(true),
      mPaintInProgress(false),
      mMaxFrameTimeValues(100)
{
    auto log = spdlog::get("QOpenGLWindowComponent");
    if (log==nullptr)
    {
        log = spdlog::stdout_color_mt("QOpenGLWindowComponent");
    }
    log->info("Constructing");
    //setFormat(format);
}

QOpenGLWindowComponent::~QOpenGLWindowComponent
()
{
    auto log = spdlog::get("QOpenGLWindowComponent");
    log->info("Destructing");
    mProjectRuntimeHandle = nullptr;
    mGridHandle = nullptr;
    mSelectionHighlighterHandle = nullptr;
    mRelationshipTreeHandle = nullptr;
}

void
QOpenGLWindowComponent::initializeGL
()
{
    auto log = spdlog::get("QOpenGLWindowComponent");
    // get context opengl-version
    log->info("OpenGL Version: {}.{}", format().majorVersion(), format().minorVersion());
    log->info("Context valid: {}",  context()->isValid());
    log->info("Really used OpenGl: {}.{}", context()->format().majorVersion(), context()->format().minorVersion());
    log->info("OpenGl information:");
    log->info("VENDOR:       {}", static_cast<const unsigned char*>(glGetString(GL_VENDOR)));
    log->info("RENDERDER:    {}", static_cast<const unsigned char*>(glGetString(GL_RENDERER)));
    log->info("VERSION:      {}", static_cast<const unsigned char*>(glGetString(GL_VERSION)));
    log->info("GLSL VERSION: {}", static_cast<const unsigned char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
}

void
QOpenGLWindowComponent::resizeGL
( int w, int h )
{
    setWidth(w);
    setHeight(h);
    mSizeHasChanged = true;
}

void
QOpenGLWindowComponent::paintGL
()
{
    auto log = spdlog::get("QOpenGLWindowComponent");
    if (mPaintInProgress)
    {
        log->error("Attempted to paint before previous paintGL call has finished");
        return;
    }

    mPaintInProgress = true;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    if (mProjectRuntimeHandle)
    {
        if (mProjectRuntimeHandle->hasActiveSceneRuntime())
        {
            updateInputState();
            SceneRuntime *sRuntime = mProjectRuntimeHandle->getActiveSceneRuntimeHandle();


            if (sRuntime->getState() != SCENE_STATE_STOPPED)
            {
                mProjectRuntimeHandle->updateLogic();
                //mProjectRuntimeHandle->updateAll();

                mProjectRuntimeHandle->updateGraphics();
                mProjectRuntimeHandle->collectGarbage();

                glm::mat4 viewMatrix = mProjectRuntimeHandle->getGraphicsComponentHandle()->getViewMatrix();
                glm::mat4 projectionMatrix = mProjectRuntimeHandle->getGraphicsComponentHandle()->getProjectionMatrix();

                glDisable(GL_DEPTH_TEST);
                if (mGridHandle)
                {
                    if(mGridEnabled)
                    {
                        if (!mGridHandle->isInitialised())
                        {
                            mGridHandle->init();

                            Constants::checkGLError("QOGLWC: After Grid Init");
                        }
                        mGridHandle->setViewMatrix(viewMatrix);
                        mGridHandle->setProjectionMatrix(projectionMatrix);
                        mGridHandle->draw();
                    }
                    else
                    {
                        log->error("QOGLWC: Grid Disabled");
                        Constants::checkGLError("QOGLWC: After Grid Draw");
                    }
                }

                if (mSelectionHighlighterHandle)
                {
                    if (mSelectionHighlighterEnabled)
                    {
                        if (!mSelectionHighlighterHandle->isInitialised())
                        {
                            mSelectionHighlighterHandle->init();
                            Constants::checkGLError("QOGLWC: SelectionHighlighter after Init");
                        }

                        mSelectionHighlighterHandle->setViewMatrix(viewMatrix);
                        mSelectionHighlighterHandle->setProjectionMatrix(projectionMatrix);
                        mSelectionHighlighterHandle->draw();
                        Constants::checkGLError("QOGLWC: SelectionHighlighter after draw");
                    }
                }

                if (mRelationshipTreeHandle)
                {
                    if (mRelationshipTreeEnabled)
                    {
                        if (!mRelationshipTreeHandle->isInitialised())
                        {
                            mRelationshipTreeHandle->init();
                            Constants::checkGLError("QOGLWC: RelTree after init");
                        }

                        mRelationshipTreeHandle->setViewMatrix(viewMatrix);
                        mRelationshipTreeHandle->setProjectionMatrix(projectionMatrix);
                        mRelationshipTreeHandle->draw();

                        Constants::checkGLError("QOGLWC: RelTree after draw");
                    }
                }
                drawStats();

            }
            else
            {
                mProjectRuntimeHandle = nullptr;
            }
        }
    }
    // If no active scene, blank screen
    else
    {
        glClearColor(0.0f,0.0f,0.0f,0.0f);
    }


    mPaintInProgress = false;
}

void
QOpenGLWindowComponent::drawStats()
{
    auto project = mActiveSceneRuntimeHandle->getProjectRuntimeHandle();
    double frame = project->getTimeHandle()->getFrameTimeDelta();
    mFrameTimes.push_back(frame);

    if (mFrameTimes.size() > mMaxFrameTimeValues)
    {
        mFrameTimes.erase(begin(mFrameTimes));
    }

    long long animation = project->getAnimationComponentHandle()->getUpdateTime();
    long long animationYield = project->getAnimationComponentHandle()->getYieldedTime();

    long long audio = project->getAudioComponentHandle()->getUpdateTime();
    long long  audioYield = project->getAudioComponentHandle()->getYieldedTime() ;

    long long graphics = project->getGraphicsComponentHandle()->getUpdateTime();
    long long graphicsYield = project->getGraphicsComponentHandle()->getYieldedTime();

    long long lua = project->getLuaEngineHandle()->getUpdateTime();
    long long luaYield = project->getLuaEngineHandle()->getYieldedTime();

    long long physics = project->getPhysicsComponentHandle()->getUpdateTime();
    long long physicsYield = project->getPhysicsComponentHandle()->getYieldedTime();

    QPainter painter(this);

    int topLeftX, topLeftY;
    topLeftX = 0;
    topLeftY = 0;

    QRectF rect(0,0,getWidth(),getHeight()/10);
    QBrush brush(QColor(0,0,0,128));

    painter.fillRect(rect,brush);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 16));
    QString text(
            "Frame:     %1s     %12 FPS\n"
            "Animation: %2µs  / %3µs | Audio: %4µs / %5µs\n"
            "Graphics:  %6µs  / %7µs | Lua:   %8µs / %9µs\n"
            "Physics:   %10µs / %11µs"
    );
    text = text
        .arg(frame)
        .arg(animation, 9, 10, QChar('0'))
        .arg(animationYield, 9, 10, QChar('0'))
        .arg(audio, 9, 10, QChar('0'))
        .arg(audioYield, 9, 10, QChar('0'))
        .arg(graphics, 9, 10, QChar('0'))
        .arg(graphicsYield, 9, 10, QChar('0'))
        .arg(lua, 9, 10, QChar('0'))
        .arg(luaYield, 9, 10, QChar('0'))
        .arg(physics, 9, 10, QChar('0'))
        .arg(physicsYield, 9,10, QChar('0'))
        .arg(1.0/averageFrameTime());

    painter.drawText(topLeftX, topLeftY,getWidth(), getHeight(), Qt::AlignLeft,text);
    painter.end();
}

double QOpenGLWindowComponent::averageFrameTime()
{
    double total = 0.0;
    for (double ft : mFrameTimes)
    {
        total += ft;
    }
    return total / mFrameTimes.size();
}

bool
QOpenGLWindowComponent::init
()
{
    return true;
}

void QOpenGLWindowComponent::updateComponent(){}

void QOpenGLWindowComponent::getCurrentDimensions(){}

void QOpenGLWindowComponent::swapBuffers(){}

void
QOpenGLWindowComponent::setProjectRuntimeHandle
(ProjectRuntime* engine)
{
    mProjectRuntimeHandle = engine;
}

void
QOpenGLWindowComponent::setGridHandle
(Grid* grid)
{
    mGridHandle = grid;
}

void
QOpenGLWindowComponent::setSelectionHighlighterHandle
(SelectionHighlighter* highlighter)
{
    mSelectionHighlighterHandle = highlighter;
}

void
QOpenGLWindowComponent::setRelationshipTreeHandle
(RelationshipTree* tree)
{
    mRelationshipTreeHandle = tree;
}

void
QOpenGLWindowComponent::wheelEvent
(QWheelEvent* event)
{
    if (mProjectRuntimeHandle)
    {
        QPoint pos = event->pixelDelta();
        int x = static_cast<int>( pos.x() );
        int y = static_cast<int>( pos.y() );
        mProjectRuntimeHandle->getCameraHandle()->processMouseMovement(-x,-y,true);
    }
}

void
QOpenGLWindowComponent::mouseMoveEvent
(QMouseEvent *event)
{
    auto pos = event->localPos();
    mMouseX = pos.x();
    mMouseY = pos.y();
}

void
QOpenGLWindowComponent::updateInputState
()
{
    auto log = spdlog::get("QOpenGLWindowComponent");
    if (!mProjectRuntimeHandle)
    {
        return;
    }

    Camera *camHandle = mProjectRuntimeHandle->getCameraHandle();
    SceneObjectRuntime *selected = nullptr;
    if (mSelectionHighlighterHandle)
    {
        selected = mSelectionHighlighterHandle->getSelectedObject();
    }

    float deltaTime = static_cast<float>
    (
        mProjectRuntimeHandle->getTimeHandle()->getFrameTimeDelta()
    );

    if (mInputState.wPressed)
    {
        camHandle->processKeyboard(Constants::CAMERA_MOVEMENT_FORWARD,deltaTime);
    }

    if(mInputState.aPressed)
    {
        camHandle->processKeyboard(Constants::CAMERA_MOVEMENT_LEFT,deltaTime);
    }

    if (mInputState.sPressed)
    {
        camHandle->processKeyboard(Constants::CAMERA_MOVEMENT_BACKWARD,deltaTime);
    }

    if(mInputState.dPressed)
    {
        camHandle->processKeyboard(Constants::CAMERA_MOVEMENT_RIGHT,deltaTime);
    }


    if (selected)
    {
        Transform3D transform = selected->getTransform();
        float moveAmount = mInputState.shiftPressed ?
                               mGridHandle->getMajorSpacing() :
                               mGridHandle->getMinorSpacing();
        if(mInputState.upPressed)
        {
            log->info("Moving Selected up");
            if (mInputState.altPressed)
            {
                transform.translateByY(moveAmount);
            }
            else
            {
                transform.translateByZ(moveAmount);
            }
        }

        if(mInputState.downPressed)
        {
            log->info("Moving Selected down");
            if (mInputState.altPressed)
            {
                transform.translateByY(-moveAmount);
            }
            else
            {
                transform.translateByZ(-moveAmount);
            }
        }

        if(mInputState.leftPressed)
        {
            log->info("Moving Selected left");
            transform.translateByX(-moveAmount);
        }

        if(mInputState.rightPressed)
        {
            log->info("Moving Selected right");
            transform.translateByX(moveAmount);
        }
        selected->setTransform(transform);
    }
}

void
QOpenGLWindowComponent::keyPressEvent
(QKeyEvent *event)
{
    auto log = spdlog::get("QOpenGLWindowComponent");
    log->info("Pressed Key {}", event->key());

    switch (event->key())
    {
        case Qt::Key_W:
            mInputState.wPressed = true;
            break;
        case Qt::Key_A:
            mInputState.aPressed = true;
            break;
        case Qt::Key_S:
            mInputState.sPressed = true;
            break;
        case Qt::Key_D:
            mInputState.dPressed = true;
            break;
        case Qt::Key_Shift:
            mInputState.shiftPressed = true;
            break;
        case Qt::Key_Alt:
            mInputState.altPressed = true;
            break;
        case Qt::Key_Up:
            mInputState.upPressed = true;
            break;
        case Qt::Key_Down:
            mInputState.downPressed = true;
            break;
        case Qt::Key_Left:
            mInputState.leftPressed = true;
            break;
        case Qt::Key_Right:
            mInputState.rightPressed = true;
            break;
    }
}

void
QOpenGLWindowComponent::setGridEnabled
(bool enabled)
{
    mGridEnabled = enabled;
}

void
QOpenGLWindowComponent::setSelectionHighlighterEnabled
(bool enabled)
{
    mSelectionHighlighterEnabled = enabled;
}

void
QOpenGLWindowComponent::setRelationshipTreeEnabled
(bool enabled)
{
    mRelationshipTreeEnabled = enabled;
}

void
QOpenGLWindowComponent::keyReleaseEvent
(QKeyEvent* event)
{
    auto log = spdlog::get("QOpenGLWindowComponent");
    log->info("Released Key {}", event->key());
    switch (event->key())
    {
        case Qt::Key_W:
            mInputState.wPressed = false;
            break;
        case Qt::Key_A:
            mInputState.aPressed = false;
            break;
        case Qt::Key_S:
            mInputState.sPressed = false;
            break;
        case Qt::Key_D:
            mInputState.dPressed = false;
            break;
        case Qt::Key_Shift:
            mInputState.shiftPressed = false;
            break;
        case Qt::Key_Alt:
            mInputState.altPressed = false;
            break;
        case Qt::Key_Up:
            mInputState.upPressed = false;
            break;
        case Qt::Key_Down:
            mInputState.downPressed = false;
            break;
        case Qt::Key_Left:
            mInputState.leftPressed = false;
            break;
        case Qt::Key_Right:
            mInputState.rightPressed = false;
            break;
    }
}

void QOpenGLWindowComponent::clearProjectRuntimeHandle()
{
   mProjectRuntimeHandle = nullptr;
}
