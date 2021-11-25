/*
 */

#pragma once


#include "GlPrototyping/Viewer.h"
#include <QDockWidget>
#include <QMainWindow>
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
	virtual void initializeGL() override final;
	virtual void loadShaders() override final;
	virtual void paintGL() override final;
    
	MainWindow *mpMainWindow;

	Realisim::Core::Timer mTimer;
	std::vector<Realisim::Rendering::VertexArrayObject*> mModelVaoPtrs; //owned
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
};

