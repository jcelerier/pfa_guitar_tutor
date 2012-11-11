#ifndef NEHEWIDGET_H
#define NEHEWIDGET_H

#include <map>
#include <string>
#include <vector>

#include <QGLWidget>

class QTimer;
class QGLWidget;

class OpenGLWidget : public QGLWidget
{
	Q_OBJECT

	static const unsigned int MILLISEC_PER_PIXEL = 10;
	
public:
	OpenGLWidget( int timerInterval=0, QWidget *parent=0 );
	void setChordToDisplay(const std::vector<std::string> & chords);

protected:
	virtual void initializeGL();
	virtual void resizeGL( int width, int height );
	virtual void paintGL();
	
	virtual void keyPressEvent( QKeyEvent *e );
	
	virtual void timeOut();
	
protected slots:
	virtual void timeOutSlot();
	
private:
	QTimer *m_timer;

	std::map<std::string, unsigned int> m_chordToPosition;
};

#endif
