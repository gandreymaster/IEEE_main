#include "ieee754.h"

#define ADDITIVE_INVERSE(N1, mantissa)                               \
    do                                                               \
    {                                                                \
        /* Flip all bits in the mantissa */                          \
        mantissa.flip();                                             \
        /* Convert the mantissa to an unsigned long integer */       \
        unsigned long ulong_mantissa = mantissa.to_ulong();          \
        /* Add one to the mantissa to obtaien the additive inverse */ \
        ulong_mantissa += 1;                                         \
        return std::bitset<N1>(ulong_mantissa);                      \
    } while (0)

#define SCALE_MANTISSA_DOWN(N1, exponent_diff, mantissa)          \
    do                                                            \
    {                                                             \
        /* Shift the mantissa right by the exponent difference */ \
        mantissa >>= exponent_diff;                               \
    } while (0)

#define PASTE_BITS_INTO(N1, N2, src, dst, dst_start_index, dst_end_index) \
    do                                                                    \
    {                                                                     \
        /* Check the sizes of src and dst */                              \
        assert(N1 <= N2);                                                 \
        /* Check that the destination indices are valid */                \
        assert(dst_start_index <= dst_end_index);                         \
        assert(dst_start_index >= 0 && dst_end_index >= 0);               \
        /* Check that the number of bits to paste is correct */           \
        assert(N1 == dst_end_index - dst_start_index + 1);                \
        /* Copy the bits from src to dst */                               \
        for (int i = 0; i <= (dst_end_index - dst_start_index); i++)      \
        {                                                                 \
            dst[dst_start_index + i] = src[i];                            \
        }                                                                 \
    } while (0)

#define PASTE_BITS_INTO2(N1, N2, src, dst, dst_start_index, dst_end_index, src_start_index, src_end_index) \
    do                                                                                                     \
    {                                                                                                      \
        assert(N1 <= N2);                                                                                  \
        assert(dst_start_index <= dst_end_index);                                                          \
        assert(src_start_index <= src_end_index);                                                          \
        assert(dst_start_index >= 0 && dst_end_index >= 0 && src_start_index >= 0 && src_end_index >= 0);  \
        assert(dst_end_index - dst_start_index == src_end_index - src_start_index);                        \
        for (int i = 0; i <= (dst_end_index - dst_start_index); i++)                                       \
        {                                                                                                  \
            dst[dst_start_index + i] = src[src_start_index + i];                                           \
        }                                                                                                  \
    } while (0)

std::chrono::duration<double> totalTime(0);

IEEE_754::IEEE_754(std::bitset<IEEE_N_BITS> ieee_number /* 0 */) : number(ieee_number)
{
    start_time = std::chrono::high_resolution_clock::now();
    operations_count++;
}

IEEE_754::IEEE_754(int num)
{
    start_time = std::chrono::high_resolution_clock::now();
    operations_count++;
    float n = (float)num;
    std::bitset<sizeof(float) * CHAR_BIT> res(*reinterpret_cast<unsigned long *>(&n));
    number = res;
}

IEEE_754::IEEE_754(float num)
{
    start_time = std::chrono::high_resolution_clock::now();
    operations_count++;
    std::bitset<sizeof(float) * CHAR_BIT> res(*reinterpret_cast<unsigned long *>(&num));
    number = res;
}

IEEE_754::IEEE_754(double num)
{
    start_time = std::chrono::high_resolution_clock::now();
    operations_count++;
    float n = (float)num;
    std::bitset<sizeof(float) * CHAR_BIT> res(*reinterpret_cast<unsigned long *>(&n));
    number = res;
}

bool IEEE_754::is_zero() const
{
    return to_float() == 0;
}

int IEEE_754::get_count_operations()
{
    return operations_count;
}

void IEEE_754::set_count_operations(int c)
{
    operations_count = c;
}

std::map<std::string, int> IEEE_754::get_all_ops_count()
{
    return all_operations_count;
}

double IEEE_754::get_total_time()
{
    return totalTime.count();
}

unsigned long IEEE_754::pow_of_two(int n)
{
    return 1UL << n;
}

