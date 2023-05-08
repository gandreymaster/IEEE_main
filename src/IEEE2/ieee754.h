#ifndef IEEE_754_H
#define IEEE_754_H

#include <bitset>
#include <iostream>
#include <cassert>
#include <cmath>
#include <string>
#include <cfenv>
#include <climits>
#include <chrono>
#include <map>

#include "..//Float_Base.h"
#include "..//float_types.h"

class IEEE_754 : public Float_Base
{
    std::bitset<IEEE_N_BITS> number;

public:
    IEEE_754(std::bitset<IEEE_N_BITS> ieee_number = 0);

    IEEE_754(int num);

    IEEE_754(float num);

    IEEE_754(double num);

    Float_Base *Clone()
    {
        return new IEEE_754(*this);
    }

    void set_number(std::bitset<IEEE_N_BITS> ieee_number);
    void set_number(FLOAT_UTYPE uint_number);
    mutable int operations_count = 0;

    mutable std::map<std::string, int> all_operations_count = {
        {"=", 0},
        {"+", 0},
        {"-", 0},
        {"*", 0},
        {"/", 0},
        {"<", 0},
        {"<=", 0},
        {">", 0},
        {">=", 0},
        {"==", 0},
        {"!=", 0}};

    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

    bool is_zero() const;

    int get_count_operations();

    void set_count_operations(int c);

    std::map<std::string, int> get_all_ops_count();

    double get_total_time();

    static unsigned long pow_of_two(int n);

    const std::bitset<IEEE_N_BITS> &get_number() const;

    std::bitset<IEEE_N_BITS> &get_number();

    float to_float() const;

    void flip_sign_bit();

    std::string print() const;

    friend std::ostream &operator<<(std::ostream &os, const IEEE_754 &num);

    friend std::istream &operator>>(std::istream &is, IEEE_754 &num);

    std::bitset<IEEE_EXPONENT_BITS> get_exponent_bits() const;

    std::bitset<IEEE_MANTISSA_BITS> get_mantissa_bits() const;

    std::bitset<IEEE_MANTISSA_BITS> get_mantissa_bits_inv() const;

    unsigned get_sign_bit() const;
    FLOAT_UTYPE get_fractional() const;
    FLOAT_STYPE get_exponent() const;

    FLOAT_UTYPE get_fractional_size() const;
    FLOAT_UTYPE get_exponent_size() const;
    void set_fractional(FLOAT_UTYPE uint_fract);
    void set_exponent(FLOAT_UTYPE uint_exp);

    void set_sign(unsigned sign);
    void set_by_parts(FLOAT_UTYPE frac, FLOAT_STYPE exp, FLOAT_UTYPE sign, FLOAT_UTYPE nbits, FLOAT_UTYPE esbits);

    void invert_bits();

    // rounding methods
    void round_ties_to_even(bool r, bool s);

    void round_ties_to_away(bool r, bool s);

    // template <size_t N1>
    // static std::bitset<N1> additive_inverse(std::bitset<N1> &mantissa);

    IEEE_754 add(IEEE_754 &num2) const;

    IEEE_754 *addition(Float_Base &other) const override;
    // Correct way of operator overload

    IEEE_754 operator+(const IEEE_754 &other_number)
    {
        return *(addition(const_cast<IEEE_754 &>(other_number)));
    }

    IEEE_754 sub(IEEE_754 &num2) const;

    IEEE_754 *subtraction(Float_Base &other) const override;
    // Correct way of operator overload

    IEEE_754 operator-(const IEEE_754 &other_number)
    {
        return *(subtraction(const_cast<IEEE_754 &>(other_number)));
    }

    IEEE_754 mul(IEEE_754 &num2) const;

    IEEE_754 *multiplication(Float_Base &other) const override;
    // Correct way of operator overload

    IEEE_754 operator*(const IEEE_754 &other_number)
    {
        return *(multiplication(const_cast<IEEE_754 &>(other_number)));
    }

    IEEE_754 div(IEEE_754 &num2) const;

    IEEE_754 *division(Float_Base &other) const override;
    // Correct way of operator overload

    IEEE_754 operator/(const IEEE_754 &other_number)
    {
        return *(division(const_cast<IEEE_754 &>(other_number)));
    }

    // TODO: also lt,gt, etc. methods
    bool operator<(const IEEE_754 &other) const;

    bool operator<=(const IEEE_754 &other) const;

    bool operator>(const IEEE_754 &other) const;

    bool operator>=(const IEEE_754 &other) const;

    bool operator==(const IEEE_754 &other) const;

    bool operator!=(const IEEE_754 &other) const;
};

IEEE_754 operator+(const double d, IEEE_754 &p);
IEEE_754 operator-(const double d, IEEE_754 &p);
IEEE_754 operator*(const double d, IEEE_754 &p);
IEEE_754 operator/(const double d, IEEE_754 &p);

#endif /* IEEE_754_H */
