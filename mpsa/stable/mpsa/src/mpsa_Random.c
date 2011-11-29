#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

#include <stdlib.h>
#include <math.h>
#include <unistd.h>

static long seed = -1;

int mpsa_InitRnd() {
  seed = -1 * getpid();
  return seed;
}

float ran1(long *idum);

float gwrand48() {
  float value;
  value = ran1(&seed);
  return value;
}

float gwgauss48() {
  float x, y, r, factor;
  r = 0.0;

  while(r >= 1.0 || r == 0.0) {
    x = 2.0 * gwrand48() - 1.0;
    y = 2.0 * gwrand48() - 1.0;
    r = x * x + y * y;
  }

  factor = sqrt(-2.0 * log(r) / r);
  return (x * factor);
}
  

float ran1(long *idum)
{
  int j;
  long k;
  static long iy=0;
  static long iv[NTAB];
  float temp;
  
  if (*idum <= 0 || !iy) {
    if (-(*idum) < 1) *idum=1;
    else *idum = -(*idum);
    for (j=NTAB+7;j>=0;j--) {
      k=(*idum)/IQ;
      *idum=IA*(*idum-k*IQ)-IR*k;
      if (*idum < 0) *idum += IM;
      if (j < NTAB) iv[j] = *idum;
    }
    iy=iv[0];
  }
  k=(*idum)/IQ;
  *idum=IA*(*idum-k*IQ)-IR*k;
  if (*idum < 0) *idum += IM;
  j=iy/NDIV;
  iy=iv[j];
  iv[j] = *idum;
  if ((temp=AM*iy) > RNMX) return RNMX;
  else return temp;
}
#undef IA
#undef IM
#undef AM
#undef IQ
#undef IR
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX
/* (C) Copr. 1986-92 Numerical Recipes Software ?4210(9p#3. */