unsigned IEEE_754::get_sign_bit() const
{
    return number[IEEE_N_BITS - 1];
}

void IEEE_754::flip_sign_bit()
{
    number[IEEE_N_BITS - 1] = ~number[IEEE_N_BITS - 1];
}

void IEEE_754::invert_bits()
{
    number = number;
}

float IEEE_754::to_float() const
{
    unsigned int x = static_cast<unsigned int>(number.to_ulong());
    float y = *(float *)&x;
    return y;
}

const std::bitset<IEEE_N_BITS> &IEEE_754::get_number() const
{
    return number;
}

std::bitset<IEEE_N_BITS> &IEEE_754::get_number()
{
    return number;
}

void IEEE_754::round_ties_to_even(bool r, bool s)
{
    // in this round method there is no need to check sign

    bool unity_bit = this->number[0];

    if (unity_bit == 0 && r == 1 && s == 1)
        unity_bit = 1;
    else if (unity_bit == 1 && r == 1)
    {
        unity_bit = 0;
        this->number[1] = 1;
    }

    this->number[0] = unity_bit;
}

void IEEE_754::round_ties_to_away(bool r, bool s)
{
    // in this round method there is no need to check sign

    bool unity_bit = this->number[0];

    if (unity_bit == 0 && (r == 1 || s == 1))
        unity_bit = 1;
    else if (unity_bit == 1 && (r == 1 || s == 1))
    {
        unity_bit = 0;
        this->number[1] = 1;
    }

    this->number[0] = unity_bit;
}

std::string IEEE_754::print() const
{

    int sign = number[IEEE_N_BITS - 1];

    int base = 1;

    auto exponent = this->get_exponent_bits().to_ulong();

    auto mantissa = this->get_mantissa_bits();

    if (exponent == 0 && mantissa.to_ulong() == 0)
    {
        std::cout << "0" << std::endl;
        return "0";
    }
    else if (exponent == 0 && mantissa.to_ulong() != 0)
    {
        exponent = 1;
        base = 0;
    }
    else if (exponent == 255 && mantissa.to_ulong() != 0)
    {
        std::cout << "NaN" << std::endl;
        return "NaN";
    }
    else if (exponent == 255 && mantissa.to_ulong() == 0)
    {
        if (sign == 1)
        {
            std::cout << "-Inf" << std::endl;
            return "-Inf";
        }
        else
        {
            std::cout << "+Inf" << std::endl;
            return "+Inf";
        }
    }

    double value = 0;
    double x = 0.5;
    for (int i = IEEE_MANTISSA_BITS - 1; i >= 0; i--)
    {
        value += x * mantissa[i];
        x /= 2;
    }

    std::cout << "Sign: " << (sign ? "-1" : "1") << "; Exponent: "
              << "2^" << (int)(exponent - IEEE_754::pow_of_two(IEEE_EXPONENT_BITS - 1) + 1) << "; Mantissa: " << base + value << std::endl;
    return "";
}

std::ostream &operator<<(std::ostream &os, const IEEE_754 &num)
{
    os << "IEEE representation: " << num.print() << "; Float representation: " << num.to_float() << ";\n Binary representation: " << num.get_sign_bit() << " " << num.get_exponent_bits() << " " << num.get_mantissa_bits();
    return os;
}

// Input stream overload
std::istream &operator>>(std::istream &is, IEEE_754 &num)
{
    int choice;
    std::cout << "Enter 1, 2 or 3 to choose which way you'd prefer to enter a POSIT number" << std::endl;
    std::cout << "1: Binary representation; 2: Float number; 3: Double number; Anything else: Double number" << std::endl;
    is >> choice;
    if (choice == 1)
    {
        std::cout << "Enter binary number:" << std::endl;
        std::bitset<IEEE_N_BITS> input;
        if (is >> input)
        {
            num = IEEE_754(input);
        }
    }
    else if (choice == 2)
    {
        float f;
        std::cout << "Enter float number:" << std::endl;
        is >> f;
        num = IEEE_754(f);
        return is;
    }
    else if (choice == 3)
    {
        double d;
        std::cout << "Enter double number:" << std::endl;
        is >> d;
        num = IEEE_754(d);
        return is;
    }
    else
    {
        double d;
        std::cout << "Enter double number:" << std::endl;
        is >> d;
        num = IEEE_754(d);
        return is;
    }
    return is;
}

