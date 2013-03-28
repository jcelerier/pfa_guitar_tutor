#include "fft.h"


static void butterfly (complex *spectrum, int i, int d, int n);

void fft (double *samples, int n, complex *spectrum)
{
	int nbits, d, i, j, k;
	complex *cptr;

	/* Assuming that the number of samples is a power of two,
	 compute the number of bits for the bit reversal.  */
	for (nbits=0, i=n; i>1; )
	{
		i >>= 1; nbits++;
	}

	/* Place the samples in the output array, in bit-reversed order.
	 Zero the imaginary part of each value in the output array.  */

	for (i=0, cptr=spectrum; i<n; i++)
	{
		for (j=nbits, k=0, d=(i<<1); j>0; j--)
		{
			k = (k << 1) | ((d >>= 1) & 1);
		}
		cptr[i] = samples[k];
	}

	/* Do the combinations.  */

	/* n=2^N échantillons
	 d=1,2,4,8,16,32,...,n
	 nombre de remontées dans l'algo */
	for (d=1; d<n; d+=d)
	{
		for (i=(n/2), j=0; i>0; )
		{
			butterfly (spectrum, j, d, n);
			i--;

            //#pragma omp parallel for
			for (k=1; k<d; k++)
			{
				butterfly (spectrum, j+k, d, n);
				i--;
			}
			j += 2*d;
		}
	}
}

static void butterfly (complex *spectrum, int i, int d, int n)
{
	double x;
	complex w, tmp;

	x = (double) ((i%d)*(n/(2*d)));
	w = cexp(-I*(2*M_PI*x) / n );

	tmp = ( creal(spectrum[i+d]) * creal(w) - (cimag(spectrum[i+d]) * cimag(w))) +
			I * ( creal(spectrum[i+d]) * cimag(w) + (cimag(spectrum[i+d]) * creal(w)));

	spectrum[i+d] = spectrum[i] - tmp;
	spectrum[i]   = spectrum[i] + tmp;
}
