#include "median_fft.h"
#include <assert.h>
//#include <malloc.h>
#include <stdlib.h>
#include <string.h>

static void
_median_filter(float *in, float *out, int N, int order)
{
  int k = order/2;
  int i;
  
  /* move window */
  for (i=k;i<N-k;i++)
    {
      float window[(int)order];
      int j;
      
      for(j=0;j<order;j++)
	{
	  window[j]=in[i-k+j];
	}

      /* Sort */
      for (j=0;j<k+1;j++)
	{
	  int min = j;
	  int l;

	  for (l=j+1;l<order;l++)
	    if (window[l]<window[min])
	      min=l;

	  /* move min */
	  float tmp = window[j];
	  window[j] = window[min];
	  window[min] = tmp;
	}

      out[i-k] = window[k];
    }
}

static void
_median_filter_even(float *in, float *out, int N, int order)
{
  int k = order/2;
  int i;
  
  /* move window */
  for (i=k;i<N-k;i++)
    {
      float window[(int)order];
      int j;
      
      for(j=0;j<order;j++)
	{
	  window[j]=in[i-k+j];
	}

      /* Sort */
      for (j=0;j<k+1;j++)
	{
	  int min = j;
	  int l;

	  for (l=j+1;l<order;l++)
	    if (window[l]<window[min])
	      min=l;

	  /* move min */
	  float tmp = window[j];
	  window[j] = window[min];
	  window[min] = tmp;
	}

      out[i-k] = (window[k]+window[k-1])/2.0;
    }
}

int median_filter_fft(float *in, int size, float *out, int order)
{
  int i;
  int k = order/2;

  if (!in || size <1)
    return 0;

  if (size == 1)
    {
      if (out)
	out[0]=in[0];
      return 1;
    }

  /* signal extension */
  float *samples = (float *)malloc(sizeof(float)*(size+2*k));
  assert(samples);

  memcpy(samples+k,in,size*sizeof(float));

  for (i=0;i<k;i++)
    {
      samples[i]=in[k-1-i];
      samples[size+k+i]=in[size-1-i];
    }

  if (order%2 == 0)
    _median_filter_even(samples, out, size+2*k, order);
  else
    _median_filter(samples, out, size+2*k, order);

  free(samples);
  return 1;
}

