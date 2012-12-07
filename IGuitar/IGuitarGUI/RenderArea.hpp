#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QPainterPath>
#include <QBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QImage>
#include <QDir>
#include <vector>
#include <map>

#include "../../GuitarTutorAPI/ScoreManager.h"

#define BACKGROUND_UPLEFT_X 0.0
#define BACKGROUND_UPLEFT_Y 0.0
#define BACKGROUND_HORIZONTAL_LENGTH 50.0
#define BACKGROUND_VERTICAL_LENGTH 100.0

#define PRESENT_LINE_OFFSET 2.0
#define PRESENT_LINE_HEIGHT 0.5
#define PRESENT_LINE_COLOR_WITHOUT_VALIDATION Qt::darkGray
#define PRESENT_LINE_COLOR_WITH_VALIDATION Qt::white

#define SCORE_OFFSET 10.0
#define SCORE_NOTE_SIZE 5.0
#define SCORE_NOTE_FONT "Arial"
#define SCORE_NOTE_COLOR Qt::white
#define SCORE_NOTE_OFFSET_X 2.0
#define SCORE_NOTE_OFFSET_Y -2.0

#define BACKGROUND_COLOR1 Qt::cyan
#define BACKGROUND_COLOR2 Qt::red

#define NOTE_IN_CURRENT_PART_COLOR Qt::blue
#define NOTE_NOT_IN_CURRENT_PART_COLOR Qt::darkBlue
#define NOTE_VALIDATED_COLOR Qt::cyan

#define MIN_SCORE_BOUND -1000
#define MAX_SCORE_BOUND 5000

#define PART_FONT_SIZE 3.0
#define CURRENT_PART_STRING "Current Part"
#define NEXT_PART_STRING "Next Part"
#define PART_COLOR Qt::white
#define PART_OFFSET_X 2.0
#define PART_VERTICAL_LINE_SPACE 4.0
#define PART_HORIZONTAL_LINE_SPACE 25.0

#define CHORDS_IMAGES_UPPER_LEFT_X 0.0
#define CHORDS_IMAGES_UPPER_LEFT_Y 0.0
#define CHORD_IMAGE_HEIGHT 12.0
#define CHORD_IMAGE_WIDTH 10.0
#define CHORD_IMAGE_OFFSET 1.0
#define CHORDS_IMAGES_PER_LINE 4.0


class RenderArea : public QWidget
{
	Q_OBJECT

	public:
		RenderArea(QWidget *parent = 0);

		QSize minimumSizeHint() const;
		QSize sizeHint() const;

		void drawScore(const prioritizedScore& currentScore, const std::string & currentChord, std::string currentPart, std::string nextPart, bool willGoToTheNextPart);
		void loadChordImages(std::string location);

		void changeButtonMode(bool play);

	public slots:
		void setFillRule(Qt::FillRule rule);
		void setFillGradient(const QColor &color1, const QColor &color2);
		void setPenWidth(int width);
		void setPenColor(const QColor &color);
		void setRotationAngle(int degrees);

		void playButtonClicked();
		void playWithoutSoundButtonClicked();
		void stopButtonClicked();

	protected:
		void paintEvent(QPaintEvent *event);

	private:
		std::map<std::string, unsigned int> m_chordToPosition;
		std::string m_currentChord;

		std::string m_nextPart;
		std::string m_currentPart;

		std::map<std::string, QImage> m_chordImages;

		prioritizedScore m_scoreToDraw;
		bool m_willGoToNextPart;

		QPainterPath m_background;
		QPainterPath m_presentline;
		std::vector<QPainterPath> m_notes;
		QColor fillColor1;
		QColor fillColor2;
		int penWidth;
		QColor penColor;
		int rotationAngle;

		QBoxLayout *bottomLayout;

		QPushButton* playButton;
		QPushButton* playWithoutSoundButton;
		QPushButton* stopButton;
};



#endif
