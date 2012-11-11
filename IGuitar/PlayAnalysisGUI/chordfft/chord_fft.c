/*
 *  chord.c
 *  Tabs
 *
 *  Created by Pierre Hanna 11/10.
 *  Copyright 2010 Simbals. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include <assert.h>

#include "chord_fft.h"
#include "ehpcp_fft.h"
#include "median_fft.h"
#include "filter_fft.h"
#include "fft.h"
#include "chordcor_fft.h"


#define MEDIAN_FILTER_ORDER 8
#define MAX_SIZE_CHROMA 2000

#define	ZERO_SIZE 32768

/*
static double hann_window(int i, int N)
{
  return 0.5*(1-cos(2*3.1415926*(double)i/(double)(N-1)));
}
*/

/* INIT */
/* ---- */

void chord_init_fft(chord_ctrl_fft* c, int samplerate, int hop, int frame)
{
  /* memory allocation */
  assert(c);

  /* variables init */
  c->num_frames=0;
  c->samplerate=samplerate;
  c->max_size_chroma_used = MAX_SIZE_CHROMA;
  c->total_samples = 0;
  c->hop=hop;
  c->frame=frame;
  
  /* Chroma init */
  int i;

  for (i=0;i<CHROMA_SIZE;i++)
  {
     c->total_chroma[i]= (float*)malloc(sizeof(float)*MAX_SIZE_CHROMA);
     assert(c->total_chroma[i]);
  }
}


/* EXIT */
/* ---- */

void chord_exit_fft(chord_ctrl_fft *c)
{
	int i;
	for (i=0; i<CHROMA_SIZE;i++)
		{
			free(c->total_chroma[i]);
		}

	
	
			
  free(c);
}



/* COMPUTATION */
/* ----------- */

int chroma_compute_fft(chord_ctrl_fft *c, double *samples, int sample_size)
{
  /* Verif*/
  if (sample_size <=0)
    {
      printf("Error chord.c : sample_size");
      return 1;
    }

  if (!samples)
    {
      printf("Error chord.c : samples");
      return 1;
    }

  if (!c)
    {
      printf("Error chord.c : c");
      return 1;
    }
    
  float chroma[CHROMA_SIZE];
  int i;


  /* ProcessFFT */
  complex spectrum[(int)sample_size];
  fft(samples, sample_size, spectrum);


  /* Compute EHPCP */
  if (!compute_EHPCP_fft(spectrum, chroma, ZERO_SIZE, CHROMA_SIZE, c->samplerate))
    return 0;

  /* Save Chroma */
  if (c->num_frames >= c->max_size_chroma_used)
    {
      c->max_size_chroma_used = 2*c->max_size_chroma_used;
      for (i=0; i<CHROMA_SIZE;i++)
	{
	  c->total_chroma[i]= (float*)realloc(c->total_chroma[i],c->max_size_chroma_used*sizeof(float));		    
	  assert(c->total_chroma[i]);
	}
    }
  
  for (i=0;i<CHROMA_SIZE;i++)
    {
      c->total_chroma[i][c->num_frames]=chroma[i];
    }
  
  c->num_frames++;
  c->total_samples += sample_size;

  return 0;
}

/* CHORD COMPUTATION */
/* ----------------- */

int chord_compute_fft(chord_ctrl_fft *c, FILE* outfile)
{

  if (c->num_frames < ((double)c->frame/c->hop))
  {
    printf("Erreur chord.c : not enough frames %d %f\n", c->num_frames,((double)c->frame/c->hop));
    return 1;
  }
	

  // median filter
  float * new_chroma[CHROMA_SIZE];
  int i;
  
  for (i=0;i<CHROMA_SIZE;i++)
    {
      new_chroma[i]= malloc(sizeof(double)*c->num_frames);
      assert(new_chroma[i]);
    }
	
  median_filter_ehpcp_fft(c->total_chroma, CHROMA_SIZE, c->num_frames, new_chroma, MEDIAN_FILTER_ORDER);


  int imax,j;
  double chroma_tmp[CHROMA_SIZE];
  int *prefilter=malloc(sizeof(int)*c->num_frames);
  int *postfilter=malloc(sizeof(int)*c->num_frames);

  for (j=0;j<c->num_frames;j++)
    {
      /* chromas */
      for (i=0;i<CHROMA_SIZE;i++)
		  chroma_tmp[i] = new_chroma[i][j];
	  
      /* DEBUG */
      /*
      for (i=0;i<CHROMA_SIZE;i++)
	printf("%f ",chroma_tmp[i]);
      printf("\n");
      */
      
      /* Correlation Chromas */
      imax = estimate_chord_fft(chroma_tmp, CHROMA_SIZE);

	  prefilter[j]=imax;
		
		/* Ouput Chords */
      //printf("%d %s\n", j, getChordName(imax)) ;
    }


	/* Post filtering */
	int f_size = 7;
	filter_fft(postfilter, prefilter, c->num_frames, f_size);
	
	int *tmp = prefilter;
	prefilter=postfilter;
	postfilter = tmp;
	
	f_size = 3;
	filter_fft(postfilter, prefilter, c->num_frames, f_size);
	
	float currentStart;
	float previousStart = 0;
	float duration = 0;

	int currentChord;
	int previousChord;

	char durationString[256];
	int durationPos;

	for (j=0;j < c->num_frames;j++)
	  {
		currentChord = postfilter[j];

		if (j==0) {
			previousChord = currentChord;
		}

		currentStart = (j*c->hop+(c->frame/2.0))/c->samplerate;
//		printf("%f\n", currentStart);
//		usleep(500*1000);

		if ((currentChord == previousChord) && (j != (c->num_frames - 1))){
			duration += (currentStart - previousStart);
		} else {
			duration += (currentStart - previousStart);
			sprintf(durationString, "%f", duration);

			durationPos = 0;
			while(durationString[durationPos] != '\0') {
				if (durationString[durationPos] == ',') {
					durationString[durationPos] = '.';
				}
				durationPos++;
			}

			fprintf(outfile, "%s %s\n", durationString, getChordName_fft(previousChord));
			duration = (currentStart - previousStart);
			previousChord = currentChord;
		}

	    previousStart = currentStart;
	}	
	

	for (i=0;i<CHROMA_SIZE;i++)
	{		
		free(new_chroma[i]);
	}

	free(prefilter);
	free(postfilter);

  return 0;
}


const char* getChordName_fft(int _chord)
{
	switch(_chord)
	{
		case 0:
			return "C"; 
		case 1:
			return "C#"; 
		case 2:
			return "D"; 
		case 3:
			return "D#"; 
		case 4:
			return "E"; 
		case 5:
			return "F"; 
		case 6:
			return "F#"; 
		case 7:
			return "G"; 
		case 8:
			return "G#"; 
		case 9:
			return "A"; 
		case 10:
			return "A#"; 
		case 11:
			return "B"; 
		
		case 100:
			return "Cm"; 
		case 101:
			return "C#m"; 
		case 102:
			return "Dm"; 
		case 103:
			return "D#m"; 
		case 104:
			return "Em"; 
		case 105:
			return "Fm"; 
		case 106:
			return "F#m"; 
		case 107:
			return "Gm"; 
		case 108:
			return "G#m"; 
		case 109:
			return "Am"; 
		case 110:
			return "A#m"; 
		case 111:
			return "Bm";
		case 112:
			return "nc";
	}
	
	return "";
}




