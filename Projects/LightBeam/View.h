#pragma once

#include "DataStructure/ImageCells.h"
#include "Interface/Mouse.h"
#include <qlabel.h>
class QResizeEvent;
#include <QWidget>

namespace Realisim
{
namespace LightBeam
{
    class Broker;

    class View : public QWidget
    {
        Q_OBJECT
    public:
        View(QWidget *ipParent, Broker *ipBroker);
        ~View() = default;

        void initialize();
        void updateUi();
        
    signals:
        void viewChanged();
        
    protected:
        Broker& getBroker();
        virtual void mousePressEvent(QMouseEvent *) override;
        virtual void mouseReleaseEvent(QMouseEvent *) override;
        void reconstructImage();
        void reconstructImage(ImageCells::Node*, int iDepth = 0);
        virtual void resizeEvent(QResizeEvent *) override;

        //--- ui
        QLabel *mpImageLabel;
        
        //--- data
        Broker &mBrokerRef;
        Interface::Mouse mMouse;
    };
}
}