#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "util.h"
#include "learn.h"

#define TWO_PI (6.283185307179586476925286766559)


void Test_Run(SAM *s, double *x, double *f_x, int num_points);

int main(){
  SAM fuzzy;
  int num_rules = 6;
  int i,j;

  double x_vec[64];
  double f_x_vec[64];

  

  Init_SAM(&fuzzy, num_rules);
  
  //Initialize the contents of the SAM
  for (i = 0 ; i<num_rules; i++){
    fuzzy.rules[i].if_shape = SHAPE_SINC;
    fuzzy.rules[i].then_shape = SHAPE_GAUSS;
    fuzzy.rules[i].ifs[MEAN] = i;
    fuzzy.rules[i].ifs[DISP] = 2;
    fuzzy.rules[i].then[CENTROID] = (double)(rand())/(double)(0xffffffff)*1;
    fuzzy.rules[i].then[STD] = 2;

  }
  
  Print_Rules(&fuzzy);


  //Initial guess at a sine wave:
  for (i=0; i<64; i++){
    x_vec[i]=(double)(i)/TWO_PI;
    f_x_vec[i] = sin(x_vec[i]);
  }
  

  for (i=0; i<1000000; i++){
    if (i%50000 ==0){
      printf("Epoch: %d\n", (i+1));
      printf("Rules:\n");
      Print_Rules(&fuzzy);
      printf("Results:\n");
      Test_Run(&fuzzy, x_vec, f_x_vec, 64);
    }
    for(j=0;j<64; j++){
      Learn(&fuzzy, x_vec[j], f_x_vec[j], 1e-5);
    }
    fflush(stdout);
  }


  Free_SAM(&fuzzy);
  return 0;
}


void Test_Run(SAM *s, double *x, double *f_x, int num_points){
  int i;
  printf("x\tf(x)\tF(x)\n");
  for (i=0; i<num_points; i++){
    printf("%1.4f  %1.4f  %1.4f\n",x[i], f_x[i], Fuzz_SAM(x[i],s));
  }
  printf("\n");
}
