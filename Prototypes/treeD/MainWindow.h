/*
 */

#pragma once

#include <QMainWindow>
#include <QOpenGLWidget>


class Viewer : public QOpenGLWidget
{
	friend class MainDialog;

public:
	Viewer(QWidget*);
	~Viewer();

private:
	virtual void initializeGL() override;
	virtual void paintGL() override;
    virtual void resizeGL(int, int) override;
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

