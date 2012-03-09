#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "util.h"
#include "learn.h"

#define TWO_PI (6.283185307179586476925286766559)


void Test_Run(SAM *s, double *x, double *f_x, int num_points, int step);

int main(){
  SAM fuzzy;
  int num_rules = 6;
  int i,j;

  double x_vec[64];
  double f_x_vec[64];

  

  Init_SAM(&fuzzy, num_rules);
  
  //Initial guess at a sine wave:                                                                                                                                                                           
  for (i=0; i<64; i++){
    x_vec[i]=(double)(i)/TWO_PI;
    f_x_vec[i] = sin(x_vec[i]);
  }

  //Initialize the contents of the SAM
  for (i = 0 ; i<num_rules; i++){
    fuzzy.rules[i].if_shape = SHAPE_SINC;
    fuzzy.rules[i].then_shape = SHAPE_GAUSS;
    fuzzy.rules[i].ifs[MEAN] = x_vec[9*(i+1)];
    fuzzy.rules[i].ifs[DISP] = (x_vec[63]-x_vec[0])/(double)(5.1*num_rules);
    fuzzy.rules[i].then[CENTROID] = f_x_vec[9*(i+1)];
    fuzzy.rules[i].then[STD] = 1;

  }
  
  Print_Rules(&fuzzy);  

  for (i=0; i<1000000; i++){
    if (i%50000 ==0 || 
	i<10 || 
	(i<1000 && (i%10 ==0))||
	i==25000 ||
	((i<14300 && i>14200))||
	i==12500 ||
	i==6000
	){
      printf("Epoch: %d\n", (i+1));
      printf("Rules:\n");
      Print_Rules(&fuzzy);
      printf("Results:\n");
      Test_Run(&fuzzy, x_vec, f_x_vec, 64, 4);
    }
    for(j=0;j<64; j++){
      Learn(&fuzzy, x_vec[j], f_x_vec[j], 1e-5);
    }
    fflush(stdout);
  }


  Free_SAM(&fuzzy);
  return 0;
}


void Test_Run(SAM *s, double *x, double *f_x, int num_points, int step){
  int i;
  printf("x\tf(x)\tF(x)\n");
  for (i=0; i<num_points; i+= step){
    printf("%1.4f  %1.4f  %1.4f\n",x[i], f_x[i], Fuzz_SAM(x[i],s));
  }
  printf("\n");
}
