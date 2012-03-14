#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "util.h"

#define PI (3.14159265358979323846)
#define SQRT_2_PI (2.506628274631000502415765284811)






/************************************************************************
These are Aj set membership functions.  They are limited in scope to this file only.
*/
static double pmk_sinc(double x, double m, double d){
  double param;
  double result = 0;
  if (d == 0.0){
    result =0;
  }else if (x==m){
    result = 1;
  }else{
    param  = (x-m)/d;
    result = sin(param)/param;
  }
  return result;
}

static double pmk_gauss(double x, double m, double d){
  double parameter;
  double result;
  if (d == 0){
    result = 0;
  }else if(x == m){
    result = 1;
  }else{
    parameter  = (x-m)/d;
    parameter  = parameter * parameter;
    result     = exp(-parameter);
  }
  return result;
}


/************************************************************************/

double Get_Aj(double x, rule *r){
  double aj = 0.0;

  switch (r->if_shape){
  case SHAPE_SINC:
    aj = pmk_sinc(x, r->ifs[MEAN], r->ifs[DISP]);
    break;
  case SHAPE_GAUSS:
    aj = pmk_gauss(x, r->ifs[MEAN], r->ifs[STD]);
    break;
  default:
    printf("in %s: Fuzzy If part %d not supported yet\n",__FUNCTION__, r->if_shape);
    break;
  }

  return aj;
}


double Get_AjVj(double x, rule *r){
  return Get_Aj(x, r) * Get_Vj(r);
}

double Fuzz_SAM(double x, SAM *s){
  int i;
  double den = 0;
  double *temp = malloc(s->num_rules*sizeof(double));
  double fuzz; 
  rule *r;

  den = 0.0;
  /*Calculate set memberships and the denominator*/
  for (i = 0; i < s->num_rules; i++){
    r = GET_RULE(s, i);
    s->aj[i]= Get_Aj(x, r);
    temp[i]= s->aj[i] * Get_Vj(r);
    den += temp[i];
  }


  fuzz = 0.0;
  if (den == 0){
    printf("%s: denominator is zero!\n",__FUNCTION__);
    fflush(stdout);
    exit(2);
  }
 
  /*calculate pjs and the fuzzy result*/
  for (i = 0; i<s->num_rules; i++){
    r = GET_RULE(s, i);
    s->pj[i]=temp[i]/den;
    fuzz += s->pj[i] * Get_Cj(r);
  }
  return fuzz;
}	


void Init_SAM(SAM *s, int num_rules){
  s->num_rules = num_rules;
  s->rules = (rule *)malloc(sizeof(rule)*num_rules);
  s->aj = (double *)malloc(sizeof(double)*num_rules);
  s->pj = (double *)malloc(sizeof(double)*num_rules);
}

void Init_SAM_with_rules(SAM *s, int num_rules, rule *rules){
  printf("%s not yet supported",__FUNCTION__);
  exit(3);
}

void Free_SAM(SAM *s){
  free(s->rules);
  free(s->aj);
  free(s->pj);
}

static void Parse_Shape(int shape){
  switch(shape){
    case SHAPE_SINC:
      printf("Sinc\t");
      break;
    case SHAPE_GAUSS:
      printf("Gauss\t");
      break;
    case SHAPE_SQUARE:
      printf("Square\t");
      break;
    default:
      printf("Unknown\t");
      break;
    }
}


void Print_Rules(SAM *s){
  int i;
  printf("Total Number of rules: %d\t Rule summary:\n", s->num_rules);
  printf("Rule #\tIF Shape\tIF Mean\tIF Dispersion\tThen Shape\tThen Centroid\tThen Volume\n");
  for (i=0; i< s->num_rules; i++){
    printf("%03d\t",i+1);
    Parse_Shape(s->rules[i].if_shape);
    printf("%0.4f\t%0.4f\t",
	   s->rules[i].ifs[MEAN], 
	   s->rules[i].ifs[DISP]);
    Parse_Shape(s->rules[i].then_shape);
    printf("%0.4f\t%0.4f\n",
	   s->rules[i].then[CENTROID], 
	   Get_Vj(GET_RULE(s,i)));

  }
  printf("\nDone.\n");
}

void Set_Vj(rule *r, double Vj){
  switch (r->then_shape){
  case SHAPE_GAUSS:
    r->then[STD] = Vj/SQRT_2_PI;
    break;
  default:
    r->then[VOL] = Vj;
    break;
  }
}    

double Get_Vj(rule *r){
  double Vj;
  switch (r->then_shape){
  case SHAPE_GAUSS:
    Vj = r->then[STD]*SQRT_2_PI;
    break;
  default:
    Vj = r->then[VOL];
    break;
    }
  //Vj = r->then[VOL];
  return Vj;
}


void Set_Cj(rule *r, double Cj){
  r->then[CENTROID] = Cj;
}

double Get_Cj(rule *r){
  return r->then[CENTROID];
}


