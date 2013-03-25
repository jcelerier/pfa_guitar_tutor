#include "MyView.h"
#include "PlayerScene.h"
#include <QGLWidget>
MyView::MyView(QGraphicsScene * scene, QWidget * parent) :
	QGraphicsView(scene, parent)
{
	setViewport(new QGLWidget); //OPENGL BITCHES
    setSceneRect(0, 0, Configuration::originalWidth-4, Configuration::originalHeight-4);
	centerOn(Configuration::originalWidth/2, Configuration::originalHeight/2);

	scale(Configuration::getQuotient(), Configuration::getQuotient()); // Adaptation a l'ecran
	//fitInView(Configuration::getWindowSize()); // Resultats pitoyables

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setRenderHints(QPainter::Antialiasing);
	//setRenderHint(QPainter::Antialiasing | QPainter::SmoothPixmapTransform); // Smoothpixmap ne s'applique pas a tout

	setFixedSize(Configuration::getWindowSize().size()); // Fenetré
	//showFullScreen();                                  // Plein ecran
}

/**
 * @brief MyView::keyPressEvent
 * @param event Evènement
 *
 * Gestion des touches du clavier sur l'interface.
 */
void MyView::keyPressEvent(QKeyEvent* event)
{
	QGraphicsView::keyPressEvent(event);

	switch ( event->key() )
	{
	//Debug
	/*case Qt::Key_Up :
			scale(0.5,0.5);
			break;
		case Qt::Key_Down :
			scale(2,2);
			break;
		case Qt::Key_Right :
			scene()->advance();
			break;*/
	case Qt::Key_Space :
		((PlayerScene*)scene())->switchPlay();
		break;
	/*case Qt::Key_Tab : //Problème: le dico apparait en arrière plan
		((PlayerScene*)scene())->displayDictionary();
		break;*/
	case Qt::Key_Escape :
		((PlayerScene*)scene())->switchMenu();
		break;

	case Qt::Key_M:
		((PlayerScene*)scene())->switchMute();
		   break;


	}
	event->accept();
}
