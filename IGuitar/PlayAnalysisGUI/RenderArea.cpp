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

#include <iostream>
#include <QtGui>
#include <QRect>


#include "RenderArea.hpp"
#include "MainWindow.hpp"

RenderArea::RenderArea(QWidget *parent)
: QWidget(parent)
{
	penWidth = 1;
	rotationAngle = 0;
	setBackgroundRole(QPalette::Base);

	m_background.moveTo(BACKGROUND_UPLEFT_X, BACKGROUND_UPLEFT_Y);
	m_background.lineTo(BACKGROUND_UPLEFT_X + BACKGROUND_HORIZONTAL_LENGTH, BACKGROUND_UPLEFT_Y);
	m_background.lineTo(BACKGROUND_UPLEFT_X + BACKGROUND_HORIZONTAL_LENGTH, BACKGROUND_UPLEFT_Y + BACKGROUND_VERTICAL_LENGTH);
	m_background.lineTo(BACKGROUND_UPLEFT_X, BACKGROUND_UPLEFT_Y + BACKGROUND_VERTICAL_LENGTH);
	m_background.closeSubpath();

	float scoreSize = MAX_SCORE_BOUND - MIN_SCORE_BOUND;
	float presentLineYPos = ((float)MAX_SCORE_BOUND)/scoreSize * 100;

	m_presentline.moveTo(BACKGROUND_UPLEFT_X + PRESENT_LINE_OFFSET, presentLineYPos);
	m_presentline.lineTo(BACKGROUND_UPLEFT_X + BACKGROUND_HORIZONTAL_LENGTH - PRESENT_LINE_OFFSET, presentLineYPos);
	m_presentline.lineTo(BACKGROUND_UPLEFT_X + BACKGROUND_HORIZONTAL_LENGTH - PRESENT_LINE_OFFSET, presentLineYPos + PRESENT_LINE_HEIGHT);
	m_presentline.lineTo(BACKGROUND_UPLEFT_X + PRESENT_LINE_OFFSET,  presentLineYPos + PRESENT_LINE_HEIGHT);
	m_presentline.closeSubpath();

	m_willGoToNextPart = false;

	setPenWidth(0);
	setRotationAngle(0);


	beginRecordButton = new QPushButton("begin record", this);
	recordButton = new QPushButton("stop record", this);
	playButton = new QPushButton("play", this);
	playWithoutSoundButton = new QPushButton("play without music", this);
	stopButton = new QPushButton("stop", this);


	//stopButton->setAlignment(Qt::AlignBottom);

	bottomLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);

//	QRect imageRect(5, 5,
//				1, 1);

	bottomLayout->addWidget(beginRecordButton);
	bottomLayout->addWidget(recordButton);
	bottomLayout->addWidget(playButton);
	bottomLayout->addWidget(playWithoutSoundButton);
	bottomLayout->addWidget(stopButton);
	//bottomLayout->addWidget(stopButtonWithoutPlayBack);
	bottomLayout->setAlignment(Qt::AlignBottom | Qt::AlignRight);

	QObject::connect(beginRecordButton, SIGNAL(clicked()), this, SLOT(beginRecordButtonClicked()));
	QObject::connect(recordButton, SIGNAL(clicked()), this, SLOT(recordButtonClicked()));
	QObject::connect(playButton, SIGNAL(clicked()), this, SLOT(playButtonClicked()));
	QObject::connect(playWithoutSoundButton, SIGNAL(clicked()), this, SLOT(playWithoutSoundButtonClicked()));
	QObject::connect(stopButton, SIGNAL(clicked()), this, SLOT(stopButtonClicked()));

	recordButton->setVisible(false);
	playButton->setVisible(false);
	playWithoutSoundButton->setVisible(false);
	stopButton->setVisible(false);

	playButtonCanBePressed = false;

	//bottomLayout->setGeometry(imageRect);
}


void RenderArea::drawScore(const prioritizedScore& currentScore, const std::string& currentChord, std::string currentPart, std::string nextPart, bool willGoToTheNextPart)
{
	m_notes.clear();
	m_scoreToDraw = currentScore;

	m_currentChord = currentChord;

	m_currentPart = currentPart;
	m_nextPart = nextPart;

	m_willGoToNextPart = willGoToTheNextPart;

	update();
}

