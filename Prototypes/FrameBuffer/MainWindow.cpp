/**/
// openglHeaders must be inluded firat
#include "Rendering/Gpu/OpenGlHeaders.h"

#include "Core/FileInfo.h"
#include "Core/Path.h"
#include "MainWindow.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QOpenGLContext>
#include <sstream>

using namespace Realisim;
using namespace Core;
using namespace Math;
using namespace Rendering;
using namespace std;

//----------------------------------------------
Viewer3d::Viewer3d(QWidget* ipParent, MainWindow* ipMw) :
    mpMainWindow(ipMw),
    Viewer(ipParent),
    mFbo(1, 1, 1)
{
	setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);    

    mTime.start();

    Projection proj;
    proj.setProjection(-1, 1, -1, 1, -10, 10, Projection::Type::tOrthogonal);
    mFullScreen2dCam.setProjection(proj);
    mFullScreen2dCam.set(Vector3(0.0, 0.0, 1.0), Vector3(0.0, 0.0, 0.0), Vector3(0.0, 1.0, 0.0));
}

Viewer3d::~Viewer3d()
{}

//-----------------------------------------------------------------------------
void Viewer3d::blitFboAttachment(FrameBufferAttachementType iAttachment)
{
    glClearColor(1.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mFbo.getAttachement(iAttachment).getId());

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();              /* saved for posterity */
    glLoadIdentity();

    glOrtho(-1., 1., -1., 1., -1., 1.);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();              /* saved for posterity */
    glLoadIdentity();

    glBegin(GL_QUADS);

    glColor4ub(255, 255, 255, 255);

    glTexCoord2f(0.0, 0.0);
    glVertex2f(-1.0, -1.0);
    glTexCoord2f(1.0, 0.0);
    glVertex2f(1.0, -1.0);
    glTexCoord2f(1.0, 1.0);
    glVertex2f(1.0, 1.0);
    glTexCoord2f(0.0, 1.0);
    glVertex2f(-1.0, 1.0);
    glEnd();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

//-----------------------------------------------------------------------------
void Viewer3d::initializeGL()
{
	Viewer::initializeGL();

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glEnable(GL_FRAMEBUFFER_SRGB);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //init the main FBO
    Texture2d* pColorTexture = mFbo.createAttachementTexture(
        Rendering::tifRgb8, 
        Rendering::tfRgb, 
        Rendering::dtUnsignedByte,
        Rendering::TextureFilter::tfLinear, 
        Rendering::TextureWrapMode::twmClampToBorder, 
        Rendering::TextureWrapMode::twmClampToBorder);
    Texture2d* pToBloomTexture = mFbo.createAttachementTexture(
        Rendering::tifRgb8,
        Rendering::tfRgb,
        Rendering::dtUnsignedByte,
        Rendering::TextureFilter::tfLinear,
        Rendering::TextureWrapMode::twmClampToBorder,
        Rendering::TextureWrapMode::twmClampToBorder);
    Texture2d* pBloomTexture = mFbo.createAttachementTexture(
        Rendering::tifRgb8,
        Rendering::tfRgb,
        Rendering::dtUnsignedByte,
        Rendering::TextureFilter::tfLinear,
        Rendering::TextureWrapMode::twmClampToBorder,
        Rendering::TextureWrapMode::twmClampToBorder);
    Texture2d* pCompositing = mFbo.createAttachementTexture(
        Rendering::tifRgb8,
        Rendering::tfRgb,
        Rendering::dtUnsignedByte,
        Rendering::TextureFilter::tfLinear,
        Rendering::TextureWrapMode::twmClampToBorder,
        Rendering::TextureWrapMode::twmClampToBorder);
    Texture2d *pDepthTexture = mFbo.createAttachementTexture(
        Rendering::TextureInternalFormat::tifDepthInteger24StencilInteger8,
        Rendering::TextureFormat::tfDepthStencil,
        Rendering::DataType::dtUnsignedInteger24_8,
        Rendering::TextureFilter::tfLinear,
        Rendering::TextureWrapMode::twmClampToBorder,
        Rendering::TextureWrapMode::twmClampToBorder);

    mFbo.addAttachement(Rendering::fbaColor0, pColorTexture);
    mFbo.addAttachement(Rendering::fbaColor1, pToBloomTexture);
    mFbo.addAttachement(Rendering::fbaColor2, pBloomTexture);
    mFbo.addAttachement(Rendering::fbaColor3, pCompositing);
    mFbo.addAttachement(Rendering::fbaDepthOnly, pDepthTexture);
    mFbo.bake();

    // init bloom shader
    FileInfo appPath(Path::getApplicationFilePath());
    string assetsPath = Path::join(appPath.getAbsolutePath(), "../assets");

    mBloomShader.setName("bloomShader");
    mBloomShader.addSourceFromFile(stVertex, Path::join(assetsPath, "bloom.vert"));
    mBloomShader.addSourceFromFile(stFragment, Path::join(assetsPath, "bloom.frag"));
    mBloomShader.compile();
    mBloomShader.link();

    if (mBloomShader.hasErrors())
    {
        cout << mBloomShader.getAndClearLastErrors();
    }

    //--- init compositing shader
    assetsPath = Path::join(appPath.getAbsolutePath(), "../assets");

    mCompositingShader.setName("compositingShader");
    mCompositingShader.addSourceFromFile(stVertex, Path::join(assetsPath, "compositing.vert"));
    mCompositingShader.addSourceFromFile(stFragment, Path::join(assetsPath, "compositing.frag"));
    mCompositingShader.compile();
    mCompositingShader.link();

    if (mCompositingShader.hasErrors())
    {
        cout << mCompositingShader.getAndClearLastErrors();
    }


    //init fullscreen quad
    BufferObject *pVboVertex, *pVboTex, *pVboIndices;
    pVboVertex = new BufferObject();
    pVboTex = new BufferObject();
    pVboIndices = new BufferObject();

    float vertices[8] = {
        -1.0, -1.0,
         1.0, -1.0,
         1.0,  1.0,
        -1.0,  1.0 };
    BufferObjectDataPlain* pVertexDataStructure = new BufferObjectDataPlain();
    pVertexDataStructure->addAttribute(Rendering::dtFloat, 2, BufferObject::lliVertex);
    pVboVertex->setDataStructure(pVertexDataStructure);
    pVboVertex->assignData(BufferBindingTarget::bbtArrayBuffer, BufferDataUsage::bduStaticDraw, 8, &vertices[0]);

    float texCoords[8] = {
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0 };
    BufferObjectDataPlain* pTexDataStructure = new BufferObjectDataPlain();
    pTexDataStructure->addAttribute(Rendering::dtFloat, 2, BufferObject::lliTexture);
    pVboTex->setDataStructure(pTexDataStructure);
    pVboTex->assignData(BufferBindingTarget::bbtArrayBuffer, BufferDataUsage::bduStaticDraw, 8, &texCoords[0]);

    uint16_t indices[6] = {
        0, 1, 2,
        0, 2, 3 };
    BufferObjectDataPlain* pIndicesDataStructure = new BufferObjectDataPlain();
    pIndicesDataStructure->addAttribute(Rendering::dtUnsignedShort, 1, 0);
    pVboIndices->setDataStructure(pIndicesDataStructure);
    pVboIndices->assignData(BufferBindingTarget::bbtElementArrayBuffer, BufferDataUsage::bduStaticDraw, 6, &indices[0]);

    mFullScreenQuad.addAndTakeOwnership(pVboVertex);
    mFullScreenQuad.addAndTakeOwnership(pVboTex);
    mFullScreenQuad.addAndTakeOwnership(pVboIndices);
    mFullScreenQuad.bake(DrawMode::dmTriangles);

	mpMainWindow->updateUi();
}

//-----------------------------------------------------------------------------
void Viewer3d::paintGL()
{
    
    Matrix4 modelMatrix(Vector3(0.0, 0.0, 0.0));
    
    //--- draw scene off screen
    mFbo.push();
    mFbo.drawTo({ Rendering::fbaColor0, Rendering::fbaColor1 });
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mFbo.drawTo(Rendering::fbaColor0);

    glUseProgram(mShadedMaterialShader.getProgramId());

    mShadedMaterialShader.setUniform("uProjectionMatrix", mCamera.getProjectionMatrix());
    mShadedMaterialShader.setUniform("uViewMatrix", mCamera.getViewMatrix());
    mShadedMaterialShader.setUniform("uModelMatrix", modelMatrix);
    mShadedMaterialShader.setUniform("uApplyLighting", true);
    mShadedMaterialShader.setUniform("uLightPosition", Math::Vector3(-1, 1, 1));

    //draw axis
    mShadedMaterialShader.setUniform("uApplyLighting", false);
    mPainter.drawAxis();

    mShadedMaterialShader.setUniform("uApplyLighting", true);
    mPainter.drawCube();

        
    //--- draw things to be bloomed in fbaColor1    
    mFbo.drawTo({ Rendering::fbaColor0, Rendering::fbaColor1 });
    modelMatrix.setAsTranslation( Vector3(0.0, 0.0, 20.0) );
    mShadedMaterialShader.setUniform("uModelMatrix", modelMatrix);
    mPainter.drawCube();


    //--- apply bloom
    mFbo.drawTo(Rendering::fbaColor2);
    //glClearColor(0.f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glUseProgram(mBloomShader.getProgramId());
    // draw full screen quad with fbaColor1 texteture
    mBloomShader.setUniform("uProjectionMatrix", mFullScreen2dCam.getProjectionMatrix());
    mBloomShader.setUniform("uViewMatrix", mFullScreen2dCam.getViewMatrix());
    mBloomShader.setUniform("uSamplerInput", 0);
    mBloomShader.setUniform("uBloomFactor", fabs((float)sin(mTime.elapsed() * 2.0)));

    const Texture2d &toBloom = mFbo.getAttachement(Rendering::fbaColor1);
    toBloom.bind(0);
    mFullScreenQuad.draw();
    toBloom.unbind();
    
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);


    //-- compose final image to screen
    mFbo.drawTo(Rendering::fbaColor3);
    //glClearColor(0.f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(mCompositingShader.getProgramId());
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // draw full screen quad with fbaColor1 texteture
    mCompositingShader.setUniform("uProjectionMatrix", mFullScreen2dCam.getProjectionMatrix());
    mCompositingShader.setUniform("uViewMatrix", mFullScreen2dCam.getViewMatrix());
    mCompositingShader.setUniform("uOriginal", 0);
    mCompositingShader.setUniform("uBloomed", 1);


    const Texture2d& originalInput = mFbo.getAttachement(Rendering::fbaColor0);
    const Texture2d& bloomed = mFbo.getAttachement(Rendering::fbaColor2);
    originalInput.bind(0);
    bloomed.bind(1);
    mFullScreenQuad.draw();
    bloomed.unbind();
    originalInput.unbind();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glUseProgram(0);
    mFbo.pop();
    

    


    //// blit to screen
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mFbo.getId());
    glReadBuffer(toGlFrameBufferAttachement(Rendering::fbaColor3));
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo.getDefaultDrawBufferId());
    glBlitFramebuffer(0, 0, mFbo.getWidth(), mFbo.getHeight(), 0, 0, width(), height(),
        GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}


