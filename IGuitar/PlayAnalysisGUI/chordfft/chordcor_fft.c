#include "chordcor_fft.h"
#include <stdio.h>

static double profilemaj36[36]=
  {1.0,0.75,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.75, 1.0,0.75,0.0, 0.0,0.0,0.0, 0.0,0.0,0.75, 1.0,0.75,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.75
  };

static double profilemaj12[12]=
  {
    1.0,0.0,0.0,0.0,1.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0
  };

static double profilemin36[36]=
  {1.0,0.75,0.0, 0.0,0.0,0.0, 0.0,0.0,0.75, 1.0,0.75,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.75, 1.0,0.75,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.75
  };

static double profilemin12[12]=
  {
    1.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0
  };

double correlation_fft(double *val, int size, double *profile);

static
void offset_one(double *val, int size)
{
  int i;

  double tmp = val[0];
  
  for (i=0;i<size-1;i++)
    {
      val[i]=val[i+1];
    }

  val[i-1]=tmp;
}


/* correlation */
double correlation_fft(double *val, int size, double *profile)
{
  int i;
  double res = 0.0;
      
  for (i=0; i<size; i++)
    {
      res += (val[i]*profile[i]);
    }
  return res;
}

/* correlation */
double correlationmaj_fft(double *val, int size)
{
  if (size == 12)
    {
      return correlation_fft(val, size, profilemaj12);
    }
  else if (size == 36)
    {
      return correlation_fft(val, size, profilemaj36);
    }
  else
    {
      printf("Error chordcor.c : chroma size");
      return 0.0;
    }
}

/* correlation */
double correlationmin_fft(double *val, int size)
{
  if (size == 12)
    {
      return correlation_fft(val, size, profilemin12);
    }
  else if (size == 36)
    {
      return correlation_fft(val, size, profilemin36);
    }
  else
    {
      printf("Error chordcor.c : chroma size");
      return 0.0;
    }
}

float rms_fft(float *tab, int size)
{
	float total = 0;
	int i;
	for (i=0; i<size; i++)
		total+=tab[i]*tab[i];
	total/=(float)size;
	total=sqrt(total);
	return 20.0*log10(total);
}

/* search chord */
int estimate_chord_fft(double *chroma, int chroma_size)
{
  int i;
  double tmpmaj, tmpmin;
  double m = 0.0;
  int im = 0;
  
  if (rms_fft(chroma, chroma_size) < 30) // -60
  	  return 112;

  for (i=0;i<chroma_size;i++)
    {
      tmpmaj = correlationmaj_fft(chroma, chroma_size);
      tmpmin = correlationmin_fft(chroma, chroma_size);
      
      if (tmpmaj > m)
	{
	  m=tmpmaj;
	  im = i/(chroma_size/12);
	}
      
      if (tmpmin > m)
	{
	  m=tmpmin;
	  im = i/(chroma_size/12)+100;
	}
      
      /* offset */
      offset_one(chroma, chroma_size);
    }

  return im;
}