void
RenderArea::loadChordImages(std::string location)
{
	QDir dir(location.data());
	if (!dir.exists()) {
		qWarning("Cannot find the chords images directory");
	} else {
		dir.setFilter(QDir::Files | QDir::NoSymLinks);
		QFileInfoList list = dir.entryInfoList();
		for (int i = 0; i < list.size(); ++i) {
			QFileInfo fileInfo = list.at(i);
			QImage currentImage(fileInfo.filePath());
			m_chordImages[fileInfo.baseName().toStdString()] = currentImage;

			if (currentImage.isNull()) {
				std::cout << fileInfo.baseName().toStdString() << std::endl;
				std::cout << "alors la n'importe quoi" << std::endl;
			}
		}
	}
}

QSize RenderArea::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize RenderArea::sizeHint() const
{
	return QSize(100, 100);
}

void RenderArea::setFillRule(Qt::FillRule rule)
{
	for (unsigned int i = 0; i < m_notes.size(); ++i) {
		m_notes[i].setFillRule(rule);
	}

	update();
}

void RenderArea::setFillGradient(const QColor &color1, const QColor &color2)
{
	fillColor1 = color1;
	fillColor2 = color2;
	update();
}

void RenderArea::setPenWidth(int width)
{
	penWidth = width;
	update();
}

void RenderArea::setPenColor(const QColor &color)
{
	penColor = color;
	update();
}

void RenderArea::setRotationAngle(int degrees)
{
	rotationAngle = degrees;
	update();
}

void RenderArea::beginRecordButtonClicked()
{
	((MainWindow*)parentWidget())->startRecord();
}

void RenderArea::setStopRecordButtonVisible()
{
	beginRecordButton->setVisible(false);
	recordButton->setVisible(true);
}

void RenderArea::recordButtonClicked()
{

	recordButton->setText("Compute score...");
	//		stopButtonWithoutPlayBack->setText("Compute score...");
	//		update();
	//		stopButton->update();
	//update(stopButton->boundingRect());

	//	SleeperThread::msleep(1000);

	((MainWindow*)parentWidget())->saveChords();



}

void RenderArea::playButtonClicked()
{
	((MainWindow*)parentWidget())->playRecord(true);
}

void RenderArea::playWithoutSoundButtonClicked()
{
	((MainWindow*)parentWidget())->playRecord(false);
}

void RenderArea::stopButtonClicked()
{
	((MainWindow*)parentWidget())->stopRecord();
}

void RenderArea::createPlayStopButtons()
{
//	bottomLayout->removeWidget(stopButton);
	recordButton->setVisible(false);

	playButton->setVisible(true);
	playWithoutSoundButton->setVisible(true);
	stopButton->setVisible(true);
}



