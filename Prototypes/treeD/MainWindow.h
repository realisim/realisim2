/*
 */

#pragma once

#include "3d/Camera.h"
#include <map>
#include <QLabel>
#include <QMainWindow>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QRadioButton>
#include <QSlider>
#include <QTimerEvent>


class Viewer : public QOpenGLWidget
{
	friend class MainWindow;

public:
	Viewer(QWidget*);
	~Viewer();

    Realisim::TreeD::Camera camera() const;
    void setCamera(Realisim::TreeD::Camera&);
    
protected:
    enum CameraMode{cmRotateAround, cmFree};
    
    void drawCube();
    void handleUserInput();
	virtual void initializeGL() override;
    virtual void keyPressEvent(QKeyEvent*) override;
    virtual void keyReleaseEvent(QKeyEvent*) override;
    virtual void mousePressEvent(QMouseEvent*) override;
    virtual void mouseMoveEvent(QMouseEvent*) override;
	virtual void paintGL() override;
    virtual void resizeGL(int, int) override;
    
    Realisim::TreeD::Camera mCamera;
    CameraMode mCameraMode;
    
    //--- mouse info
    std::map<int, bool> mKeyboard;
    int mMouseX;
    int mMouseY;
    int mMouseDeltaX;
    int mMouseDeltaY;
    bool mMouseButtonPressed;
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
};

