/*
 *  filter.c
 *  Tabs
 *
 *  Created by Matthias Robine and Pierre Hanna on 30/11/10.
 *  Copyright 2010 Simbals. All rights reserved.
 *
 */

#include "filter_fft.h"

#include <stdlib.h>
#include <stdio.h>

int
imax(int* tab)
{
  int i, ix = 0, max = -1;
  
  for(i=0; i<25; i++)
    if (tab[i] > max)
      {
	max = tab[i];
	ix = i;
      }
  return ix;
}

int
max_occ(int* tab, int size)
{
  int i, val, ix;
  int occ[25] = {0};
  
  for (i=0; i<size; i++)
    {
      val = tab[i];
      if(val>99) val -= 88;
      occ[val]++;
    }
  
  ix = imax(occ);
  if(ix>11) ix+=88;
  return ix;  
}

// filtre "in" dans "out", de taille f_size
void
filter_fft(int* out, int* in, int in_size, int f_size)
{
  int i, j, half = f_size/2;
  int* w = (int*)malloc(f_size*sizeof(int));
 
  // debut inchange
  for(i=0; i<in_size && i<half; i++) 
    out[i] = in[i];
    
  // filtrage fenetre w
  while(i+half<in_size)
    {
      for(j=-half; j<=half; j++)
	  w[j+half] = in[i+j];
      out[i] = max_occ(w, f_size);
      i++;
    }
  
  // fin inchangee
  while(i<in_size)
    {
      out[i] = in[i];
      i++;
    }
}

