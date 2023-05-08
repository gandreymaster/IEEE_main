#include "tests.h"
#include <iostream>

void test_create_fir_cascade_IEEE()
{
    IEEE_754 c0(0.5f);
    IEEE_754 c1(0.3f);
    IEEE_754 zero(0.0f);
    FIRCascadeState *state = new FIRCascadeState();
    create_fir_cascade(&c0, &c1, &zero, state);

    std::cout << "Test 1: test create_fir_cascade function" << std::endl;
    std::cout << "mult_const[0]: expected " << c0.to_float() << ", actual " << state->mult_const[0]->to_float() << std::endl;
    std::cout << "mult_const[1]: expected " << c1.to_float() << ", actual " << state->mult_const[1]->to_float() << std::endl;
    std::cout << "state[0]: expected " << 0.0f << ", actual " << state->state[0]->to_float() << std::endl;
    std::cout << "state[1]: expected " << 0.0f << ", actual " << state->state[1]->to_float() << std::endl;
    delete state;

    FIRCascadeState *state2 = new FIRCascadeState();
    IEEE_754 c00(0.75f);
    IEEE_754 c10(0.23f);
    create_fir_cascade(&c00, &c10, &zero, state2);

    std::cout << "Test 1: test create_fir_cascade function" << std::endl;
    std::cout << "mult_const[0]: expected " << c00.to_float() << ", actual " << state2->mult_const[0]->to_float() << std::endl;
    std::cout << "mult_const[1]: expected " << c10.to_float() << ", actual " << state2->mult_const[1]->to_float() << std::endl;
    std::cout << "state[0]: expected " << 0.0f << ", actual " << state2->state[0]->to_float() << std::endl;
    std::cout << "state[1]: expected " << 0.0f << ", actual " << state2->state[1]->to_float() << std::endl;
    delete state2;
}

void test_init_fir_cascade_IEEE()
{
    std::cout << "Test 2: test init_fir_cascade function 1 " << std::endl;
    const int n = 3;
    std::cout << "Test 2: test init_fir_cascade function 2 " << std::endl;
    Float_Base *w[n] = {new IEEE_754(0.5f), new IEEE_754(0.3f), new IEEE_754(0.1f)};
    std::cout << "Test 2: test init_fir_cascade function 3 " << std::endl;
    Float_Base *q[n] = {new IEEE_754(0.2f), new IEEE_754(0.4f), new IEEE_754(0.6f)};
    std::cout << "Test 2: test init_fir_cascade function 4 " << std::endl;

    IEEE_754 zero(0.0f);

    FIRCascadeState stage[3]; // = new FIRCascadeState[3];
    /*for (int i = 0; i < n; i++) {
        stage[i] = new FIRCascadeState();
    }*/
    init_fir_cascade(w, q, n, &zero, stage);

    std::cout << "Test 2: test init_fir_cascade function FINAL " << std::endl;
    for (int i = 0; i < n; i++)
    {
        FIRCascadeState *state = &stage[i];
        std::cout << "Cascade " << i << std::endl;
        std::cout << "mult_const[0]: " << state->mult_const[0]->to_float() << std::endl;
        std::cout << "mult_const[1]: " << state->mult_const[1]->to_float() << std::endl;
        std::cout << "state[0]: " << state->state[0]->to_float() << std::endl;
        std::cout << "state[1]: " << state->state[1]->to_float() << std::endl;
    }
    for (int i = 0; i < n; i++)
    {
        delete w[i];
        delete q[i];
    }
}

void test_process_cascades_IEEE()
{
    std::cout << "Test 3" << std::endl;

    Float_Base *input[4] = {new IEEE_754(1.0f), new IEEE_754(2.0f), new IEEE_754(3.0f), new IEEE_754(4.0f)};
    FIRCascadeState *stage = new FIRCascadeState[3];
    IEEE_754 zero(0.0f);

    const int n = 3;
    Float_Base *w[n] = {new IEEE_754(0.5f), new IEEE_754(0.8f), new IEEE_754(0.5f)};
    Float_Base *q[n] = {new IEEE_754(0.2f), new IEEE_754(0.2f), new IEEE_754(0.9f)};
    std::cout << "FIR cascade init!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;

    for (int j = 0; j < 4; j++)
    {
        std::cout << "FIR cascade processed" << std::endl;

        init_fir_cascade(w, q, n, &zero, stage);

        IEEE_754 *output = dynamic_cast<IEEE_754 *>(process_cascades(input[j], n, stage));

        std::cout << "FIR cascade done" << std::endl;

        std::cout << "output: " << output->to_float() << ";Should be " << input[j]->to_float() << std::endl;
    }
    for (int i = 0; i < n; i++)
    {
        delete w[i];
        delete q[i];
        delete input[i];
    }
    delete[] stage;
}
