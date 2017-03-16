/*
 */

#pragma once

#include "Rendering/Camera.h"
#include <map>
#include <QLabel>
#include <QMainWindow>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QRadioButton>
#include <QSlider>
#include <QTimerEvent>
#include "Utilities/Timer.h"
#include <vector>

//-------------------------------------------------------------------
struct Box
{
    Box();
    Box(const Box&) = default;
    Box& operator=(const Box&) = default;
    ~Box() = default;
    
    void add(Realisim::Math::Vector3);
    Realisim::Math::Vector3 center() const {return mMin + (mMax-mMin)/2.0;}
    double depth() const {return mMax.z() - mMin.z(); }
    double height() const {return mMax.y() - mMin.y(); }
    Realisim::Math::Vector3 point(int) const;
    double width() const {return mMax.x() - mMin.x(); }
    
    Realisim::Math::Vector3 mMin;
    Realisim::Math::Vector3 mMax;
    Realisim::Math::Matrix4 mTransfo;
};

//-------------------------------------------------------------------
class Viewer : public QOpenGLWidget
{
	friend class MainWindow;

public:
	Viewer(QWidget*);
	~Viewer();

    Realisim::Rendering::Camera camera() const;
    void setCamera(Realisim::Rendering::Camera&);
    
protected:
    enum CameraMode{cmRotateAround, cmFree};
    
    void drawCube();
    void handleUserInput();
	virtual void initializeGL() override;
    virtual void keyPressEvent(QKeyEvent*) override;
    virtual void keyReleaseEvent(QKeyEvent*) override;
    virtual void mouseMoveEvent(QMouseEvent*) override;
    virtual void mousePressEvent(QMouseEvent*) override;
    virtual void mouseReleaseEvent(QMouseEvent*) override;
	virtual void paintGL() override;
    virtual void resizeGL(int, int) override;
    
    Realisim::Rendering::Camera mCamera;
    CameraMode mCameraMode;
    std::vector<Box> mBoxes;
    int mSelectedBoxIndex;
    
    //--- mouse info
    std::map<int, bool> mKeyboard;
    int mMouseX;
    int mMouseY;
    int mMouseDeltaX;
    int mMouseDeltaY;
    bool mMouseLeftPressed;
    bool mMouseRightPressed;
};

//----------------------------------------------------
class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();
	~MainWindow() = default;

protected slots:
    void cameraProjOrthoClicked();
    void cameraProjPerspectiveClicked();
    void cameraRotateAboutClicked();
    void cameraFreeClicked();
    void zoomFactorChanged(int);

protected:
    
    virtual void timerEvent(QTimerEvent*) override;
	void updateUi();

	//--- ui
	Viewer* mpViewer;
    QRadioButton *mpCameraRotateAbout;
    QRadioButton *mpCameraFree;
    QRadioButton *mpOrthoProj;
    QRadioButton *mpPerspectiveProj;
    QSlider *mpZoomFactor;
    QLabel *mpZoomFactorLabel;

	//--- data
    int mTimerId;
    Realisim::Utilities::Timer mTimerToUpdateGl;
};

