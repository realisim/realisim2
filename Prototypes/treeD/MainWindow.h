/*
 */

#pragma once

#include "3d/Camera.h"
#include <QMainWindow>
#include <QMouseEvent>
#include <QRadioButton>
#include <QOpenGLWidget>


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
	virtual void initializeGL() override;
    virtual void keyPressEvent(QKeyEvent*) override;
    virtual void mouseMoveEvent(QMouseEvent*) override;
	virtual void paintGL() override;
    virtual void resizeGL(int, int) override;
    
    Realisim::TreeD::Camera mCamera;
    CameraMode mCameraMode;
    
    //--- mouse info
    int mMouseX;
    int mMouseY;
};

//----------------------------------------------------
class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();
	~MainWindow() = default;

protected slots:
    void cameraRotateAboutClicked();
    void cameraFreeClicked();

protected:
    
	void updateUi();

	//--- ui
	Viewer* mpViewer;
    QRadioButton *mpCameraRotateAbout;
    QRadioButton *mpCameraFree;

	//--- data
    
};

