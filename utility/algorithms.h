#pragma once

#include "typedefs.h"
#include <algorithm>
#include <bitset>
#include <type_traits>


constexpr bool powerOfTwo(unsigned v)
{
	if ((v & v - 1) != 0)
		return false;

	if (v == 0)
		return false;

	return true;
}

template<typename T>
constexpr n_t numbits()
{
    return sizeof(T) * CHAR_BIT;
}

// Find the number of bits needed to contain v, i.e. ceil(log_2(v+1)) = floor(log_2(v)+1)
template<typename T, typename std::enable_if_t<std::is_unsigned_v<T>, T> = T()>
constexpr n_t numbits(T v)
{
	n_t bits(0);
	for (; v; v >>= 1)
		++bits;

	return bits;
}

template<typename T, typename std::enable_if_t<std::is_signed_v<T>, T> = T()>
constexpr n_t numbits(T v)
{
	n_t bits(1); // For the sign bit
	for (; v && v != -1; v >>= 1)
		++bits;

	return bits;
}

template<n_t n>
constexpr n_t maxbitsof(const signed(&data)[n])
{
    return
        std::max
        (
            numbits(*std::max_element(std::begin(data), std::end(data))),
            numbits(*std::min_element(std::begin(data), std::end(data)))
        );
}

template<n_t n>
constexpr n_t maxbitsof(const unsigned(&data)[n])
{
	return numbits(*std::max_element(std::begin(data), std::end(data)));
}

constexpr unsigned pmod(signed x, unsigned n)
{
    return (x % n + n) % n;
}

constexpr unsigned power(unsigned base, n_t exponent)
{
	unsigned ret(1);

	for (; exponent; --exponent)
		ret *= base;

	return ret;
}