std::bitset<IEEE_EXPONENT_BITS> IEEE_754::get_exponent_bits() const
{
    std::bitset<IEEE_EXPONENT_BITS> exponent;

    for (size_t i = 0; i < IEEE_EXPONENT_BITS; i++)
    {
        exponent[i] = this->number[IEEE_MANTISSA_BITS + i];
    }

    return exponent;
}

std::bitset<IEEE_MANTISSA_BITS> IEEE_754::get_mantissa_bits() const
{

    std::bitset<IEEE_MANTISSA_BITS> mantissa;

    for (size_t i = 0; i < IEEE_MANTISSA_BITS; i++)
    {
        mantissa[i] = this->number[i];
    }

    return mantissa;
}

std::bitset<IEEE_MANTISSA_BITS> IEEE_754::get_mantissa_bits_inv() const
{

    std::bitset<IEEE_MANTISSA_BITS> mantissa;

    for (size_t i = 0; i < IEEE_MANTISSA_BITS; i++)
    {
        mantissa[IEEE_MANTISSA_BITS - i - 1] = this->number[i];
    }

    return mantissa;
}

FLOAT_UTYPE IEEE_754::get_fractional() const
{
    std::bitset<IEEE_MANTISSA_BITS> mantissa;

    for (size_t i = 0; i < IEEE_MANTISSA_BITS; i++)
    {
        mantissa[i] = this->number[i];
    }

    return mantissa.to_ulong();
}

FLOAT_STYPE IEEE_754::get_exponent() const
{
    std::bitset<IEEE_EXPONENT_BITS> exponent;

    for (size_t i = 0; i < IEEE_EXPONENT_BITS; i++)
    {
        exponent[i] = this->number[IEEE_MANTISSA_BITS + i];
    }

    return exponent.to_ulong();
}
FLOAT_UTYPE IEEE_754::get_fractional_size() const
{
    return 23;
}
FLOAT_UTYPE IEEE_754::get_exponent_size() const
{
    return 8;
}

void IEEE_754::set_number(std::bitset<IEEE_N_BITS> ieee_number)
{
    number = ieee_number;
}

void IEEE_754::set_number(FLOAT_UTYPE uint_number)
{
    number = std::bitset<32>(uint_number);
}

void IEEE_754::set_fractional(FLOAT_UTYPE uint_fract)
{
    number >>= 23;
    number <<= 23;
    number |= uint_fract;
}
void IEEE_754::set_exponent(FLOAT_UTYPE uint_exp)
{
    FLOAT_UTYPE mask = (1U << 31) - (1 << 23);
    number &= ~mask;
    number |= (uint_exp << 23);
}

void IEEE_754::set_sign(unsigned sign)
{
    number &= ~(1U << 31);
    number |= (sign << 31);
}
void IEEE_754::set_by_parts(FLOAT_UTYPE frac, FLOAT_STYPE exp, FLOAT_UTYPE sign, FLOAT_UTYPE nbits, FLOAT_UTYPE esbits)
{
    if (frac == 0 && exp == 0 && sign == 0)
    {
        number = std::bitset<32>(0);
    }
    else
    {
        exp = exp - 127;
        number = std::bitset<32>(std::to_string(sign) + std::bitset<8>(exp).to_string() + std::bitset<23>(frac).to_string());
    }
}

IEEE_754 *IEEE_754::addition(Float_Base &other) const
{
    IEEE_754 *b = dynamic_cast<IEEE_754 *>(&other); // try to cast
    if (b != nullptr)
    { // if successful, add values
        return new IEEE_754(this->add(*b));
    }
    else
    { // otherwise, return nullptr indicating the addition is not possible
        return nullptr;
    }
}

