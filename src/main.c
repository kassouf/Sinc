#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "util.h"
#include "learn.h"

#define TWO_PI (6.283185307179586476925286766559)


void Report_Run(double *x, double *f_x, double *F_x, int num_points, int step);
void Evaluate_SAM (SAM *s, double *x, double *F_x, int num_points);
double Calc_MSE(double *f_x, double *F_x, int len);

int main(){
  SAM fuzzy;
  int num_rules = 3;
  int i,j, step;

  const int num_points = 64;

  double x_vec[num_points];
  double f_x_vec[num_points];
  double results_vec[num_points];
  
  Init_SAM(&fuzzy, num_rules);
  
  //Initial guess at a sine wave:                                                                                                                                                                           
  for (i=0; i<num_points; i++){
    x_vec[i]=(double)(i)/TWO_PI;
    f_x_vec[i] = sin(x_vec[i]);
  }

  step = (num_points-1)/(num_rules -1);
  //Initialize the contents of the SAM
  for (i = 0 ; i<num_rules; i++){
    fuzzy.rules[i].if_shape = SHAPE_SINC;
    fuzzy.rules[i].then_shape = SHAPE_GAUSS;
    fuzzy.rules[i].ifs[MEAN] = x_vec[i*step];
    fuzzy.rules[i].ifs[DISP] = (x_vec[63]-x_vec[0])/(double)(3*num_rules);
    fuzzy.rules[i].then[CENTROID] = f_x_vec[i*step];
    fuzzy.rules[i].then[STD] = 1;
  }
  
  Print_Rules(&fuzzy);  

  for (i=0; i<1000000; i++){
    Evaluate_SAM(&fuzzy, x_vec, results_vec, num_points);

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
      
      Report_Run(x_vec, f_x_vec, results_vec, num_points, 4);
    }

    if (i%1000 ==0){
      //every thousand learning epochs, prinout the MSE
      printf("%d: MSE: %6.5f\n",i+1, Calc_MSE(f_x_vec, results_vec, num_points));
    }

    for(j=0; j<num_points ; j++){
      Learn(&fuzzy, x_vec[j], f_x_vec[j], 1e-7);
    }
    
    

    fflush(stdout);
  }


  Free_SAM(&fuzzy);
  return 0;
}

void Evaluate_SAM (SAM *s, double *x, double *F_x, int num_points){
  int i;
  for (i=0; i<num_points; i++){
    F_x[i] = Fuzz_SAM(x[i],s);
  }
}


void Report_Run(double *x, double *f_x, double *F_x, int num_points, int step){
  int i;
  double MSE;

  
  printf("x\tf(x)\tF(x)\n");
  for (i=0; i<num_points; i+= step){
    printf("%1.4f  %1.4f  %1.4f\n",x[i], f_x[i], F_x[i]);
  }
  MSE = Calc_MSE(f_x, F_x, num_points);
  printf("MSE: %6.4f\n",MSE);
  printf("\n");
}

double Calc_MSE(double *f_x, double *F_x, int len){
  double MSE = 0;
  double error;
  int i;

  for (i=0; i<len; i++){
    error = f_x[i]-F_x[i];
    MSE += error * error;
  }
  MSE = MSE / (double)len;
  return MSE;
}

