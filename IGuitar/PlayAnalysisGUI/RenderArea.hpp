/****************************************************************************
 **
 ** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
 **     the names of its contributors may be used to endorse or promote
 **     products derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QThread>
#include <QPainterPath>
#include <QPushButton>
#include <QWidget>
#include <QImage>
#include <QDir>
#include <QBoxLayout>
#include <vector>
#include <map>

#include <ScoreManager.h>

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

#define CURRENT_CHORD_FONT_SIZE 8.0

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

class SleeperThread : public QThread
{
    public:
    static void msleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
};


class RenderArea : public QWidget
{
	Q_OBJECT





public:
	RenderArea(QWidget *parent = 0);

	QSize minimumSizeHint() const;
	QSize sizeHint() const;

	void drawScore(const prioritizedScore& currentScore, const std::string & currentChord, std::string currentPart, std::string nextPart, bool willGoToTheNextPart);
	void loadChordImages(std::string location);

	void createPlayStopButtons();
	void setStopRecordButtonVisible();


public slots:
void setFillRule(Qt::FillRule rule);
void setFillGradient(const QColor &color1, const QColor &color2);
void setPenWidth(int width);
void setPenColor(const QColor &color);
void setRotationAngle(int degrees);

protected slots:
void recordButtonClicked();
void playButtonClicked();
void playWithoutSoundButtonClicked();
void stopButtonClicked();
void beginRecordButtonClicked();



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

QPushButton* beginRecordButton;
QPushButton* recordButton;
QPushButton* playButton;
QPushButton* playWithoutSoundButton;
QPushButton* stopButton;

bool playButtonCanBePressed;
};



#endif
