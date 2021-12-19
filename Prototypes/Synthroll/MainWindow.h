
#pragma once

#include "GlWindow.h"
#include <map>
class QDockWidget;
class QKeyEvent;
#include <QMainWindow>
#include <QTimerEvent>

namespace Realisim
{

    //-------------------------------------------------------------------------
    class MainWindow : public QMainWindow
    {
        Q_OBJECT
    public:
        MainWindow() = delete;
        explicit MainWindow(Reactor::Engine *ipEngine);
        ~MainWindow() = default;

    protected slots:
        void aboutClicked();
        void handleFullScreenToggled();
        void handleGlContextIsInitialized();
        Reactor::Engine& MainWindow::getEngine();

    protected:      
        void createMenus();
        virtual void keyReleaseEvent(QKeyEvent *ipE) override;
        virtual void timerEvent(QTimerEvent *ipE) override;
        void updateUi();
        void updateUiAtHighFrequency();
        
        //--- Ui
        QWidget *mpCentralWidget;
        GlWindow *mpGlWindow;
        
        //--- Data
        Reactor::Engine &mEngine;
        int mTimerId;
        //Simthetiq::Core::Timer mTimerHighFrequencyRefreshTimer;
    };

}

