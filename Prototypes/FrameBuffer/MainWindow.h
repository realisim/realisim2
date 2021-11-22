/*
 */

#pragma once


#include "GlPrototyping/Viewer.h"
#include <QDockWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QTimerEvent>
#include "Rendering/Gpu/FrameBufferObject.h"
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
	void blitFboAttachment(Realisim::Rendering::FrameBufferAttachementType iAttachment);
	virtual void initializeGL() override final;
	virtual void paintGL() override final;
	virtual void resizeGL(int, int) override final;
    
	MainWindow *mpMainWindow;

	Realisim::Core::Timer mTime;

	Realisim::Rendering::FrameBufferObject mFbo;
	Realisim::Rendering::Shader mBloomShader;
	Realisim::Rendering::Shader mCompositingShader;
	Realisim::Rendering::VertexArrayObject mFullScreenQuad;
	Realisim::Rendering::Camera mFullScreen2dCam;
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

