#ifndef FloatBase_H
#define FloatBase_H
#include <cstdint>
#include <bitset>

// abstract class for the Floats
class FloatBase{
public:
   virtual ~FloatBase() {}

   virtual void setNumber(std::bitset<32> bit_number) = 0;
   virtual void setNumber(uint32_t uint_number) = 0;


   virtual FloatBase* addition(FloatBase& other) const = 0;
   virtual FloatBase* subtraction(FloatBase& other) const = 0;
   virtual FloatBase* multiplication(FloatBase& other) const = 0;
   virtual FloatBase* division(FloatBase& other) const = 0;
   virtual float to_float() const = 0;

   virtual uint32_t getFractional() const = 0;
   virtual uint32_t getExponent() const = 0;
   virtual unsigned get_sign_bit() const = 0;

};

#endif // FloatBase_H

