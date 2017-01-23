/*
 */

#ifndef MainDialog_hh
#define MainDialog_hh

#include <QKeyEvent>
#include <QColor>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <vector>


class Viewer : public QOpenGLWidget
{
	friend class MainDialog;

public:
	Viewer(QWidget*);
	~Viewer();

private:
	virtual void initializeGL() override;
	virtual void paintGL() override;
};


class MainDialog : public QMainWindow
{
	Q_OBJECT
public:
	MainDialog();
	~MainDialog() {};

protected slots:
    void testClicked();

protected:
	void updateUi();

	//--- ui
	Viewer* mpViewer;

	//--- data
    
};

#endif