//-----------------------------------------------------------------------------
void Viewer3d::resizeGL(int iW, int iH)
{
    Viewer::resizeGL(iW, iH);
    Rendering::FrameBufferObject::setDefaultFrameBufferViewport(Vector2i(0, 0), Vector2i(iW, iH));
    Rendering::FrameBufferObject::initializeDefaultFrameBufferId();

    mFbo.resize(iW, iH);
}

//-----------------------------------------------------------------------------
//--- MainWindow
//-----------------------------------------------------------------------------
MainWindow::MainWindow() : QMainWindow(),
mpViewer(0),
mTimerId(0),
mShaderSourceInitialized(false)
{
	resize(800, 600);

	QWidget *pCentralWidget = new QWidget(this);
	setCentralWidget(pCentralWidget);

	QVBoxLayout* pLyt = new QVBoxLayout(pCentralWidget);
	pLyt->setMargin(5);
	{
		mpViewer = new Viewer3d(pCentralWidget, this);

		pLyt->addWidget(mpViewer);
	}
   

    mTimerId = startTimer(0);
    mTimerToUpdateGl.start();
	updateUi();
}

//-----------------------------------------------------------------------------
void MainWindow::timerEvent(QTimerEvent *ipE)
{
    if(ipE->timerId() == mTimerId)
    {
        mpViewer->handleUserInput();
        
        const double e = mTimerToUpdateGl.elapsed();
        mUpdateStats.add(e);
        if(e + mUpdateStats.average() >= 1/60.0 )
        {
            mpViewer->update();
            mTimerToUpdateGl.start();
            mUpdateStats.clear();
        }
    }
}

//-----------------------------------------------------------------------------
void MainWindow::updateUi()
{
}