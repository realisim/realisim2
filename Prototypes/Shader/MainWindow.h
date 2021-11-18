/*
 */

#pragma once


#include "GlPrototyping/Viewer.h"
#include <QDockWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QTimerEvent>
#include "Rendering/Gpu/Shader.h"
#include "Core/Statistics.h"
#include "Core/Timer.h"

class MainWindow;

//-------------------------------------------------------------------
class Viewer3d : public Viewer
{
	friend class MainWindow;

public:
	Viewer3d(QWidget*, MainWindow*);
	virtual ~Viewer3d();
    
protected:
	virtual void initializeGL() override;
	virtual void paintGL() override;
    
	MainWindow *mpMainWindow;
    Realisim::Rendering::Shader mShader;
};


//----------------------------------------------------
class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();
	~MainWindow() = default;

	void updateUi();

protected:    
    virtual void timerEvent(QTimerEvent*) override;

	//--- ui
	Viewer3d* mpViewer;

	//--- data
    int mTimerId;
    Realisim::Core::Statistics mUpdateStats;
	Realisim::Core::Timer mTimerToUpdateGl;
	bool mShaderSourceInitialized;
};

