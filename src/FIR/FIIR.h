#ifndef FIIR_H
#define FIIR_H


#include <stdio.h>
#include "..//Float_Base.h"

#define N 20 // number of cascades

struct FIRCascadeState {
    Float_Base* state[2];
    Float_Base* mult_const[2];
};

struct Coded_number{
    uint32_t exp;
    uint32_t mult_s[8];
    uint32_t mult_m[8];
    uint32_t mult_q[8];
};


struct FIRCascadeStateOpt {
    uint32_t state_mant[2];
    uint32_t state_exp[2];
    uint32_t state_sign[2];

    uint32_t mult_exp[2];

    uint32_t mult_s[2][8];
    uint32_t mult_m[2][8];
    uint32_t mult_q[2][8];
};

struct FIRCascadeStateOptFpag {
    uint32_t state_mant[2];
    uint32_t state_exp[2];
    uint32_t state_sign[2];

    uint32_t mult_exp[2];

    uint32_t mult_op[2][8];
    uint32_t mult_op1[2][8];
    uint32_t mult_op2[2][8];
};

Float_Base* cascade_fir(Float_Base* x, FIRCascadeState* p);
//TODO;
Coded_number cascade_fir_opt(Coded_number* x, FIRCascadeStateOpt* p);

void create_fir_cascade(Float_Base* c0,
                   Float_Base* c1,
                   Float_Base* init,
                   FIRCascadeState *p) ;

void create_fir_cascade_opt(Coded_number c0,
                   Coded_number c1,
                   Coded_number init,
                   FIRCascadeStateOpt *p) ;


//extern FIRCascadeState* stage[N];

void init_fir_cascade(Float_Base **w, Float_Base **q, int n, Float_Base* init, FIRCascadeState* stage);

void init_fir_cascade_opt(Coded_number *w, Coded_number *q, int n, Coded_number init, FIRCascadeStateOpt* stage);


//TODO: int conversions
Float_Base * process_cascades(Float_Base *initFloat, int n, FIRCascadeState* stage);

Coded_number process_cascades_opt(Coded_number initFloat, int n, FIRCascadeState* stage);


int getLast(float* w, float* q, int n, float start, float* wl, float* ql);
/////////////////////////////////////////////////////////
/// IIR

struct IIRCascadeState {
    Float_Base* state[2];   // state
    Float_Base* b[3];  // nominator coeff  b0 b1 b2
    Float_Base* a[2];  // denominator coeff   a1 a2
};

Float_Base* cascade_iir(Float_Base* x, IIRCascadeState *p) ;

Float_Base* cascade_iir_T(Float_Base* x, IIRCascadeState *p) ;

void create_cascade_iir(Float_Base* b0,
                   Float_Base* b1,
                   Float_Base* b2,
                   Float_Base* a1,
                   Float_Base* a2,
                   IIRCascadeState *p) ;

void init_iir_cascade() ;

Float_Base* process_cascade_iir(Float_Base* x) ;
Float_Base* process_cascade_iir_T(Float_Base* x);

#endif // FIIR_H

