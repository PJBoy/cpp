#include "../utility/utility.h"
#include <fstream>
#include <iostream>

namespace fun
{
    // The simplest sorting algorithms
    void leftPartition(int a[], n_t n)
    {
        for (index_t r{}; r < n; ++r)
            for (index_t l{}; l < r; ++l)
                if (a[r] < a[l])
                    std::swap(a[r], a[l]);
    }
    template<n_t N>
    void leftPartition(int(&a)[N]) { leftPartition(a, N); }

    void rightPartition(int a[], n_t n)
    {
        for (index_t l{}; l < n; ++l)
            for (index_t r(l); r < n; ++r)
                if (a[r] < a[l])
                    std::swap(a[l], a[r]);
    }
    template<n_t N>
    void rightPartition(int(&a)[N]) { rightPartition(a, N); }


    // The simplest formula for pi
    double pi(const double precision)
    {
        /*
            Approximates t = pi by using a sequence of linear approximations of the sin-cos differential equation to find the first t > 0 such that sin(t*h) < 0
            x(t) = sin
            y(t) = cos
            x' =  y
            y' = -x
            x(0) = 0
            y(0) = 1
            x(t+h) = x(t) + x'(t) * h
            y(t+h) = y(t) + y'(t+h) * h
            x(pi) = 0
            y(pi) = -1
        */

        const double h(precision);

        unsigned t(0);
        for (double x(0), y(1); x >= 0; ++t)
        {
            x += y * h;
            y -= x * h;
        }

        return t * h;
    }


    // Bitwise operations implemented without bitwise operations
    unsigned lsl(unsigned x, unsigned y)
    {
        for (; y; --y)
            x *= 2;

        return x;
    }

    unsigned lsr(unsigned x, unsigned y)
    {
        for (; y; --y)
            x /= 2;

        return x;
    }

    unsigned not_(unsigned x)
    {
        return -1 - x;
    }

    unsigned and_(unsigned x, unsigned y)
    {
        constexpr n_t n(bitSize_v<decltype(x)>);

        unsigned
            ret(0),
            multiplier(1);

        for (index_t i(n); i; --i)
        {
            ret += x*y % 2 * multiplier;
            multiplier *= 2;
            x /= 2;
            y /= 2;
        }

        return ret;
    }

    unsigned or_(unsigned x, unsigned y)
    {
        constexpr n_t n(bitSize_v<decltype(x)>);

        unsigned
            ret(0),
            multiplier(1);

        for (index_t i(n); i; --i)
        {
            ret += (x + y - x*y) % 2 * multiplier;
            multiplier *= 2;
            x /= 2;
            y /= 2;
        }

        return ret;
    }

    unsigned xor_(unsigned x, unsigned y)
    {
        constexpr n_t n(bitSize_v<decltype(x)>);

        unsigned
            ret(0),
            multiplier(1);

        for (index_t i(n); i; --i)
        {
            ret += (x + y) % 2 * multiplier;
            multiplier *= 2;
            x /= 2;
            y /= 2;
        }

        return ret;
    }


    // User defined literal _bits, constructs the minimum capacity bitset for an integer.
    // E.g. 51_bits = std::bitset<6>(0b110011)
    template<char... S, index_t... I>
    constexpr unsigned long long charsToInt_helper(std::index_sequence<I...>)
    {
        return (((S - '0') * power(10, sizeof...(I)-1 - I)) + ...);
    }

    template<char... S>
    constexpr unsigned long long charsToInt()
    {
        return charsToInt_helper<S...>(std::make_index_sequence<sizeof...(S)>());
    }

    template<char... S>
    constexpr auto operator"" _bits() // -> std::bitset<bitSize(charsToInt<S...>())>
    {
        return std::bitset<bitSize(charsToInt<S...>())>(charsToInt<S...>());
    }

    void bitsExample()
    {
        std::cout << 51_bits << '\n';
    }
}

#if 0
int main()
{
    fun::bitsExample();
}
#endif
