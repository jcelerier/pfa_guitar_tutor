#include "OpenGLWidget.h"
#include <iostream>

#include <QApplication>
#include <QTimer>
#include <QKeyEvent>
#include <unistd.h>
#include <GL/glu.h>

OpenGLWidget::OpenGLWidget( int timerInterval, QWidget *parent ) : QGLWidget( parent )
{
	if( timerInterval == 0 )
		m_timer = 0;
	else
	{
		m_timer = new QTimer( this );
		connect( m_timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()) );
		m_timer->start( timerInterval );
	}
}

void OpenGLWidget::setChordToDisplay(const std::vector<std::string> & chords)
{
	for (unsigned int i = 0; i < chords.size(); ++i) {
		m_chordToPosition[chords[i]] = i;
	}
}

void OpenGLWidget::keyPressEvent( QKeyEvent *e )
{
	switch( e->key() )
	{
	case Qt::Key_Escape:
		close();
	}
}

void OpenGLWidget::initializeGL()
{
	glShadeModel(GL_SMOOTH);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void OpenGLWidget::resizeGL( int width, int height )
{
	height = height?height:1;

	glViewport( 0, 0, (GLint)width, (GLint)height );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void OpenGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//glTranslatef(-1.5f,0.0f,-6.0f);
	glTranslatef(0.0f,0.0f,-3.0f);
	glTranslatef(0.0f,0.0f,-3.0f);

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f,0.0f,0.0f);
	glVertex3f( 0.0f, 1.0f, 0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(-1.0f,-1.0f, 0.0f);
	glColor3f(0.0f,0.0f,1.0f);
	glVertex3f( 1.0f,-1.0f, 0.0f);
	glEnd();

	//glTranslatef(3.0f,0.0f,0.0f);

//	glColor3f(0.5f,0.5f,1.0f);
//	glColor3f(rand()/(float)RAND_MAX,rand()/(float)RAND_MAX,rand()/(float)RAND_MAX);
//	glBegin(GL_QUADS);
//	glVertex3f(-1.0f, 1.0f, 0.0f);
//	glVertex3f( 1.0f, 1.0f, 0.0f);
//	glVertex3f( 1.0f,-1.0f, 0.0f);
//	glVertex3f(-1.0f,-1.0f, 0.0f);
//	glEnd();
}

void OpenGLWidget::timeOut()
{
}

void OpenGLWidget::timeOutSlot()
{
	//std::cout << "gleu" << std::endl;
	timeOut();
}
