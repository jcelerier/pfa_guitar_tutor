#ifndef EHPCP_FFT_H
#define EHPCP_FFT_H

#include <complex.h>
#include <math.h>

/* Compute EHPCP */
int compute_EHPCP_fft(complex *spectrum, float *chroma, int spec_size, int chroma_size, double sr);

/* Median filtering EHPCP */
void median_filter_ehpcp_fft(float **chroma, int chroma_size, int size, float **new_chroma, int order);

#endif
