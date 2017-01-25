#include "../utility/utility.h"
#include <bitset>
#include <cstdio>
#include <cstdlib>


namespace wheel
{
    template<typename T>
    index_t binarySearch_min(const T x[], n_t n, const T v)
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
    unsigned binarySearch_min(const T(&x)[n], const T v){ return binary_search_min(x, n, v); }

    template<typename T>
    index_t binarySearch_max(const T x[], n_t n, const T v)
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
    unsigned binarySearch_max(const T(&x)[n], const T v){ return binary_search_max(x, n, v); }

    template<typename T>
    interval_t binarySearch_interval(const T x[], n_t n, const T v)
    {
        interval_t interval{binary_search_min(x, n, v), binary_search_max(x, n, v)};
        if (!interval.second)
            return {0,0};
        return interval;
    }
    template<typename T, n_t n>
    interval_t binarySearch_interval(const T(&x)[n], const T v){ return binary_search_interval(x, n, v); }


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


    char* c_read_stdin()
    {
        const unsigned n_bufferChunk(8);
        #define N_BUFFER_CHUNK "8"
        
        unsigned n_buffer(n_bufferChunk);
        char* buffer((char*)(std::malloc(n_buffer + 1)));
        *buffer = 0;
        char newlineBuffer[2];
        while (std::scanf("%" N_BUFFER_CHUNK "[^\n]%1[\n]", buffer + n_buffer - n_bufferChunk, newlineBuffer) == 1)
        {
            n_buffer += n_bufferChunk;
            buffer = (char*)(std::realloc(buffer, n_buffer));
        }

        return buffer;
    }
}
