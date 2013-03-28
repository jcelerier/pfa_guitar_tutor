#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QtGui>
#include <QString>
#include <portaudio.h>

#define ORIGINAL_WIDTH 1920
#define ORIGINAL_HEIGHT 1080

#define PAUSE_TO_SAME_TIME 0
#define PAUSE_TO_LAST_CHORD 1
#define PAUSE_TO_LAST_PART 2

#define PERCENT_TO_VALIDATE_EASY 30
#define PERCENT_TO_VALIDATE_MEDIUM 50
#define PERCENT_TO_VALIDATE_HARD 70

/**
 * @brief Classe gérant la configuration d'une partie (difficulté, reprises,...)
 */
class Configuration
{
	public:
		Configuration();

		void setSongName(QString songName);
		QString getSongName();

		void setAudioInput(QString audioInput);
		QString getAudioInput();

		void setAudioOutput(QString audioOutput);
        QString getAudioOutput();

        int getPauseSetting() const;
		void setPauseSetting(int);

        int getDifficulty() const;
        void setDifficulty(int d);

        bool getLoopSetting() const;
		void setLoopSetting(bool);

        QString getSongDirectory() const;
        void setSongDirectory(QString);

		static int const framesPerSec=60;
		static int const originalWidth=ORIGINAL_WIDTH;
		static int const originalHeight=ORIGINAL_HEIGHT;

		static float getQuotient();
		static QRect getWindowSize();
		static void setWindowSize(QRect qr);
		static void setWindowSize(int w, int h);

private:
		QString m_currentSongName;
		QString m_audioInput;
		QString m_audioOutput;

		PaDeviceIndex m_inputIndex, m_outputIndex;
        static int m_height;        // Taille de l'ecran
        static int m_width;
        static float m_quotient;    // Rapport entre taille de la scene et la taille d'affichage (horizontal)

        int m_pauseSetting;
        int m_percentToGoToNextPart;   // Difficulté

        bool m_loopOnFailure;
        QString m_songDirectory;              // Repertoire par defaut ou chercher les chansons

};

#endif // CONFIGURATION_H
