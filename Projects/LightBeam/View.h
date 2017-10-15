#pragma once

#include "DataStructure/ImageCells.h"
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
        void reconstructImage();
        void updateUi();
        
    signals:
        void viewChanged();
        
    protected:
        Broker& getBroker();
        virtual void keyPressEvent(QKeyEvent *) override;
        virtual void keyReleaseEvent(QKeyEvent *) override;
        virtual void mousePressEvent(QMouseEvent *) override;
        virtual void mouseMoveEvent(QMouseEvent *) override;
        virtual void mouseReleaseEvent(QMouseEvent *) override;
        virtual void resizeEvent(QResizeEvent *) override;

        //--- ui
        QLabel *mpImageLabel;
        
        //--- data
        Broker &mBrokerRef;
    };
}
}