void RenderArea::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.scale(width() / 100.0, height() / 100.0);

	painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap,
			Qt::RoundJoin));
	QLinearGradient gradient(0, 0, 0, 100);
	gradient.setColorAt(0.0, BACKGROUND_COLOR1);
	gradient.setColorAt(1.0, BACKGROUND_COLOR2);
	painter.setBrush(gradient);

	painter.drawPath(m_background);

	float scoreVerticalSize = MAX_SCORE_BOUND - MIN_SCORE_BOUND;
	float scoreHorizontalSize = BACKGROUND_HORIZONTAL_LENGTH - 2 * (PRESENT_LINE_OFFSET + SCORE_OFFSET);
	float scoreXBegin = BACKGROUND_UPLEFT_X + PRESENT_LINE_OFFSET + SCORE_OFFSET;
	float scoreXEnd = scoreXBegin + scoreHorizontalSize;

	while (!m_scoreToDraw.empty()) {
		QPainterPath currentNoteDraw;

		PrioritizedNote currentNote = m_scoreToDraw.top();

		float noteBegin = 100.0 - (currentNote.relativeBegin - MIN_SCORE_BOUND)/scoreVerticalSize * 100.0;
		float noteEnd = 100.0 - (currentNote.relativeEnd - MIN_SCORE_BOUND)/scoreVerticalSize * 100.0;

		currentNoteDraw.moveTo(scoreXBegin, noteBegin);
		currentNoteDraw.lineTo(scoreXEnd, noteBegin);
		currentNoteDraw.lineTo(scoreXEnd, noteEnd);
		currentNoteDraw.lineTo(scoreXBegin, noteEnd);
		currentNoteDraw.closeSubpath();

		gradient.setColorAt(0.0, currentNote.isValidated?NOTE_VALIDATED_COLOR:(currentNote.isInCurrentPart?NOTE_IN_CURRENT_PART_COLOR:NOTE_NOT_IN_CURRENT_PART_COLOR));
		gradient.setColorAt(1.0, currentNote.isValidated?NOTE_VALIDATED_COLOR:(currentNote.isInCurrentPart?NOTE_IN_CURRENT_PART_COLOR:NOTE_NOT_IN_CURRENT_PART_COLOR));

		painter.setBrush(gradient);
		painter.drawPath(currentNoteDraw);

		QPainterPath currentNoteText;
//		QRect imageRect(scoreXBegin - SCORE_NOTE_OFFSET_X - CHORD_IMAGE_WIDTH, noteBegin + SCORE_NOTE_OFFSET_Y - CHORD_IMAGE_HEIGHT,
//				CHORD_IMAGE_WIDTH, CHORD_IMAGE_HEIGHT);
//
//		if (m_chordImages.find(currentNote.chord.data()) != m_chordImages.end()) {
//			painter.drawImage(imageRect, m_chordImages[currentNote.chord.data()]);
//		}

		QFont font(SCORE_NOTE_FONT, SCORE_NOTE_SIZE);
		font.setStyleStrategy(QFont::ForceOutline);
		currentNoteText.addText(scoreXBegin + SCORE_NOTE_OFFSET_X, noteBegin + SCORE_NOTE_OFFSET_Y, font, tr(currentNote.chord.data()));

		gradient.setColorAt(0.0, SCORE_NOTE_COLOR);
		gradient.setColorAt(1.0, SCORE_NOTE_COLOR);

		painter.setBrush(gradient);
		painter.drawPath(currentNoteText);

		m_scoreToDraw.pop();
	}

	float presentLineYPos = ((float)MAX_SCORE_BOUND)/scoreVerticalSize * 100;

	gradient.setColorAt(0.0, m_willGoToNextPart?PRESENT_LINE_COLOR_WITH_VALIDATION:PRESENT_LINE_COLOR_WITHOUT_VALIDATION);
	gradient.setColorAt(1.0, m_willGoToNextPart?PRESENT_LINE_COLOR_WITH_VALIDATION:PRESENT_LINE_COLOR_WITHOUT_VALIDATION);

	painter.setBrush(gradient);

	painter.drawPath(m_presentline);

	QFont partFont(SCORE_NOTE_FONT, PART_FONT_SIZE);
//
	float partX = BACKGROUND_UPLEFT_X + BACKGROUND_HORIZONTAL_LENGTH + PART_OFFSET_X;
	float partY = BACKGROUND_UPLEFT_Y + PART_VERTICAL_LINE_SPACE;
//
	std::string partText("Input Chord:");
	QPainterPath currentPartIntroText;
	currentPartIntroText.addText(partX, partY, partFont, tr(partText.data()));

	QPainterPath currentPlayedChordText;

	partY += PART_VERTICAL_LINE_SPACE * 2;

	QFont font(SCORE_NOTE_FONT, CURRENT_CHORD_FONT_SIZE);
	font.setStyleStrategy(QFont::ForceOutline);
	currentPlayedChordText.addText(partX, partY, font, tr(m_currentChord.data()));

	gradient.setColorAt(0.0, SCORE_NOTE_COLOR);
	gradient.setColorAt(1.0, SCORE_NOTE_COLOR);

	painter.setBrush(gradient);
	painter.drawPath(currentPlayedChordText);
//
//	partY += PART_VERTICAL_LINE_SPACE;
//	partText = m_currentPart;
//	QPainterPath currentPartText;
//	currentPartText.addText(partX, partY, partFont, tr(partText.data()));
//
//	partY = BACKGROUND_UPLEFT_Y + PART_VERTICAL_LINE_SPACE;
//	partX += PART_HORIZONTAL_LINE_SPACE;
//	partText = NEXT_PART_STRING;
//	QPainterPath nextPartIntroText;
//	nextPartIntroText.addText(partX, partY, partFont, tr(partText.data()));
//
//	partY += PART_VERTICAL_LINE_SPACE;
//	partText = m_nextPart;
//	QPainterPath nextPartText;
//	nextPartText.addText(partX, partY, partFont, tr(partText.data()));
//
	gradient.setColorAt(0.0, SCORE_NOTE_COLOR);
	gradient.setColorAt(1.0, SCORE_NOTE_COLOR);
//
//	painter.setBrush(gradient);
	painter.drawPath(currentPartIntroText);
//	painter.drawPath(currentPartText);
//	painter.drawPath(nextPartIntroText);
//	painter.drawPath(nextPartText);

//	QPoint imageLocation(0.0, 0.0);
//	painter.drawImage(imageLocation, m_chordImages["A#m"]);
}