IEEE_754 *IEEE_754::subtraction(Float_Base &other) const
{
    IEEE_754 *b = dynamic_cast<IEEE_754 *>(&other); // try to cast
    if (b != nullptr)
    { // if successful, add values
        return new IEEE_754(this->sub(*b));
    }
    else
    { // otherwise, return nullptr indicating the addition is not possible
        return nullptr;
    }
}

IEEE_754 *IEEE_754::multiplication(Float_Base &other) const
{
    IEEE_754 *b = dynamic_cast<IEEE_754 *>(&other); // try to cast
    if (b != nullptr)
    { // if successful, add values
        return new IEEE_754(this->mul(*b));
    }
    else
    { // otherwise, return nullptr indicating the addition is not possible
        return nullptr;
    }
}

IEEE_754 *IEEE_754::division(Float_Base &other) const
{
    IEEE_754 *b = dynamic_cast<IEEE_754 *>(&other); // try to cast
    if (b != nullptr)
    { // if successful, add values
        return new IEEE_754(this->div(*b));
    }
    else
    { // otherwise, return nullptr indicating the addition is not possible
        return nullptr;
    }
}

IEEE_754 IEEE_754::add(IEEE_754 &num2) const
{
    operations_count++;
    num2.operations_count++;
    all_operations_count["+"]++;
    num2.all_operations_count["+"]++;
    // TODO: sign = 1 -> negative numbers

    int sign1 = get_sign_bit();
    int sign2 = num2.get_sign_bit();

    std::bitset<IEEE_MANTISSA_BITS> mantissa1 = get_mantissa_bits();
    std::bitset<IEEE_MANTISSA_BITS> mantissa2 = num2.get_mantissa_bits();

    unsigned long exponent1_ulong = get_exponent_bits().to_ulong();
    unsigned long exponent2_ulong = num2.get_exponent_bits().to_ulong();

    bool num1_is_denormalized = (exponent1_ulong == 0) ? true : false;
    bool num2_is_denormalized = (exponent2_ulong == 0) ? true : false;

    unsigned long max_exponent = IEEE_754::pow_of_two(IEEE_EXPONENT_BITS) - 1;

    // NaN
    if (exponent1_ulong == max_exponent && mantissa1.to_ulong() != 0)
    {
        return *(this);
    }

    if (exponent2_ulong == max_exponent && mantissa2.to_ulong() != 0)
    {
        return num2;
    }

    if (exponent1_ulong == max_exponent && mantissa1.to_ulong() == 0)
    {
        if (exponent2_ulong == max_exponent && mantissa2.to_ulong() == 0 && sign1 != sign2)
        {
            return IEEE_754(std::bitset<IEEE_N_BITS>(0b01111111110000000000000000000000));
        }
        return *(this);
    }

    if (exponent2_ulong == max_exponent && mantissa2.to_ulong() == 0)
    {
        return num2;
    }

    if (num1_is_denormalized)
    {
        exponent1_ulong = 1;
    }

    if (num2_is_denormalized)
    {
        exponent2_ulong = 1;
    }

    // adding 1 as first bit as all numbers start with 1
    std::bitset<IEEE_MANTISSA_BITS + 1> _mantissa1(mantissa1.to_ulong());
    std::bitset<IEEE_MANTISSA_BITS + 1> _mantissa2(mantissa2.to_ulong());

    if (num1_is_denormalized)
    {
        _mantissa1[IEEE_MANTISSA_BITS] = 0;
    }
    else
    {
        _mantissa1[IEEE_MANTISSA_BITS] = 1;
    }

    if (num2_is_denormalized)
    {
        _mantissa2[IEEE_MANTISSA_BITS] = 0;
    }
    else
    {
        _mantissa2[IEEE_MANTISSA_BITS] = 1;
    }

    unsigned long exponent_result = exponent1_ulong;

    if (exponent1_ulong > exponent2_ulong)
    {
        SCALE_MANTISSA_DOWN(IEEE_MANTISSA_BITS + 1, exponent1_ulong - exponent2_ulong, _mantissa2);
        exponent_result = exponent1_ulong;
    }
    else if (exponent1_ulong < exponent2_ulong)
    {
        SCALE_MANTISSA_DOWN(IEEE_MANTISSA_BITS + 1, exponent2_ulong - exponent1_ulong, _mantissa1);
        exponent_result = exponent2_ulong;
    }

    int mantissa1_signed = (sign1 == 0) ? static_cast<int>(_mantissa1.to_ulong()) : static_cast<int>(_mantissa1.to_ulong() * (-1));
    int mantissa2_signed = (sign2 == 0) ? static_cast<int>(_mantissa2.to_ulong()) : static_cast<int>(_mantissa2.to_ulong() * (-1));

    int mantissa_add_result = mantissa1_signed + mantissa2_signed;

    // 25 bit bitset
    std::bitset<IEEE_MANTISSA_BITS + 2> mantissa_result;

    bool result_is_negative = false;

    if (mantissa_add_result >= 0)
    {
        mantissa_result = (unsigned long)mantissa_add_result;
    }
    else
    {
        mantissa_add_result *= (-1);
        mantissa_result = (unsigned long)mantissa_add_result;
        result_is_negative = true;
    }

    // TODO Round the result
    if (mantissa_result[IEEE_MANTISSA_BITS + 1] == 1)
    {
        exponent_result += 1;
        SCALE_MANTISSA_DOWN(IEEE_MANTISSA_BITS + 1, 1, mantissa_result);
    }

    while (mantissa_result[IEEE_MANTISSA_BITS] == 0 && exponent_result > 0)
    {
        exponent_result -= 1;

        if (exponent_result != 0)
            mantissa_result <<= 1;
    }

    // > max
    if (exponent_result >= max_exponent)
    {
        exponent_result = max_exponent;
        mantissa_result = 0;
    }

    IEEE_754 result;
    result.get_number()[IEEE_N_BITS - 1] = result_is_negative;
    PASTE_BITS_INTO2(IEEE_MANTISSA_BITS + 1, IEEE_N_BITS, mantissa_result, result.number, 0, IEEE_MANTISSA_BITS - 1, 0, IEEE_MANTISSA_BITS - 1);
    PASTE_BITS_INTO(IEEE_EXPONENT_BITS, IEEE_N_BITS, std::bitset<IEEE_EXPONENT_BITS>(exponent_result), result.number, IEEE_MANTISSA_BITS, IEEE_MANTISSA_BITS + IEEE_EXPONENT_BITS - 1);

    totalTime += std::chrono::high_resolution_clock::now() - start_time;

    return result;
}

