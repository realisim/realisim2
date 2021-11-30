

#include <GlWindow.h>
#include <QMainWindow>
#include <QMouseEvent>
#include <QOpenGLContext>
#include <QtPlatformHeaders/QWGLNativeContext>
#include "Reactor/Systems/Renderer/Renderer.h"
#include "Rendering/Camera.h"
//#include "Rendering/FrameBufferObject.h"

using namespace Realisim;

namespace
{
    //-------------------------------------------------------------------------
    // Since the Key enum is taken from Qt, the hexadecimal values are
    // simply compatible...
    Realisim::Key fromQtKey(int iKey)
    {
        return Key(iKey);
    }
}

//------------------------------------------------------------------------------
GlWindow::GlWindow(QWindow *ipParent, QOpenGLContext* ipSharedContext, Reactor::Engine *ipEngine) :
    QWindow(ipParent),
    mpContext(nullptr),
    mpSharedGlContext(ipSharedContext),
    mEngine(*ipEngine),
    mShouldInitializeContext(true)
{
    setSurfaceType(QWindow::OpenGLSurface);
    mpContext = new QOpenGLContext(this);
}

//------------------------------------------------------------------------------
GlWindow::~GlWindow()
{
    delete mpContext;
}

//------------------------------------------------------------------------------
void GlWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed() && mShouldInitializeContext)
    {
        //assign openGLContext
        mpContext->setFormat(requestedFormat());
        mpContext->setShareContext(mpSharedGlContext);
        mpContext->create();

        mpContext->makeCurrent(this);

        glClearColor(0.5, 0.0, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        mpContext->swapBuffers(this);
        mpContext->doneCurrent();

        mShouldInitializeContext = false;

        emit glContextHasBeenInitialized();
    }
}

//------------------------------------------------------------------------------
QOpenGLContext* GlWindow::getQOpenGlContext()
{
    return mpContext;
}

//------------------------------------------------------------------------------
HGLRC GlWindow::getOpenGlNativeHandle() const
{
    HGLRC r = nullptr;
    if(mpContext->isValid())
    {
        QVariant v = mpContext->nativeHandle();
        if (v.canConvert<QWGLNativeContext>())
        {
            r = v.value<QWGLNativeContext>().context();
        }
        
    }
    return r;
}

//------------------------------------------------------------------------------
HDC GlWindow::getOpenGlNativeDC() const
{
    HDC r = nullptr;
    HWND hwnd = (HWND)this->winId();
    r = GetDC(hwnd);

    return r;
}

//------------------------------------------------------------------------------
void GlWindow::keyPressEvent(QKeyEvent *e)
{
    Interface::Keyboard& k = mEngine.getBrokerRef().getKeyboardRef();
    k.setKeyPressed(fromQtKey(e->key()));
    
    e->ignore();
}

//------------------------------------------------------------------------------
void GlWindow::keyReleaseEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    //case Qt::Key_W: break;
    case Qt::Key_F11: emit fullScreenToggled(); break;
    default:  break;
    }

    Interface::Keyboard& k = mEngine.getBrokerRef().getKeyboardRef();
    k.setKeyReleased(fromQtKey(e->key()));

    e->ignore();
}

//------------------------------------------------------------------------------
void GlWindow::mouseMoveEvent(QMouseEvent *e)
{
    Interface::Mouse& m = mEngine.getBrokerRef().getMouse();

    //modify input to engine.
    m.setPosition(e->x(), e->y());

    e->ignore();
}

//------------------------------------------------------------------------------
void GlWindow::mousePressEvent(QMouseEvent *e)
{
    using namespace Interface;
    Mouse& m = mEngine.getBrokerRef().getMouse();

    //modify input to engine.
    switch(e->button())
    {
    case Qt::LeftButton: m.setButtonPressed(e->x(), e->y(), Mouse::bLeft); break;
    case Qt::MiddleButton: m.setButtonPressed(e->x(), e->y(), Mouse::bMiddle); break;
    case Qt::RightButton: m.setButtonPressed(e->x(), e->y(), Mouse::bRight); break;
    default: break;
    }        

    e->ignore();
}

//------------------------------------------------------------------------------
void GlWindow::mouseReleaseEvent(QMouseEvent *e)
{
    using namespace Interface;
    Mouse& m = mEngine.getBrokerRef().getMouse();

    //modify input to engine.
    switch (e->button())
    {
    case Qt::LeftButton: m.setButtonReleased(e->x(), e->y(), Mouse::bLeft); break;
    case Qt::MiddleButton: m.setButtonReleased(e->x(), e->y(), Mouse::bMiddle); break;
    case Qt::RightButton: m.setButtonReleased(e->x(), e->y(), Mouse::bRight); break;
    default: break;
    }

    e->ignore();
}

//------------------------------------------------------------------------------
void GlWindow::resizeEvent(QResizeEvent *e)
{
    const int w = e->size().width();
    const int h = e->size().height();
    Reactor::Renderer& r = mEngine.getHub().getRendererRef();
    r.resizeGl(w, h);

    const Math::Vector2i o(0, 0);
    const Math::Vector2i s(w, h);
    //Rendering::FrameBufferObject::setDefaultFrameBufferId(getQOpenGlContext()->defaultFramebufferObject());
    //Rendering::FrameBufferObject::setDefaultFrameBufferViewport(o, s);

    e->ignore();
}

//------------------------------------------------------------------------------
void GlWindow::wheelEvent(QWheelEvent* e)
{
    using namespace Interface;
    Mouse& m = mEngine.getBrokerRef().getMouse();

    QPoint deltaInDegrees = e->angleDelta() / 8;
    m.setWheelDeltaInDegrees(deltaInDegrees.x(), deltaInDegrees.y());

    e->ignore();
}