#ifndef CHORDCOR_H
#define CHORDCOR_H

/* correlation */
float correlationmaj(float *val, int size);
float correlationmin(float *val, int size);

/* search chord */
int estimate_chord(float *chroma, int chroma_size);

#endif