IEEE_754 IEEE_754::sub(IEEE_754 &num2) const
{
    operations_count++;
    num2.operations_count++;
    all_operations_count["-"]++;
    num2.all_operations_count["-"]++;
    IEEE_754 opposite_sign_num2 = num2;
    opposite_sign_num2.flip_sign_bit();
    totalTime += std::chrono::high_resolution_clock::now() - start_time;
    return (add(opposite_sign_num2));
}

IEEE_754 IEEE_754::mul(IEEE_754 &num2) const
{
    operations_count++;
    num2.operations_count++;
    all_operations_count["*"]++;
    num2.all_operations_count["*"]++;

    int sign1 = get_sign_bit();
    int sign2 = num2.get_sign_bit();

    std::bitset<IEEE_MANTISSA_BITS> mantissa1 = get_mantissa_bits();
    std::bitset<IEEE_MANTISSA_BITS> mantissa2 = num2.get_mantissa_bits();

    unsigned long exponent1_ulong = get_exponent_bits().to_ulong();
    unsigned long exponent2_ulong = num2.get_exponent_bits().to_ulong();

    bool num1_is_denormalized = (exponent1_ulong == 0) ? true : false;
    bool num2_is_denormalized = (exponent2_ulong == 0) ? true : false;

    unsigned long max_exponent = IEEE_754::pow_of_two(IEEE_EXPONENT_BITS) - 1;

    // NaN
    if (exponent1_ulong == max_exponent && mantissa1.to_ulong() != 0)
    {
        return *(this);
    }

    if (exponent2_ulong == max_exponent && mantissa2.to_ulong() != 0)
    {
        return num2;
    }

    // infinity
    if (exponent1_ulong == max_exponent && mantissa1.to_ulong() == 0)
    {
        // inf *inf
        if (exponent2_ulong == max_exponent && mantissa2.to_ulong() == 0)
        {
            // inf * (-inf) = -inf
            if (sign1 != sign2)
                return IEEE_754(std::bitset<IEEE_N_BITS>(0b11111111100000000000000000000000));
            // inf_same_sign * inf_same_sign = +inf
            else
                return IEEE_754(std::bitset<IEEE_N_BITS>(0b01111111100000000000000000000000));
        }
        // inf * number
        else if (sign1 != sign2)
        {
            // inf * number_different_sign
            return IEEE_754(std::bitset<IEEE_N_BITS>(0b11111111100000000000000000000000));
        }
        // inf * number_same_sign
        return *(this);
    }

    if (exponent2_ulong == max_exponent && mantissa2.to_ulong() == 0)
    {
        if (sign1 != sign2)
        {
            // inf * number_different_sign
            return IEEE_754(std::bitset<IEEE_N_BITS>(0b11111111100000000000000000000000));
        }
        // inf * number_same_sign
        return num2;
    }

    // numb * zero = zero
    if ((exponent2_ulong == 0 && mantissa2.to_ulong() == 0) || (exponent1_ulong == 0 && mantissa1.to_ulong() == 0))
    {
        return IEEE_754(std::bitset<IEEE_N_BITS>(0b00000000000000000000000000000000));
    }

    if (num1_is_denormalized)
    {
        exponent1_ulong = 1;
    }

    if (num2_is_denormalized)
    {
        exponent2_ulong = 1;
    }

    std::bitset<IEEE_MANTISSA_BITS + 1> _mantissa1(mantissa1.to_ulong());
    std::bitset<IEEE_MANTISSA_BITS + 1> _mantissa2(mantissa2.to_ulong());

    if (num1_is_denormalized)
    {
        _mantissa1[IEEE_MANTISSA_BITS] = 0;
    }
    else
    {
        _mantissa1[IEEE_MANTISSA_BITS] = 1;
    }

    if (num2_is_denormalized)
    {
        _mantissa2[IEEE_MANTISSA_BITS] = 0;
    }
    else
    {
        _mantissa2[IEEE_MANTISSA_BITS] = 1;
    }

    long load = IEEE_754::pow_of_two(IEEE_EXPONENT_BITS - 1) - 1;

    long exponent_result = exponent1_ulong + exponent2_ulong - load;

    if (exponent_result < (-1) * IEEE_MANTISSA_BITS)
        return IEEE_754(std::bitset<IEEE_N_BITS>(0b00000000000000000000000000000000));

    unsigned long long mantissa_multiply_result = _mantissa1.to_ullong() * _mantissa2.to_ullong();

    mantissa_multiply_result /= pow_of_two(IEEE_MANTISSA_BITS);

    std::bitset<IEEE_MANTISSA_BITS + 2> mantissa_result;

    mantissa_result = (unsigned long)mantissa_multiply_result;

    while (exponent_result <= 0 && mantissa_result.to_ulong() != 0)
    {
        exponent_result++;
        mantissa_result >>= 1;
    }

    // TODO Round the result
    if (mantissa_result[IEEE_MANTISSA_BITS + 1] == 1)
    {
        exponent_result += 1;
        SCALE_MANTISSA_DOWN(IEEE_MANTISSA_BITS + 1, 1, mantissa_result);
    }

    while (mantissa_result[IEEE_MANTISSA_BITS] == 0 && exponent_result > 0)
    {
        exponent_result -= 1;
        if (exponent_result != 0)
            mantissa_result <<= 1;
    }

    // > max
    if (exponent_result >= static_cast<long>(max_exponent))
    {
        exponent_result = max_exponent;
        mantissa_result = 0;
    }

    IEEE_754 result;
    result.get_number()[IEEE_N_BITS - 1] = (sign1 != sign2);
    PASTE_BITS_INTO2(IEEE_MANTISSA_BITS + 1, IEEE_N_BITS, mantissa_result, result.number, 0, IEEE_MANTISSA_BITS - 1, 0, IEEE_MANTISSA_BITS - 1);
    PASTE_BITS_INTO(IEEE_EXPONENT_BITS, IEEE_N_BITS, std::bitset<IEEE_EXPONENT_BITS>(exponent_result), result.number, IEEE_MANTISSA_BITS, IEEE_MANTISSA_BITS + IEEE_EXPONENT_BITS - 1);
    totalTime += std::chrono::high_resolution_clock::now() - start_time;
    return result;
}

