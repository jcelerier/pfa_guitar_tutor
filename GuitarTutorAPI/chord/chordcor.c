#include "chordcor.h"
#include <stdio.h>
#include <math.h>

static float profilemaj36[36]=
  {1.055,0.75,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.75, 1.0,0.75,0.0, 0.0,0.0,0.0, 0.0,0.0,0.75, 1.0,0.75,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.75
  };

static float profilemaj12[12]=
  {
    1.055,0.0,0.0,0.0,1.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0
  };

static float profilemin36[36]=
  {1.05,0.75,0.0, 0.0,0.0,0.0, 0.0,0.0,0.75, 1.0,0.75,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.75, 1.0,0.75,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.75
  };

static float profilemin12[12]=
  {
    1.05,0.0,0.0,1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0
  };

float correlation(float *val, int size, float *profile);

static
void offset_one(float *val, int size)
{
  int i;

  float tmp = val[0];
  
  for (i=0;i<size-1;i++)
    {
      val[i]=val[i+1];
    }

  val[i-1]=tmp;
}


/* correlation */
float correlation(float *val, int size, float *profile)
{
  int i;
  float res = 0.0;
      
  for (i=0; i<size; i++)
    {
      res += (val[i]*profile[i]);
    }
  return res;
}

/* correlation */
float correlationmaj(float *val, int size)
{
  if (size == 12)
    {
      return correlation(val, size, profilemaj12);
    }
  else if (size == 36)
    {
      return correlation(val, size, profilemaj36);
    }
  else
    {
      printf("Error chordcor.c : chroma size");
      return 0.0;
    }
}

/* correlation */
float correlationmin(float *val, int size)
{
  if (size == 12)
    {
      return correlation(val, size, profilemin12);
    }
  else if (size == 36)
    {
      return correlation(val, size, profilemin36);
    }
  else
    {
      printf("Error chordcor.c : chroma size");
      return 0.0;
    }
}

/* RMS */
float rms(float *tab, int size)
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
int estimate_chord(float *chroma, int chroma_size)
{
  int i;
  float tmpmaj, tmpmin;
  float m = 0.0;
  int im = 0;
  
  // silence : no chord
  //printf("rms %f\n", rms(chroma, chroma_size));
  
  
  if (rms(chroma, chroma_size)< 30) // -60
	  return 112;

  for (i=0;i<chroma_size;i++)
    {
      tmpmaj = correlationmaj(chroma, chroma_size);
      tmpmin = correlationmin(chroma, chroma_size);
      
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
