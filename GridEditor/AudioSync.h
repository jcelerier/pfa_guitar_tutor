#ifndef AUDIOSYNC_H
#define AUDIOSYNC_H


#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QPushButton>
#include <QTime>

#include "TimeEdit.h"
#include "TempoEdit.h"


#define TIMER_BEGINNING 0
#define TIMER_END       1
#define TIMER_BAR       2
#define TIMER_PLAY		3
class TimeEdit;

/**
 * @brief Interface de synchronisation audio
 */
class AudioSync : public QWidget
{
		Q_OBJECT

	public:
		AudioSync(QWidget* parent = 0);
		~AudioSync();
		void activeButtons(bool active);

		void setBarTimer(const QTime);
		void setBeginningTimer(const QTime);
		void setEndTimer(const QTime);
		int getBeginning();
		int getBar();
		int getEnd();

		void checkTimes();
		void updateTempo();

	public slots:
		void sendData();
		void tempoClicked();
		void recvTimer(int, QTime);
		void setTimeSignature(int);

	private slots:
		void emitSignalTimer();
		void beginningChanged(QTime);
		void barChanged(QTime);
		void endChanged(QTime);
		void tempoChanged(int);

	signals:
		/**
		* @brief refreshTimer est émis quand on clique sur un bouton pour set début, bar, ou fin
		*/
		void refreshTimer(int);

		/**
		* @brief sendTimers est utilisé pour envoyer les timers à audioWindow puis à la grille
		*/
		void sendTimers(QTime, QTime, QTime);

		/**
		* @brief sendTimer est utilisé pour envoyer un timer à audioWindow (pour mettre à jour la waveform)
		*/
		void sendTimer(int, QTime);

	private:
		/**
		* @brief m_layout Layout qui contient l'affichage
		*/
		QGridLayout* m_layout;

		/**
		* @brief m_beginning permet d'ajuster le début
		*/
		TimeEdit *m_beginning;

		/**
		* @brief m_end permet d'ajuster la fin du morceau
		*/
		TimeEdit *m_end;

		/**
		* @brief m_bar permet d'ajuster la fin de la première mesure
		*/
		TimeEdit *m_bar;

		/**
		* @brief m_bbeginning Bouton qui permet de fixer le début
		*/
		QToolButton* m_bbeginning;

		/**
		* @brief m_bend Bouton qui permet de fixer la fin du morceau
		*/
		QToolButton *m_bend;

		/**
		* @brief m_bbar Bouton qui permet de fixer la fin de la première mesure
		*/
		QToolButton *m_bbar;

		/**
		* @brief m_lbeginning Label du début
		*/
		QLabel *m_lbeginning;

		/**
		* @brief m_lend Label de la fin du morceau
		*/
		QLabel *m_lend;

		/**
		* @brief m_lbar Label de la fin de la première mesure
		*/
		QLabel *m_lbar;

		/**
		* @brief m_lbpm Label du tempo
		*/
		QLabel *m_lbpm;


		/**
		* @brief m_bpm permet d'ajuster le tempo
		*/
		TempoEdit *m_bpm;

		/**
		* @brief m_sendButton permet d'envoyer les informations à la grille
		*/
		QPushButton *m_sendButton;

		/**
		* @brief m_tempBarTime stocke temporairement le tempo (utilisé pour éviter les feedbacks infini entre l'ajustement de bar et de bpm
		*/
		QTime m_tempBarTime;

		/**
		* @brief m_timeSignature stocke la signature temporelle (pour calculs)
		*/
		unsigned int m_timeSignature;

};

#endif // AUDIOSYNC_H