IEEE_754 IEEE_754::div(IEEE_754 &num2) const
{
    operations_count++;
    num2.operations_count++;
    all_operations_count["/"]++;
    num2.all_operations_count["/"]++;

    int sign1 = get_sign_bit();
    int sign2 = num2.get_sign_bit();

    std::bitset<IEEE_MANTISSA_BITS> mantissa1 = get_mantissa_bits();
    std::bitset<IEEE_MANTISSA_BITS> mantissa2 = num2.get_mantissa_bits();

    unsigned long exponent1_ulong = get_exponent_bits().to_ulong();
    unsigned long exponent2_ulong = num2.get_exponent_bits().to_ulong();

    bool num1_is_denormalized = (exponent1_ulong == 0) ? true : false;
    bool num2_is_denormalized = (exponent2_ulong == 0) ? true : false;

    unsigned long max_exponent = IEEE_754::pow_of_two(IEEE_EXPONENT_BITS) - 1;

    // NaN
    if (exponent1_ulong == max_exponent && mantissa1.to_ulong() != 0)
    {
        return *(this);
    }

    if (exponent2_ulong == max_exponent && mantissa2.to_ulong() != 0)
    {
        return num2;
    }

    // infinity
    if (exponent1_ulong == max_exponent && mantissa1.to_ulong() == 0)
    {
        // inf /inf = nan
        if (exponent2_ulong == max_exponent && mantissa2.to_ulong() == 0)
        {
            return IEEE_754(std::bitset<IEEE_N_BITS>(0b01111111110000000000000000000000));
        }
        // inf / number
        else if (sign1 != sign2)
        {
            // inf / number_different_sign
            return IEEE_754(std::bitset<IEEE_N_BITS>(0b11111111100000000000000000000000));
        }
        // inf / number_same_sign
        return *(this);
    }

    // numb / zero = nan
    if (exponent2_ulong == 0 && mantissa2.to_ulong() == 0)
    {
        return IEEE_754(std::bitset<32>(0b11111111100001100000000000000000));
    }

    // zero / numb = zero
    if (exponent1_ulong == 0 && mantissa1.to_ulong() == 0)
    {
        return IEEE_754(std::bitset<IEEE_N_BITS>(0b00000000000000000000000000000000));
    }

    // number / inf = zero
    if (exponent2_ulong == max_exponent && mantissa2.to_ulong() == 0)
    {
        return IEEE_754(std::bitset<IEEE_N_BITS>(0b00000000000000000000000000000000));
    }

    if (num1_is_denormalized)
    {
        exponent1_ulong = 1;
    }

    if (num2_is_denormalized)
    {
        exponent2_ulong = 1;
    }

    std::bitset<IEEE_MANTISSA_BITS + 1> _mantissa1(mantissa1.to_ulong());
    std::bitset<IEEE_MANTISSA_BITS + 1> _mantissa2(mantissa2.to_ulong());

    if (num1_is_denormalized)
    {
        _mantissa1[IEEE_MANTISSA_BITS] = 0;
    }
    else
    {
        _mantissa1[IEEE_MANTISSA_BITS] = 1;
    }

    if (num2_is_denormalized)
    {
        _mantissa2[IEEE_MANTISSA_BITS] = 0;
    }
    else
    {
        _mantissa2[IEEE_MANTISSA_BITS] = 1;
    }

    int load = static_cast<int>(IEEE_754::pow_of_two(IEEE_EXPONENT_BITS - 1)) - 1;
    long exponent_result = exponent1_ulong - exponent2_ulong + load;

    if (exponent_result < (-1) * IEEE_MANTISSA_BITS)
    {
        return IEEE_754(std::bitset<IEEE_N_BITS>(0b00000000000000000000000000000000));
    }

    unsigned long long mantissa_division_result = (unsigned long long)_mantissa1.to_ulong() * IEEE_754::pow_of_two(IEEE_MANTISSA_BITS) / (unsigned long long)_mantissa2.to_ulong();

    std::bitset<IEEE_MANTISSA_BITS + 2> mantissa_result;

    mantissa_result = (unsigned long)mantissa_division_result;

    while (exponent_result <= 0 && mantissa_result.to_ulong() != 0)
    {
        exponent_result++;
        mantissa_result >>= 1;
    }

    while (mantissa_result[IEEE_MANTISSA_BITS] == 0 && exponent_result > 0)
    {
        exponent_result -= 1;
        if (exponent_result != 0)
            mantissa_result <<= 1;
    }

    // > max
    if (exponent_result >= static_cast<long>(max_exponent))
    {
        exponent_result = max_exponent;
        mantissa_result = 0;
    }

    IEEE_754 result;

    result.get_number()[IEEE_N_BITS - 1] = (sign1 != sign2);

    PASTE_BITS_INTO2(IEEE_MANTISSA_BITS + 1, IEEE_N_BITS, mantissa_result, result.number, 0, IEEE_MANTISSA_BITS - 1, 0, IEEE_MANTISSA_BITS - 1);
    PASTE_BITS_INTO(IEEE_EXPONENT_BITS, IEEE_N_BITS, std::bitset<IEEE_EXPONENT_BITS>(exponent_result), result.number, IEEE_MANTISSA_BITS, IEEE_MANTISSA_BITS + IEEE_EXPONENT_BITS - 1);
    totalTime += std::chrono::high_resolution_clock::now() - start_time;
    return result;
}

