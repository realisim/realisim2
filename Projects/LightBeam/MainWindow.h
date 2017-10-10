/*
 */

#pragma once

#include <QMainWindow>
#include <QTimerEvent>
#include "Raytracer.h"

namespace Realisim
{
namespace LightBeam
{
    class Broker;
    class View;

    //----------------------------------------------------
    class MainWindow : public QMainWindow
    {
        Q_OBJECT
    public:
        MainWindow(Broker *ipBroker);
        ~MainWindow() = default;

    protected slots:
        void viewChanged();
        
    protected:
        Broker& getBroker();
        virtual void timerEvent(QTimerEvent *) override;
        void updateUi();

        int mUpdateTimerId;
        Broker &mBrokerRef;
        View *mpView;
        RayTracer mRayTracer;
    };
}
}


