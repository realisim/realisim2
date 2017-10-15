/*
 */

#pragma once

#include <QMainWindow>
#include <QTimerEvent>

namespace Realisim
{
namespace LightBeam
{
    class Broker;
    class RayTracer;
    class View;

    //----------------------------------------------------
    class MainWindow : public QMainWindow
    {
        Q_OBJECT
    public:
        MainWindow(Broker *ipBroker, RayTracer *ipRayTracer);
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
        RayTracer &mRayTracerRef;
    };
}
}


