#ifndef AUDIOCONFIGURATION_H
#define AUDIOCONFIGURATION_H

#include <QDialog>
#include <QList>
#include <string>

namespace Ui {
class AudioConfiguration;
}

class AudioConfiguration : public QDialog
{
		Q_OBJECT
		
	public:
		explicit AudioConfiguration(QWidget *parent = 0);
		void init(void);
		void sendOutputVect(std::vector<std::string> &outputVect );
		void sendInputVect(std::vector<std::string> &inputVect );
		~AudioConfiguration();

		
	private:
		Ui::AudioConfiguration *ui;

		QString chosenInput;
		QString chosenOutput;

		QString tmpInput;
		QString tmpOutput;

	public slots:
		void swapShowDialogModeless();
		void setInput(QString input);
		void setOutput(QString output);
		void acceptData();

};

#endif // AUDIOCONFIGURATION_H
