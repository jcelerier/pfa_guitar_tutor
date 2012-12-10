#ifndef EHPCP_H
#define EHPCP_H

#include <complex.h>
#include <math.h>

/* Compute EHPCP */
int compute_EHPCP(complex *spectrum, float *chroma, int spec_size, int chroma_size, double sr);

/* Median filtering EHPCP */
void median_filter_ehpcp(float **chroma, int chroma_size, int size, float **new_chroma, int order);

#endif
