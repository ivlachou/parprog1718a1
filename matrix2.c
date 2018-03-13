#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// compile like:  gcc -Wall -O2 -DNROWS=10000 matrix1.c -o matrix1


#define NCOLS 100

void get_walltime(double *wct) {
  struct timeval tp;
  gettimeofday(&tp,NULL);
  *wct = (double)(tp.tv_sec+tp.tv_usec/1000000.0);
}


int main() {
  double *table;
  double *random1, *random2;
  double ts,te, mflops, maccesses;
  int i = 0, j = 0;


  table = (double *)malloc(DNROWS*NCOLS*sizeof(double)); 
  random1 = (double *)malloc(DNROWS*NCOLS*sizeof(double)); 
  random2 = (double *)malloc(DNROWS*NCOLS*sizeof(double)); 
  if (table==(NULL || random1 || random2)) {
    printf("alloc error!\n");
    exit(1);
  }
  
  srand((unsigned)time(NULL));

  // warmup
  for (i=0;i<DNROWS;i++) {
      for (j=0;j<NCOLS;j++){
          table[i*NCOLS+j]=0.0;
          random1[i*NCOLS+j]=(rand()%100+1.0)/(rand()%5+1.0);
          random2[i*NCOLS+j]=(rand()%90+1.0)/(rand()%4+1.0);
      }
    
  }

  // get starting time (double, seconds) 
  get_walltime(&ts);
  
  // workload
  for (i=0;i<NCOLS;i++){
      for (j=0;j<DNROWS;j++) {
          table[i+j*NCOLS]=random1[i+j*NCOLS]*random2[i+j*NCOLS] + random2[i+j*NCOLS];
      }
  }

  // get ending time
  get_walltime(&te);

  // check results
  printf("Sanity check for rows: %d\n",DNROWS);
  printf("Last element of table: %f\n",table[DNROWS*NCOLS-1]);

  // print time elapsed and/or Maccesses/sec
  mflops = (DNROWS*NCOLS*2.0)/((te-ts)*1e6);
  maccesses = (DNROWS*NCOLS*4.0)/((te-ts)*1e6);
  printf("Time elapsed: %f\n",te-ts);
  printf("Mflops/sec: %f\n",mflops);
  printf("Maccesses/sec: %f\n",maccesses);
  
  free(table);
  free(random1);
  free(random2);

  return 0;
}

