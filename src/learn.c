#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "learn.h"
#include "util.h"

static void Learn_If_Sinc (rule *r, double mu, double x, double error, double df_daj, double aj){
  double daj_dm;
  double daj_dd;
  double mj = r->ifs[MEAN];
  double dj = r->ifs[DISP];
  double argument;

  if (dj != 0){
    argument = (x-mj)/dj;
    daj_dd   = (aj - cos(argument))/(dj); 
  }else{
    argument = 0;
    daj_dd = 0;
  }

  if((x-mj)!=0){
    daj_dm = (aj - cos(argument))/(x-mj);
  }else{
    daj_dm = 0;
  }
  
  r->ifs[MEAN] = mj + mu * df_daj * daj_dm;
  r->ifs[DISP] = dj + mu * df_daj * daj_dd;
  
}

static void Learn_Then(rule *r, double mu, double error, double pj, double F_x){
  double new_vj;
  double new_cj;
  register double cj = Get_Cj(r);
  register double vj = Get_Vj(r);

  new_cj = cj + mu * error * pj;
  if(vj != 0){
    new_vj = vj + mu * error * (cj - F_x)*(pj/vj);
  }else{
    new_vj = 0;
  }
  
  Set_Cj(r, new_cj);
  Set_Vj(r, new_vj);

}

void Learn(SAM* s, double x, double f_x, double mu){
  double error;
  double F_x; //fuzzy result
  int i;
  double df_daj;
  register rule *r;
  register double aj;
  register double pj;

  //calculate the error used for the gradient descent
  F_x = Fuzz_SAM(x, s);
  error = (f_x - F_x);

  //iterate through the rules and update:
  //if part mean and dispersion
  //then part centroid and volume
  for (i=0; i< s->num_rules; i++){
    r = &(s->rules[i]);  //point to current rule
    aj = s->aj[i];
    pj = s->pj[i];
    //calculate df/daj first, needed for all subsequent math
    if (aj!=0){
      df_daj = (Get_Cj(r) - F_x) * pj / aj;
    }else{
      df_daj = 0;
    }
    
    //update the if parts
    switch(r->if_shape){
    case SHAPE_SINC:
      Learn_If_Sinc(r, mu, x, error, df_daj, aj);
      break;
    default:
      printf("%s: %d if learning not yet supported",__FUNCTION__, r->if_shape);
      exit(3);
    }//end switch
    
    //update the then parts
    Learn_Then(r, mu, error, pj, F_x);

  }//end for
}