bool IEEE_754::operator<(const IEEE_754 &other) const
{
    // Check if the numbers have the same sign
    operations_count++;
    other.operations_count++;
    all_operations_count["<"]++;
    other.all_operations_count["<"]++;
    bool this_sign = this->get_sign_bit();
    bool other_sign = other.get_sign_bit();
    if (this_sign != other_sign)
    {
        return this_sign > other_sign;
    }

    // If the numbers have the same sign, compare their values
    std::bitset<IEEE_N_BITS> this_bits = this->get_number();
    std::bitset<IEEE_N_BITS> other_bits = other.get_number();
    return (this_bits.to_ulong() < other_bits.to_ulong()) ^ this_sign;
}

bool IEEE_754::operator<=(const IEEE_754 &other) const
{
    operations_count++;
    other.operations_count++;
    all_operations_count["<="]++;
    other.all_operations_count["<="]++;
    return (*this < other) || (*this == other);
}

bool IEEE_754::operator>(const IEEE_754 &other) const
{
    operations_count++;
    other.operations_count++;
    all_operations_count[">"]++;
    other.all_operations_count[">"]++;
    return !(*this <= other);
}

bool IEEE_754::operator>=(const IEEE_754 &other) const
{
    operations_count++;
    other.operations_count++;
    all_operations_count[">="]++;
    other.all_operations_count[">="]++;
    return !(*this < other);
}

bool IEEE_754::operator==(const IEEE_754 &other) const
{
    operations_count++;
    other.operations_count++;
    all_operations_count["=="]++;
    other.all_operations_count["=="]++;
    return this->get_number() == other.get_number();
}

bool IEEE_754::operator!=(const IEEE_754 &other) const
{
    operations_count++;
    other.operations_count++;
    all_operations_count["!="]++;
    other.all_operations_count["!="]++;
    return !(*this == other);
}

IEEE_754 operator+(double d, IEEE_754 &p)
{
    return p + d;
}

IEEE_754 operator-(double d, IEEE_754 &p)
{
    return p - d;
}

IEEE_754 operator*(double d, IEEE_754 &p)
{
    return p * d;
}

IEEE_754 operator/(double d, IEEE_754 &p)
{
    return p / d;
}
