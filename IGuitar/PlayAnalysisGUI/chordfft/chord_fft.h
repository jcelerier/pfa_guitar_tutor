/*
 *  chord.h
 *  Tabs
 *
 *  Created by Pierre Hanna 11/10.
 *  Copyright 2010 Simbals. All rights reserved.
 *
 */

#ifndef CHORD_FFT_H
#define CHORD_FFT_H

#define CHROMA_SIZE 36



#include <stdio.h>

typedef
struct chord_ctrl_fft
{
  float * total_chroma[CHROMA_SIZE];
  int num_frames;
  int samplerate;
  int max_size_chroma_used;
  int hop;
  int frame;
  int total_samples;
  
} chord_ctrl_fft;



/* Chroma computation */
int chroma_compute_fft(chord_ctrl_fft *c, double *amplitude, int sample_size);

/*Chord computation initialiser*/
void chord_init_fft(chord_ctrl_fft* c, int samplerate, int hop, int frame);


/* Chord computation */
int chord_compute_fft(chord_ctrl_fft *c, FILE *f);

/* Chord controler free */
void chord_exit_fft(chord_ctrl_fft *c);

const char* getChordName_fft(int _chord);






#endif
