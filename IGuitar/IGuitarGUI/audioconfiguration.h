#ifndef AUDIOCONFIGURATION_H
#define AUDIOCONFIGURATION_H

#include <QtWidgets/QDialog>
#include <QList>
#include "Configuration.h"
#include <string>

namespace Ui {
class AudioConfiguration;
}

class AudioConfiguration : public QDialog
{
		Q_OBJECT
		
	public:
		explicit AudioConfiguration(Configuration &conf, QWidget *parent = 0);
		void init();
		void sendOutputVect(std::vector<std::string> &outputVect );
		void sendInputVect(std::vector<std::string> &inputVect );
		~AudioConfiguration();

	public slots:
		void swapShowDialogModeless();
		void setInput(QString input);
		void setOutput(QString output);
		void acceptData();

	private:
		Ui::AudioConfiguration *ui;

		Configuration& m_conf;

		QString m_tmpInput;
		QString m_tmpOutput;
};

#endif // AUDIOCONFIGURATION_H
