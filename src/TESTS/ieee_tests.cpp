#include "tests.h"

void ieee_op_tests(){
    //IEEE
    std::cout << "IEEE OPERATIONS TESTING: " << std::endl;
    IEEE_754 bit_A(std::bitset<32>(0b01000000000000000000000000000000)); //2
    IEEE_754 bit_B(std::bitset<32>(0b01000010101000000000000000000000)); //80
    IEEE_754 float_A(15.5f);
    IEEE_754 float_B(4.3f);
    IEEE_754 double_A(4.2);
    IEEE_754 double_B(8.6);
    IEEE_754 ieee_add = double_B + bit_B;
    IEEE_754 ieee_sub = float_A - float_B;
    IEEE_754 ieee_mul = double_A * float_B;
    IEEE_754 ieee_div = float_A / float_B;
    IEEE_754 ieee_add2 = double_B + 5.3;
    IEEE_754 ieee_add3 = 5.3 + double_B;

    std::cout << "Addition testing: " << double_B.to_float() << " + " << bit_B.to_float() << " = " << ieee_add.to_float() << std::endl;
    std::cout << "Substarction testing: " << float_A.to_float() << " - " << float_B.to_float() << " = " << ieee_sub.to_float() << std::endl;
    std::cout << "Multiplication testing: " << double_A.to_float() << " * " << float_B.to_float() << " = " << ieee_mul.to_float() << std::endl;
    std::cout << "Division testing: " << float_A.to_float() << " / " << float_B.to_float() << " = " << ieee_div.to_float() << std::endl;
    std::cout << "Addition testing: " << double_B.to_float() << " + 5.3 = " << ieee_add2.to_float() << std::endl;
    std::cout << "Addition testing: " << "5.3 + " << double_B.to_float() << " = " << ieee_add3.to_float() << std::endl;
}

void ieee_cin_cout_tests(){
    std::cout << "IEEE CIN & COUT TESTING: " << std::endl;
    IEEE_754 ie(3.2), ie1(3.2), ie2(3.2);
    std::cout << "Enter first ieee: ";
    std::cin >> ie;
    std::cout << "The ieee you entered is: " << ie << std::endl;
    std::cout << "Enter second ieee: ";
    std::cin >> ie1;
    std::cout << "The ieee you entered is: " << ie1 << std::endl;
    std::cout << "Enter third ieee: ";
    std::cin >> ie2;
    std::cout << "The ieee you entered is: " << ie2 << std::endl;
}

void ieee_bool_ops_tests(){
    std::cout << "IEEE BOOL OPS TESTING: " << std::endl;
    IEEE_754 bit_B(std::bitset<32>(0b01000010101000000000000000000000));
    IEEE_754 double_A(4.2);
    IEEE_754 double_B(8.6);
    IEEE_754 float_C(3.2f);
    IEEE_754 float_D(3.2f);
    bool gt = bit_B > double_B;
    bool lt = bit_B < double_B;
    bool neq = double_A != double_B;
    bool eq = float_C == float_D;
    bool geq = float_C >= float_D;
    bool leq = bit_B <= double_B;
    std::cout << "GT testing: " << bit_B.to_float() << " > " << double_B.to_float() << " = " << gt << std::endl;
    std::cout << "LT testing: " << bit_B.to_float() << " < " << double_B.to_float() << " = " << lt << std::endl;
    std::cout << "NEQ testing: " << double_A.to_float() << " != " << double_B.to_float() << " = " << neq << std::endl;
    std::cout << "EQ testing: " << float_C.to_float() << " == " << float_D.to_float() << " = " << eq << std::endl;
    std::cout << "GEQ testing: " << float_C.to_float() << " >= " << float_D.to_float() << " = " << geq << std::endl;
    std::cout << "GEQ testing: " << bit_B.to_float() << " <= " << double_B.to_float() << " = " << leq << std::endl;
}

void ieee_loop_test(){
    IEEE_754 double_A(4.2);
    IEEE_754 double_B(8.6);
    std::cout << "IEEE LOOP TESTING: " << std::endl;
    IEEE_754 loop_ieee = IEEE_754(0.0);
    for (int i = 0; i < 1000; i++)
    {
        loop_ieee = loop_ieee + double_A + double_B;
    }
    std::cout << loop_ieee << std::endl;
    std::cout << "Number of operations of loop_ieee: " << loop_ieee.get_count_operations() << std::endl;
    for (auto element : loop_ieee.get_all_ops_count())
    {
         std::cout << "Number: loop_ieee; " << "Operation: " << element.first << "; Used: " << element.second << std::endl;
    }
    std::cout << "Time of loop_ieee: " << loop_ieee.get_total_time() << std::endl;
}