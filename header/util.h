#ifndef _PMK_UTIL_H_
#define _PMK_UTIL_H_



#define MEAN 0
#define CENTROID 0

#define DISP 1
#define VAR  1
#define VOL  1
#define STD  1

#define SHAPE_SINC	0
#define SHAPE_GAUSS	1
#define SHAPE_SQUARE	2

typedef struct _rule{
	int if_shape;
	int then_shape;

	double ifs[2];
	double then[2];
}rule;

typedef struct _SAM{
	int num_rules;
	rule *rules;
	double *aj;
	double *pj;
}SAM;

extern void Init_SAM(SAM *s, int num_rules);
extern void Init_SAM_with_rules(SAM *s, int num_rules, rule *rules);
extern void Free_SAM(SAM *s);

/*IF PART function*/
extern double Get_Aj(double x, rule *r);
extern double Get_AjVj(double x, rule *r);

/*Then Part functions*/
extern void Set_Vj(rule *r, double Vj);
extern double Get_Vj(rule *r);

extern void Set_Cj(rule *r, double Cj);
extern double Get_Cj(rule *r);

/*SAM model*/
extern double Fuzz_SAM(double x, SAM *s);

/*Reporting Stuff*/
extern void Print_Rule(rule *r);


/************************************************************************
These are Simple Gaussian Sigma to Volume and vice versa conversion functions
and macros
*/
#define VJ_FROM_STD(STD)((STD)*SQRT_2_PI)
#define STD_FROM_VJ(VJ)((VJ)/SQRT_2_PI)

/************************************************************************/

#define GET_RULE(pSAM, rule_num)(&( (pSAM)->rules[rule_num]))



#endif

