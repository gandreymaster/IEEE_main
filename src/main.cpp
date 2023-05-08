#include "TESTS//tests.h"
#define DEBUG_main

#define NFILE_MAX 100

int main()
{
#ifdef DEBUG_main

    ieee_op_tests();
    ieee_cin_cout_tests();
    ieee_bool_ops_tests();
    ieee_loop_test();

    test_create_fir_cascade_IEEE();
    test_init_fir_cascade_IEEE();
    test_process_cascades_IEEE();

#endif

#ifdef DEBUG_main

    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
    std::cout << "UNITED IEEE TESTS" << std::endl;
    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
    Float_Base *arg01 = new IEEE_754(-77.543);
    Float_Base *arg02 = new IEEE_754(234);
    Float_Base *res0 = new IEEE_754(0);

    Float_Base *arg11 = new IEEE_754(3.0);
    Float_Base *arg12 = new IEEE_754(0);
    Float_Base *res1 = new IEEE_754(0);

    Float_Base *arg21 = new IEEE_754(2222.65);
    Float_Base *arg22 = new IEEE_754(0.0005);
    Float_Base *res2 = new IEEE_754(0);

    Float_Base *arg31 = new IEEE_754(2222.65);
    Float_Base *arg32 = new IEEE_754(4654.564);
    Float_Base *res3 = new IEEE_754(0);

    Float_Base *arg41 = new IEEE_754(0.0003);
    Float_Base *arg42 = new IEEE_754(0.00054);
    Float_Base *res4 = new IEEE_754(0);

    // IEEE_754
    Float_Base *real_res0 = arg01->multiplication(*arg02);
    Float_Base *real_res1 = arg11->multiplication(*arg12);
    Float_Base *real_res2 = arg21->multiplication(*arg22);
    Float_Base *real_res3 = arg31->multiplication(*arg32);
    Float_Base *real_res4 = arg41->multiplication(*arg42);
    std::cout << "Float_Base IEEE_754 Real: " << real_res0->to_float() << std::endl;
    std::cout << "Float_Base IEEE_754 Real: " << real_res1->to_float() << std::endl;
    std::cout << "Float_Base IEEE_754 Real: " << real_res2->to_float() << std::endl;
    std::cout << "Float_Base IEEE_754 Real: " << real_res3->to_float() << std::endl;
    std::cout << "Float_Base IEEE_754 Real: " << real_res4->to_float() << std::endl;


    delete arg01;
    delete arg02;
    delete arg11;
    delete arg12;
    delete arg21;
    delete arg22;
    delete arg31;
    delete arg32;
    delete arg41;
    delete arg42;
    delete res0;
    delete res1;
    delete res2;
    delete res3;
    delete res4;
    delete real_res0;
    delete real_res1;
    delete real_res2;
    delete real_res3;
    delete real_res4;
#endif
}
