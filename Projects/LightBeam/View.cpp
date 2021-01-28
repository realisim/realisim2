
#include "Broker.h"
#include "Core/Timer.h"
#include "Geometry/Rectangle.h"
#include "Interface/Keyboard.h"
#include "Interface/Mouse.h"
#include "Math/Vector.h"
#include <QImage>
#include <qlayout.h>
#include <QResizeEvent>
#include <stdint.h>
#include "View.h"

using namespace Realisim;
    using namespace Core;
    using namespace Geometry;
    using namespace Interface;
    using namespace LightBeam;
    using namespace Math;
    using namespace Rendering;

//-----------------------------------------------------------------------------
View::View(QWidget *ipParent, Broker *ipBroker) : QWidget(ipParent),
    mBrokerRef(*ipBroker)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

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
    Camera &cRef = b.getSceneRef().getCameraRef();
    
    cRef.set(Vector3(0, 12, 100),
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
void View::keyPressEvent(QKeyEvent *ipE)
{
    Keyboard &k = getBroker().getKeyboard();
    k.setKeyPressed( (Key)ipE->key() );
}

//-----------------------------------------------------------------------------
void View::keyReleaseEvent(QKeyEvent *ipE)
{
    Keyboard &k = getBroker().getKeyboard();
    k.setKeyReleased( (Key)ipE->key() );
}

//-----------------------------------------------------------------------------
void View::mousePressEvent(QMouseEvent *ipE)
{
    Mouse& m = getBroker().getMouse();
    
    m.setButtonPressed(ipE->x(), ipE->y(), Mouse::bLeft);
}

//-----------------------------------------------------------------------------
void View::mouseMoveEvent(QMouseEvent *ipE)
{
    Mouse& m = getBroker().getMouse();
    m.setPosition(ipE->x(), ipE->y());
}

//-----------------------------------------------------------------------------
void View::mouseReleaseEvent(QMouseEvent *ipE)
{
    Mouse& m = getBroker().getMouse();
    m.setButtonReleased(ipE->x(), ipE->y(), Mouse::bLeft);
}

//-----------------------------------------------------------------------------
void View::resizeEvent(QResizeEvent *ipE)
{
    const int w = ipE->size().width();
    const int h = ipE->size().height();
    Broker &b = getBroker();
    
    // resize the projection and viewport of the camera
    Camera &cRef = b.getSceneRef().getCameraRef();
    Viewport vp;
    vp.set(w, h);
    cRef.setViewport(vp);
    
    //init final image
    Image &im = b.getFinalImage();
    im.set(vp.getWidth(), vp.getHeight(), iifRgbaUint8);
    
    viewChanged();
}

//-----------------------------------------------------------------------------
void View::updateUi()
{
    Broker &b = getBroker();
    Image &im = b.getFinalImage();
    if(im.isValid())
    {
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