#include "FIIR.h"
#include <iostream>
#include "../IEEE2/ieee754.h"

#define N 20 // number of cascades

int getLast(float *w, float *q, int n, float start, float *wl, float *ql)
{
    float st0 = start;
    float st1 = 0;
    for (int i = 0; i < n; i++)
    {
        float tmp = st0;
        st0 = st0 + w[i] * st0 - q[i] * st1;
        st1 = tmp;
    }

    *wl = 0.5f;
    *ql = (st0 + (*wl) * st1 - 1) / st0;

    return 0;
}

Float_Base *cascade_fir(Float_Base *x, FIRCascadeState *p)
{
    Float_Base *r = x->Clone();

    Float_Base *r1 = p->state[1]->multiplication(*p->mult_const[1]);
    Float_Base *r2 = p->state[0]->multiplication(*p->mult_const[0]);

    r = r->addition(*r1);
    r = r->subtraction(*r2);

    p->state[1] = p->state[0];
    p->state[0] = r;

    return r;
}

void create_fir_cascade(Float_Base *c0,
                        Float_Base *c1,
                        Float_Base *init,
                        FIRCascadeState *p)
{
    p->mult_const[0] = c0;
    p->mult_const[1] = c1;
    p->state[0] = init;
    p->state[1] = init;
}

void init_fir_cascade(Float_Base **w, Float_Base **q, int n, Float_Base *init, FIRCascadeState *stage)
{
    for (int i = 0; i < n; i++)
    {
        create_fir_cascade(w[i], q[i], init, &stage[i]);
    }
}

// TODO: int conversions
Float_Base *process_cascades(Float_Base *initFloat, int n, FIRCascadeState *stage)
{
    Float_Base *v;
    // static Float_Base * d;

    v = cascade_fir(initFloat, &stage[0]);
    for (int i = 1; i < n; i++)
    {
        stage[i].state[0] = stage[i - 1].state[0];
        stage[i].state[1] = stage[i - 1].state[1];
        v = cascade_fir(v, &stage[i]);
    }

    // d = initFloat;

    return v;
}
