
#include "Painter.h"
#include <QMouseEvent>
#include <QOpenGLWidget>
#include "Rendering/Camera.h"
#include "Rendering/Gpu/Shader.h"
#include "Math/VectorI.h"

//-------------------------------------------------------------------
class Viewer : public QOpenGLWidget
{

public:
    Viewer(QWidget*);
    virtual ~Viewer();

    Realisim::Rendering::Camera getCamera() const;
	double getCameraMovementIncrement() const;
    Realisim::Math::Vector3 getWorldUpVector() const;
    void setCamera(Realisim::Rendering::Camera&);
	void setCameraMovementIncrement(double);
	void setWorldUpVector(const Realisim::Math::Vector3& iWorldUp);
    
protected:
    enum CameraMode{cmRotateAround, cmFree};
    
    void handleUserInput();
    virtual void initializeGL() override;
    virtual void keyPressEvent(QKeyEvent*) override;
    virtual void keyReleaseEvent(QKeyEvent*) override;
    virtual void mouseMoveEvent(QMouseEvent*) override;
    virtual void mousePressEvent(QMouseEvent*) override;
    virtual void mouseReleaseEvent(QMouseEvent*) override;
    virtual void paintGL() override;
    virtual void resizeGL(int, int) override;

    virtual void loadShaders();
    
    Realisim::Rendering::Camera mCamera;
    Realisim::Math::Vector3 mWorldUpVector;
	double mCameraMovementIncrement;
    CameraMode mCameraMode;
    Realisim::Painter mPainter;

    Realisim::Rendering::Shader mShadedMaterialShader;

    //--- mouse info
    std::map<int, bool> mKeyboard;
    Realisim::Math::Vector2i mMousePos;
    Realisim::Math::Vector2i mMouseDelta;
    Realisim::Math::Vector2i mMousePosWhenPressed;
    bool mMouseLeftPressed;
    bool mMouseRightPressed;
};