
#include "Broker.h"
#include "Core/Timer.h"
#include "Geometry/Rectangle.h"
#include "Math/CommonMath.h"
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
void View::mousePressEvent(QMouseEvent *ipE)
{
    mMouse.setButtonPressed(Mouse::bLeft);
    mMouse.setPosition(ipE->x(), ipE->y());
    
    emit viewChanged();
    
    updateUi();
}

//-----------------------------------------------------------------------------
void View::mouseMoveEvent(QMouseEvent *ipE)
{
    mMouse.setPosition(ipE->x(), ipE->y());
    
    if(mMouse.isButtonPressed(Mouse::bLeft))
    {
        Broker &b = getBroker();
        Camera &c = b.getCamera();
        
        const double f = degreesToRadians(1.0);
        const Vector2 d = mMouse.getAndClearDelta();
        
        c.rotate(f * d.x(),
                 Vector3(0.0, 1.0, 0.0),
                 c.getPosition());
        
        c.rotate(f * d.y(),
            c.getLateralVector(),
            c.getPosition() );
    }
    
    emit viewChanged();
    
    updateUi();
}

//-----------------------------------------------------------------------------
void View::mouseReleaseEvent(QMouseEvent *ipE)
{
    mMouse.setButtonReleased(Mouse::bLeft);
    mMouse.setPosition(ipE->x(), ipE->y());
}

//-----------------------------------------------------------------------------
void View::reconstructImage()
{
Core::Timer _t;

    Broker &b = getBroker();
    ImageCells &cells = b.getImageCells();
    
    // init final image
    Image &im = b.getFinalImage();
    
    // reconstruct image from cells
    for(int cellY = 0; cellY < cells.getHeightInCells(); ++cellY)
        for(int cellX = 0; cellX < cells.getWidthInCells(); ++cellX)
        {
            const Vector2i cellIndex(cellX, cellY);
            Rectangle r = cells.getCellCoverage(cellIndex);
            const Vector2 bl = r.getBottomLeft();
            const Vector2 tr = r.getTopRight();
            
            Color c;
            for(int y = bl.y(); y < tr.y(); ++y)
                for(int x = bl.x(); x < tr.x(); ++x)
                {
                    c = cells.getCellColor(cellIndex);
                    im.setPixelColor(Vector2i(x,y), c);
                }
        }
        
printf("reconstructImage %f(s)\n", _t.elapsed());
}

//-----------------------------------------------------------------------------
void View::resizeEvent(QResizeEvent *ipE)
{
    const int w = ipE->size().width();
    const int h = ipE->size().height();
    Broker &b = getBroker();
    
    // resize the projection and viewport of the camera
    Camera &cRef = b.getCamera();
    Viewport vp;
    vp.set(w, h);
    cRef.setViewport(vp);
    
    //init final image
    Image &im = b.getFinalImage();
    im.set(vp.getWidth()-2, vp.getHeight()-2, iifRgbaUint8);
    
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