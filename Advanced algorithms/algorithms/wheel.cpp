#include "../utility/utility.h"
#include <bitset>


namespace wheel
{
    template<typename T>
    index_t binary_search_min(const T x[], n_t n, const T v)
    {
        index_t begin(0);
        while (n > 0)
        {
            const n_t step(n / 2);
            const index_t i(begin + step);
            if (x[i] < v)
            {
                begin = i + 1;
                n -= step + 1;
            }
            else
                n = step;
        }
        if (x[begin] != v)
            return n;
        return begin;
    }
    template<typename T, n_t n>
    unsigned binary_search_min(const T(&x)[n], const T v){ return binary_search_min(x, n, v); }

    template<typename T>
    index_t binary_search_max(const T x[], n_t n, const T v)
    {
        index_t begin(0);
        while (n > 0)
        {
            const n_t step(n / 2);
            const index_t i(begin + step);
            if (x[i] <= v)
            {
                begin = i + 1;
                n -= step + 1;
            }
            else
                n = step;
        }
        if (!begin || x[begin - 1] != v)
            return 0;
        return begin;
    }
    template<typename T, n_t n>
    unsigned binary_search_max(const T(&x)[n], const T v){ return binary_search_max(x, n, v); }

    template<typename T>
    interval_t binary_search_interval(const T x[], n_t n, const T v)
    {
        interval_t interval{binary_search_min(x, n, v), binary_search_max(x, n, v)};
        if (!interval.second)
            return {0,0};
        return interval;
    }
    template<typename T, n_t n>
    interval_t binary_search_interval(const T(&x)[n], const T v){ return binary_search_interval(x, n, v); }


    unsigned exponentiate(unsigned base, std::bitset<numbits(numbits<unsigned>()-1)> exponent)
    {
        // Square and multiply, right to left
        /*
            b^e = b^(sum_k e_k 2^k)
                = prod_k b^(e_k 2^k)
        */

        if (base < 2)
            return base;

        unsigned ret(1);

        while (exponent.any())
        {
            if (exponent.test(0))
                ret *= base;

            base *= base;
            exponent >>= 1;
        }

        return ret;
    }
}
