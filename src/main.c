#include <stdio.h>
#include <stdlib.h>


#include "util.h"


int main(){
  SAM fuzzy;
  int num_rules = 3;
  int i;

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

  printf("Fuzzy 1: %f\n",Fuzz_SAM(0.1, &fuzzy));

  Free_SAM(&fuzzy);
  return 0;
}


