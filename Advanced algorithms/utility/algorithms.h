#pragma once

#include "typedefs.h"


constexpr bool powerOfTwo(unsigned v)
{
    // The '&& v' part is to stop 0 being classed as a power of two
    return (v & (v - 1)) == 0 && (v != 0);
}

template<typename T>
constexpr n_t numbits()
{
    return sizeof(T) * CHAR_BIT;
}

// Find the number of bits needed to contain v, i.e. ceil(log_2(v+1)) = floor(log_2(v)+1)
constexpr n_t numbits(unsigned v, n_t bits = 0)
{
    return v == 0 ? bits : numbits(v >> 1, bits + 1);
}

constexpr n_t numbits(signed v, n_t bits = 1)
{
    return v == 0 || v == -1 ? bits : numbits(v >> 1, bits + 1);
}

template<typename T>
constexpr T max(T a, T b)
{
    return a < b ? b : a;
}

template<typename T>
constexpr T min(T a, T b)
{
    return a < b ? a : b;
}

template<typename T, n_t n>
constexpr n_t numof(T(&data)[n])
{
    return n;
}

template<typename T, n_t n>
constexpr T maxof(const T(&data)[n], index_t i = 0, T max = T())
{
    return i < n ? maxof(data, i + 1, i == 0 ? data[0] : ::max(max, data[i])) : max;
}

template<typename T, n_t n>
constexpr T minof(const T(&data)[n], index_t i = 0, T min = T())
{
    return i < n ? minof(data, i + 1, i == 0 ? data[0] : ::min(min, data[i])) : min;
}

template<n_t n>
constexpr unsigned maxbitsof(const signed(&data)[n])
{
    return
        max
        (
            numbits(maxof(data)),
            numbits(minof(data))
        );
}

template<n_t n>
constexpr unsigned maxbitsof(const unsigned(&data)[n])
{
    return numbits(maxof(data));
}

constexpr unsigned pmod(signed x, unsigned n)
{
    return (x % n + n) % n;
}
