/*
 */

#ifndef MainDialog_hh
#define MainDialog_hh

#include <QKeyEvent>
#include <QGLwidget>
#include <QMainWindow>
#include <vector>


class Viewer : public QGLWidget
{
	friend class MainDialog;

public:
	Viewer(QWidget*);
	~Viewer();

private:
//	virtual void draw() override;
//	virtual void keyPressEvent(QKeyEvent*);
//	virtual void loadShaders();
//	virtual void initializeGL() override;
//	virtual void paintGL() override;
//	virtual void resizeGL(int, int) override;

//	realisim::treeD::Texture mTexture;
//	realisim::treeD::FrameBufferObject mFbo;
};


class MainDialog : public QMainWindow
{
	Q_OBJECT
public:
	MainDialog();
	~MainDialog() {};

protected slots:

protected:
	void updateUi();

	//--- ui
	Viewer* mpViewer;

	//--- data
};

#endif
