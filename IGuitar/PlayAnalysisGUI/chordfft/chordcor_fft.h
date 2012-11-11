#ifndef CHORDCOR_FFT_H
#define CHORDCOR_FFT_H

/* correlation */
double correlationmaj_fft(double *val, int size);
double correlationmin_fft(double *val, int size);

/* search chord */
int estimate_chord_fft(double *chroma, int chroma_size);

#endif
