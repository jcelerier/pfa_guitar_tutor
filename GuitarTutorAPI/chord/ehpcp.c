#include "ehpcp.h"
//#include "median.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

static int freq2chro(double freq, int chroma_size)
{
  return (int)round(chroma_size*log2(freq/261.6256));
}

static int my_modulo(int x, int y)
{
  int res = fmod(x,y);
  if (res <0)
    res+=y;
  return res;
}

static double max_value(double *amp, int low, int high, int *bin)
{
  int i;
  double max = -99999.0;
  *bin = low;

  for (i=low;i<=high;i++)
    if (amp[i] > max)
      {
	max=amp[i];
	*bin=i;
      }

  return max;
}

/*
static void normalize(double *chroma, int chroma_size)
{
  int i, imax;

  double m = max_value(chroma,0,chroma_size-1,&imax);
  if (m > 0.0)
    for (i=0;i<chroma_size;i++)
      chroma[i]/=m;
}
*/

 /*
   void median_filter_ehpcp(float **chroma, int chroma_size, int size, float **new_chroma, int order)
{
  int i;

  for (i=0;i<chroma_size;i++)
    {
      if (! median_filter(chroma[i], size, new_chroma[i], order))
	{
	  printf("error median filter !!\n");
	}
    }
}
 */
 
int compute_EHPCP(complex *spectrum, float *chroma, int spec_size, int chroma_size, double sr)
{
  int i;
  double fstep=sr/(double)spec_size;
  double kmin = floor(30.0/fstep);
  double fmin = kmin*fstep; // Hz
  double kmax = ceil(2500/fstep);

  double inharm_w=6.0/100.0; // cumul bandwidth
  double exp_value=0.85;

  /* reset chroma */
  for (i=0;i<chroma_size;i++)
    chroma[i] = 0.0;



  /* amplitude spectrum processing */
  double *amp = (double*)malloc((spec_size/2)*sizeof(double));
  double *amp_save = (double*)malloc((spec_size/2)*sizeof(double));
  double *freq = (double*)malloc((spec_size/2)*sizeof(double));
  assert(amp);
  assert(amp_save);
  assert(freq);


  for (i=0;i<spec_size/2;i++)
    {
	 // printf("%d\n", i);
      amp[i] = cabs(spectrum[i]);
      //amp[i] = cabs(0);
      amp_save[i] = amp[i];
      freq[i] = (double)i*fstep;
    }


  /* peak selection */
  for (i=kmax-2;i>kmin-1;i--)
    {
      if (amp_save[i] < amp_save[i+1] || amp_save[i] < amp_save[i-1])
	{
	  amp[i]=0.0;
	}
    }

    for (i=kmax-2;i>kmin-1;i--)
    {
      amp_save[i] = amp[i];
    }
    

  /* from high freq to low */
  for (i=kmax-2;i>kmin-1;i--)
    {
      double nmax;
      double sum_bin;
      int j;
      double freq_cur;
      double bin_l, bin_h;
      double m;
      double contrib;
      int bin_max[7];
      int bin_chroma;

      /* peak selection */
      if (amp_save[i] !=0 )
	{
	  nmax=floor(freq[i]/fmin);
	  if (nmax > 7)
            nmax=7;
	  
	  sum_bin = amp_save[i]; //init
	  
	  for (j=2;j<nmax+1;j++)
	    {
	      freq_cur = freq[i]/(double)j;

	      //printf("freq_cur %f %f %f\n", freq[i], freq_cur, fmin);
	      
	      bin_l=round(((freq_cur-freq_cur*inharm_w)-fmin+1)/fstep)+1;
	      bin_h=round(((freq_cur+freq_cur*inharm_w*0.5)-fmin+1)/fstep)+1;//asymetric
									     //inharmonicity
	      //printf("binl %f - binh %f \n", bin_l, bin_h);
	      
	      if (bin_l <1)
		bin_l=1;// no lesser than fmin

	      // amplitude max sur intervalle
	      m=max_value(amp_save,bin_l+fmin-2,bin_h+fmin-2,bin_max+(j-1));

	      //sum_bin +=m;
	      sum_bin += (pow(exp_value,j-1)*m);
	    }

	  //return;
	  
	  for (j=1;j<nmax;j++)
	    {
	      contrib = pow(exp_value,j)*amp_save[bin_max[j]]/sum_bin*amp_save[i];
	      amp[i] -= contrib;
	      amp[bin_max[j]] += contrib;
	    }

	  //printf("contrib %f\n", contrib);
	  //break;
	 
	  // contribute chroma
	  bin_chroma = my_modulo(freq2chro(freq[i],chroma_size),chroma_size);
	  chroma[bin_chroma] += amp[i];
	}	  
    }

  //normalize(chroma, chroma_size);

  /* offset */
  
  if (chroma_size==36)
    {
      double chroma_save= chroma[35];
      
      for (i=chroma_size-2;i>=0;i--)
	chroma[i+1]=chroma[i];

      chroma[0] = chroma_save;
    }
  
  free(amp);
  free(amp_save);
  free(freq);

  return 1;
}

