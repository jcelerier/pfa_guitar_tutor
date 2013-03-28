#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QImage>
#include <QPoint>
#include <QColor>
#include <QTime>

#include "MusicPlayer.h"

/**
 * @brief Affichage d'une forme d'onde
 */
class Waveform : public QLabel
{
			Q_OBJECT

	public:
		explicit Waveform(QWidget *parent, int w, int h);
		~Waveform();
		void update();
		void display();
		void initImage();

		void setWidth(unsigned int w);
		void setHeight(int h);

		int getWidth();
		int* getSpectrum();

		int getSampleBegin();
		int getSampleBar();
		int getSampleEnd();

		void simpleDrawColumn(int col, int value, QPen* pen);
		void drawColumn(int col, int beg, int end, int smp_begin, int smp_end, int pos_begin, int pos_end);


	public slots:
		void setTimer(int, QTime);
		void activate();
		void setPlayerTimer(QTime);

	protected:
		void mouseMoveEvent(QMouseEvent * event);
		void mousePressEvent(QMouseEvent * event);

	private:
		/**
		 * @brief m_parent Parent du widget
		 */
		QWidget* m_parent;

		/**
		 * @brief m_empty Utilisé pour dire si un morceau est chargé dans la waveform
		 */
		bool m_empty;

		/**
		 * @brief m_pixmap Image qui contient la waveform
		 */
		QPixmap* m_pixmap;

		/**
		 * @brief m_mainPen Utilisé pour les traits verts
		 */
		QPen* m_mainPen;

		/**
		 * @brief m_darkPen Utilisé pour le fond noir
		 */
		QPen* m_darkPen;

		/**
		 * @brief m_delimiterPen Utilisé pour les traits blancs
		 */
		QPen* m_delimiterPen;

		/**
		 * @brief m_playerPen Utilisé pour le trait jaune
		 */
		QPen* m_playerPen;

		/**
		 * @brief m_painter Effectue le dessin
		 */
		QPainter* m_painter;

		/**
		 * @brief m_spectrum Tableau d'entiers qui contient le spectre
		 */
		int* m_spectrum;

		/**
		 * @brief m_width Longueur en pixels
		 */
		unsigned int m_width;

		/**
		 * @brief m_height Hauteur en pixels
		 */
		unsigned int m_height;

		/**
		 * @brief m_reductionFactor Sert à normaliser la waveform pour que ça ne sature pas
		 *
		 * L'idéal serait de diviser par la valeur moyenne du morceau
		 */
		int m_reductionFactor;

		/**
		 * @brief m_oldMousePos Ancienne position de la souris
		 */
		QPoint m_oldMousePos;

		/**
		 * @brief m_clickPos Position du clic actuel
		 */
		QPoint m_clickPos;

		/**
		 * @brief m_beginTime Temps du début
		 */
		QTime m_beginTime;

		/**
		 * @brief m_endTime Temps de la fin
		 */
		QTime m_endTime;

		/**
		 * @brief m_barTime Temps de la mesure
		 */
		QTime m_barTime;
};

#endif // WAVEFORM_H
