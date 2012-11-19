#ifndef AUDIOCONFIGURATION_H
#define AUDIOCONFIGURATION_H

#include <QDialog>
#include <QList>
#include <string>
#include "ScoreManager.h"
#include "MusicManager.h"

namespace Ui {
class AudioConfiguration;
}

class AudioConfiguration : public QDialog
{
		Q_OBJECT
		
	public:
		explicit AudioConfiguration(QWidget *parent = 0);
		void init(ScoreManager * scoreManager);
		~AudioConfiguration();

		
	private:
		Ui::AudioConfiguration *ui;

		std::string chosenInput;
		std::string chosenOutput;


	public slots:
		void swapShowDialogModeless()
		{
			// Si elle est deja affichee, on cache la boite de dialogue sinon on l'affiche

			setVisible(!isVisible());
		}

};

#endif // AUDIOCONFIGURATION_H
