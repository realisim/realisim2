
#include "Broker.h"
#include "Math/Vector.h"
#include <QImage>
#include <qlayout.h>
#include <QResizeEvent>
#include <stdint.h>
#include "View.h"

using namespace Realisim;
    using namespace Core;
    using namespace Interface;
    using namespace LightBeam;
    using namespace Math;
    using namespace Rendering;

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
void View::initialize()
{
    Broker &b = getBroker();
    
    // resize the projection and viewport of the camera
    Camera &cRef = b.getCamera();
    
    cRef.set(Vector3(0, 0, 100),
             Vector3(0, 0, 0),
             Vector3(0, 1, 0));
    
    Projection p;
    p.setPerspectiveProjectionWithRatio(70, width() / height(), 1.0, 5000.0);
    
    cRef.setProjection(p);
    cRef.setProjectionProportionalToViewport(true);
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
    const int w = ipE->size().width();
    const int h = ipE->size().height();
    Broker &b = getBroker();
    
    // resize the projection and viewport of the camera
    Camera &cRef = b.getCamera();
    Viewport v;
    v.set(w, h);
    cRef.setViewport(v);
    
    // resize the frame buffer.
    // -2 to leave a bit of room to down size the widget...
    //
    Math::Vector2i s(w - 2, h - 2);
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
        
        //QImage has image origin at topLeft while
        //we have bottomleft... lets flip it.
        qim = qim.mirrored(false, true);
        
        mpImageLabel->setPixmap(QPixmap::fromImage(qim));
    }
}