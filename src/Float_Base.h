#ifndef Float_Base_H
#define Float_Base_H
#include <cstdint>
#include <bitset>
#include "float_types.h"

// abstract class for the Floats
class Float_Base{
public:
   virtual ~Float_Base() {};
   virtual Float_Base *Clone() = 0;

   virtual void set_number(std::bitset<32> bit_number) = 0;
   virtual void set_number(FLOAT_UTYPE uint_number) = 0;


   virtual Float_Base* addition(Float_Base& other) const = 0;
   virtual Float_Base* subtraction(Float_Base& other) const = 0;
   virtual Float_Base* multiplication(Float_Base& other) const = 0;
   virtual Float_Base* division(Float_Base& other) const = 0;
   virtual float to_float() const = 0;

   virtual FLOAT_UTYPE get_fractional() const = 0;
   virtual FLOAT_STYPE get_exponent() const = 0;
   virtual unsigned get_sign_bit() const = 0;
   virtual bool is_zero() const = 0;

   virtual FLOAT_UTYPE get_fractional_size() const = 0;
   virtual FLOAT_UTYPE get_exponent_size() const = 0;

   virtual void set_by_parts(FLOAT_UTYPE frac, FLOAT_STYPE exp, FLOAT_UTYPE sign, FLOAT_UTYPE nbits, FLOAT_UTYPE esbits) = 0;
   virtual void invert_bits() = 0;
   //virtual Float_Base* generateRandomNumbersPrec(int size, int decimals) const = 0;

};

#endif // Float_Base_H

