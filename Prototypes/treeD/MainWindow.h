/*
 */

#pragma once

#include "3d/Camera.h"
#include <QMainWindow>
#include <QMouseEvent>
#include <QOpenGLWidget>


class Viewer : public QOpenGLWidget
{
	friend class MainDialog;

public:
	Viewer(QWidget*);
	~Viewer();

    Realisim::TreeD::Camera camera() const;
    void setCamera(Realisim::TreeD::Camera&);
    
private:
    void drawCube();
	virtual void initializeGL() override;
    virtual void mouseMoveEvent(QMouseEvent*) override;
	virtual void paintGL() override;
    virtual void resizeGL(int, int) override;
    
    Realisim::TreeD::Camera mCamera;
};

//----------------------------------------------------
class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();
	~MainWindow() = default;

protected slots:

protected:
	void updateUi();

	//--- ui
	Viewer* mpViewer;

	//--- data
    
};

