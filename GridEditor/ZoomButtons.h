#ifndef ZOOMBUTTONS_H
#define ZOOMBUTTONS_H

#include <QtWidgets/QWidget>
#include <QString>

/**
 * @brief Boutons de zoom en dessous de l'affichage de la waveform
 */
class ZoomButtons : public QWidget
{
		Q_OBJECT

	public:
		explicit ZoomButtons(QWidget *parent = 0);
};

#endif // ZOOMBUTTONS_H
