
#include "Broker.h"
#include <QImage>
#include <qlayout.h>
#include <QResizeEvent>
#include <stdint.h>
#include "View.h"

using namespace Realisim;
    using namespace Core;
    using namespace Interface;
    using namespace LightBeam;

//-----------------------------------------------------------------------------
View::View(QWidget *ipParent, Broker *ipBroker) : QWidget(ipParent),
    mBrokerRef(*ipBroker)
{
    QHBoxLayout *pLyt = new QHBoxLayout(this);
    pLyt->setMargin(0);
    pLyt->setSpacing(0);
    {
        mpImageLabel = new QLabel("nothing yet...");
        
        pLyt->addWidget(mpImageLabel);
    }
    
}

//-----------------------------------------------------------------------------
Broker& View::getBroker()
{ return mBrokerRef; }

//-----------------------------------------------------------------------------
void View::mousePressEvent(QMouseEvent *ipE)
{
    mMouse.setButtonPressed(Mouse::bLeft);
    mMouse.setPosition(ipE->x(), ipE->y());
    
    emit viewChanged();
}

//-----------------------------------------------------------------------------
void View::mouseReleaseEvent(QMouseEvent *ipE)
{
    mMouse.setButtonReleased(Mouse::bLeft);
    mMouse.setPosition(ipE->x(), ipE->y());
}

//-----------------------------------------------------------------------------
void View::resizeEvent(QResizeEvent *ipE)
{
    Broker &b = getBroker();
    
    // -2 to leave a bit of room to down size the widget...
    //
    Math::Vector2i s(ipE->size().width() - 2, ipE->size().height() - 2);
    b.getFrameBuffer().setSize(s);
    
    updateUi();
}

//-----------------------------------------------------------------------------
void View::updateUi()
{
    Broker &b = getBroker();
    FrameBuffer &fRef = b.getFrameBuffer();
    if(fRef.isValid())
    {
        Image im = fRef.getColorBuffer();
        
        QImage qim((const uint8_t*)im.getImageData().constData(),
            im.getWidth(),
            im.getHeight(),
            QImage::Format_RGBA8888);
        mpImageLabel->setPixmap(QPixmap::fromImage(qim));
    }
}