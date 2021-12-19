
#pragma once

class QExposeEvent;
class QMouseEvent;
class QOpenGLContext;
#include <QWindow>
#include "Reactor/Engine.h"
#include <Windows.h>


namespace Realisim
{
    /*
    
    Note on events:
        The GlWindow is not really a QWidget and treats event (mouseMove, keyPress...) on its
        own. It is not really possible (not that I know) to pass event from child Qwindow to
        container QWidget parent (usually done by calling ignore() on the event). For that
        reason, GlWindow will emit some signals to notify the parent Widget on certain things
        such as fullScreenToggled().
    */

    class GlWindow : public QWindow
    {
        Q_OBJECT
    public:
        GlWindow() = delete;
        explicit GlWindow(QWindow* ipParent, QOpenGLContext* ipSharedContext, Reactor::Engine *ipEngine);
        virtual ~GlWindow();

        QOpenGLContext* getQOpenGlContext();
        HGLRC getOpenGlNativeHandle() const;
        HDC getOpenGlNativeDC() const;

    signals:
        void fullScreenToggled();
        void glContextHasBeenInitialized();

    protected:
        virtual void exposeEvent(QExposeEvent *e) override;
        virtual void keyPressEvent(QKeyEvent *e) override;
        virtual void keyReleaseEvent(QKeyEvent *e) override;
        virtual void mouseMoveEvent(QMouseEvent *e) override;
        virtual void mousePressEvent(QMouseEvent *e) override;
        virtual void mouseReleaseEvent(QMouseEvent *e) override;
        virtual void resizeEvent(QResizeEvent *e) override;
        virtual void wheelEvent(QWheelEvent* e) override;

        QOpenGLContext *mpContext;
        QOpenGLContext *mpSharedGlContext;
        Reactor::Engine &mEngine; //
        bool mShouldInitializeContext;
    };
}