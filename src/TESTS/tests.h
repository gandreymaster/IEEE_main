#include "..//IEEE2//ieee754.h"
#include "..//FIR//FIIR.h"
#include <fstream>
#include <chrono>
#define ARRAY_SIZE 100000

void ieee_op_tests();
void ieee_cin_cout_tests();
void ieee_bool_ops_tests();
void ieee_loop_test();

void test_create_fir_cascade_IEEE();
void test_init_fir_cascade_IEEE();
void test_process_cascades_IEEE();
