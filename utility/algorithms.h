#pragma once

#include "typedefs.h"

#include <algorithm>
#include <bitset>
#include <type_traits>


constexpr bool isPowerOfTwo(unsigned long long v)
{
	if ((v & v - 1) != 0)
		return false;

	if (v == 0)
		return false;

	return true;
}

template<typename T>
constexpr n_t bitSize_v(sizeof(T) * CHAR_BIT);

// Find the number of bits needed to contain v, i.e. ceil(log_2(v+1)) = floor(log_2(v)+1)
template<typename T>
constexpr n_t bitSize(T v)
{
    if constexpr (std::is_unsigned_v<T>)
    {
        n_t n_bits{};
	    for (; v; v >>= 1)
		    ++n_bits;

	    return n_bits;
    }
    else
    {
        n_t n_bits(1); // For the sign bit
        for (; v && v != -1; v >>= 1)
            ++n_bits;

        return n_bits;
    }
}

template<typename T, n_t n>
constexpr n_t maxBitSizeOf(const T (&data)[n])
{
    if constexpr (n == 0)
        return 0;

    if constexpr (std::is_unsigned_v<T>)
        return numbits(*std::max_element(std::begin(data), std::end(data)));
    else
    {
        return
            std::max
            (
                numbits(*std::max_element(std::begin(data), std::end(data))),
                numbits(*std::min_element(std::begin(data), std::end(data)))
            );
    }
}

constexpr unsigned long long pmod(signed long long x, unsigned long long n)
{
    return static_cast<unsigned long long>(x % n + n) % n;
}

constexpr unsigned long long power(unsigned long long base, n_t exponent)
{
	unsigned long long ret(1);

	while (exponent --> 0)
		ret *= base;

	return ret;
}
