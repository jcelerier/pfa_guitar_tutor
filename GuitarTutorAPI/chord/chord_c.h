/*
 *  chord.h
 *  Tabs
 *
 *  Created by Pierre Hanna 11/10.
 *  Copyright 2010 Simbals. All rights reserved.
 *
 */

#ifndef CHORD_H
#define CHORD_H

#define CHROMA_SIZE 36

/**
 * @brief Structure représentative d'un accord
 */
typedef struct chord_ctrl
{
  float chroma[CHROMA_SIZE]; //!< ?
  int num_frames; //!< Nombre de frames
  int samplerate; //!< Taux d'échantillonage
  int max_size_chroma_used; //!< ?
  int hop; //!< ?
  int frame; //!< ?
  int total_samples; //!< Nombre total de samples
  
} chord_ctrl;


/* Chroma computation */
int chroma_compute(chord_ctrl *c, double *amplitude, int sample_size);

/*Chord computation initialiser*/
void chord_init(chord_ctrl* c, int samplerate, int hop, int frame);


/* Chord computation */
const char* chord_compute(chord_ctrl *c);

/* Chord controler free */
void chord_exit(chord_ctrl *c);

const char *getChordName(int _chord);


#endif
