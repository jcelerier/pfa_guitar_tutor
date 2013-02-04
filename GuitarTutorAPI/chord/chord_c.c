/*
 *  chord.c
 *  Tabs
 *
 *  Created by Pierre Hanna 11/10.
 *  Copyright 2010 Simbals. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include <assert.h>

#include "chord_c.h"
#include "ehpcp.h"
#include "fft.h"
#include "chordcor.h"


#define MEDIAN_FILTER_ORDER 8
#define MAX_SIZE_CHROMA 2000

#define	ZERO_SIZE (32768)


static double hann_window(int i, int N)
{
  return 0.5*(1-cos(2*3.1415926*(double)i/(double)(N-1)));
}


/* INIT */
/* ---- */

/**
 * @fn chord_init
 * Initialise les structures chord_ctrl
 * @param c Structure d'accord à initialiser
 * @param samplerate Taux d'échantillonage
 * @param hop ?
 * @param frame ?
 */
void chord_init(chord_ctrl* c, int samplerate, int hop, int frame)
{
	/* memory allocation */
	assert(c);

	/* variables init */
	c->num_frames=0;
	c->samplerate=samplerate;
	c->max_size_chroma_used = MAX_SIZE_CHROMA;
	c->total_samples = 0;
	c->hop=hop;
	c->frame=frame;
}


/* EXIT */
/* ---- */

/**
 * @fn chord_exit
 * Désallocation des structures d'accord
 * @param c Structure à désallouer
 */
void chord_exit(chord_ctrl *c)
{
	free(c);
}



/* COMPUTATION */
/* ----------- */

/**
 * @fn chroma_compute
 * Analyse de ?
 * @param c Accord à analyser
 * @param samples ?
 * @param sample_size ?
 * @return 1 en cas d'erreur, 0 sinon
 */
int chroma_compute(chord_ctrl *c, double *samples, int sample_size)
{
	/* Verif*/
	if (sample_size <= 0)
	{
		printf("Error chord.c : sample_size");
		return 1;
	}

	if (!samples)
	{
		printf("Error chord.c : samples");
		return 1;
	}

	if (!c)
	{
		printf("Error chord.c : c");
		return 1;
	}

	/* ProcessFFT */
	complex spectrum[ZERO_SIZE];
	double in[ZERO_SIZE];

//	printf("taille: %d\n", sizeof(double));

	int i;
	for (i= 0; i < sample_size; i++)
		in[i] = samples[i] * hann_window(i, sample_size);
	for (i= sample_size; i < ZERO_SIZE; i++)
		in[i] = 0.0;

	fft(in, ZERO_SIZE, spectrum);



	/* Compute EHPCP */
	if (!compute_EHPCP(spectrum, c->chroma, ZERO_SIZE, CHROMA_SIZE, c->samplerate))
		return 0;

	c->num_frames++;
	c->total_samples += sample_size;

	return 0;
}

/* CHORD COMPUTATION */
/* ----------------- */

/**
 * @fn chord_compute
 * Analyse d'un accord
 * @param c Accord à analyser
 * @return "NO_CHORD" en cas d'erreur, le nom de l'accord sinon
 */
const char* chord_compute(chord_ctrl *c)
{
	if (c->num_frames < ((double)c->frame/c->hop))
	{
		printf("Erreur chord.c : not enough frames %d %f\n", c->num_frames,((double)c->frame/c->hop));
		return "NO_CHORD";
	}

	/* Correlation Chromas */
	int imax = estimate_chord(c->chroma, CHROMA_SIZE);
	// int j = c->num_frames;

	//  //printf("frame %d - temps %f : %d accord %s\n", j,
	//  //(j*c->hop+(c->frame/2.0))/c->samplerate,imax,getChordName(imax));
	//  fprintf(f, "frame %d - temps %f : %d accord %s\n", j, (j*c->hop+(c->frame/2.0))/c->samplerate,imax,getChordName(imax));

	return getChordName(imax);
}

/**
 * @fn getChordName
 * Retourne le nom de l'accord correspondant à l'entrée
 * @param _chord Accord à nommer
 * @return La chaîne vide en cas d'erreur, le nom de l'accord sinon
 */
const char* getChordName(int _chord)
{
	switch(_chord)
	{
	case 0:
		return "C";
	case 1:
		return "C#";
	case 2:
		return "D";
	case 3:
		return "D#";
	case 4:
		return "E";
	case 5:
		return "F";
	case 6:
		return "F#";
	case 7:
		return "G";
	case 8:
		return "G#";
	case 9:
		return "A";
	case 10:
		return "A#";
	case 11:
		return "B";

	case 100:
		return "Cm";
	case 101:
		return "C#m";
	case 102:
		return "Dm";
	case 103:
		return "D#m";
	case 104:
		return "Em";
	case 105:
		return "Fm";
	case 106:
		return "F#m";
	case 107:
		return "Gm";
	case 108:
		return "G#m";
	case 109:
		return "Am";
	case 110:
		return "A#m";
	case 111:
		return "Bm";
	case 112:
		return "nc";
	}

	return "";
}




