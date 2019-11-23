#include "../utility/utility.h"
#include <bitset>
#include <cstdio>
#include <cstdlib>


namespace wheel
{
    template<typename T>
    index_t binarySearch_min(const T data[], n_t n, T v)
    {
        // Return the index of the first element e such that v <= e

        if (n == 0)
            return 0;

        if (v <= data[0])
            return 0;

        if (data[n - 1] < v)
            return n;

        index_t i_left{};
        while (n > 2)
        {
            // Invariant: data[i_left] < v <= data[i_left + n]
            // v <= data[i_left + n / 2] => data[i_left] < v <= data[i_left + n] where n = n / 2
            // data[i_left + n / 2] < v => data[i_left] < v <= data[i_left + n] where i_left = i_left + n / 2, n = n - n / 2
            const n_t n_left(n / 2);
            const index_t i_pivot(i_left + n_left);
            if (v <= data[i_pivot])
                n = n_left;
            else
            {
                i_left = i_pivot;
                n -= n_left;
            }
        }

        return i_left + 1;
    }
    template<typename T, n_t n>
    unsigned binarySearch_min(const T(&x)[n], const T v){ return binary_search_min(x, n, v); }

    template<typename T>
    index_t binarySearch_max(const T x[], n_t n, T v)
    {
        // Return the index of the last element e such that e <= v

        if (n == 0)
            return 0;

        if (v < data[0])
            return 0;

        if (data[n - 1] <= v)
            return n;

        index_t i_left{};
        while (n > 2)
        {
            // Invariant: data[i_left] <= v < data[i_left + n]
            // v < data[i_left + n / 2] => data[i_left] <= v < data[i_left + n] where n = n / 2
            // data[i_left + n / 2] <= v => data[i_left] <= v < data[i_left + n] where i_left = i_left + n / 2, n = n - n / 2
            const n_t n_left(n / 2);
            const index_t i_pivot(i_left + n_left);
            if (v < data[i_pivot])
                n = n_left;
            else
            {
                i_left = i_pivot;
                n -= n_left;
            }
        }

        return i_left;
    }
    template<typename T, n_t n>
    unsigned binarySearch_max(const T(&x)[n], const T v){ return binary_search_max(x, n, v); }

    template<typename T>
    interval_t binarySearch_interval(const T x[], n_t n, const T v)
    {
        return {binary_search_min(x, n, v), binary_search_max(x, n, v)};
    }
    template<typename T, n_t n>
    interval_t binarySearch_interval(const T(&x)[n], const T v){ return binary_search_interval(x, n, v); }


    unsigned long long exponentiate(unsigned long long base, std::bitset<bitSize(bitSize_v<unsigned long long>-1)> exponent)
    {
        // Square and multiply, right to left
        /*
            b^e = b^(sum_k e_k 2^k)
                = prod_k b^(e_k 2^k)
        */

        if (base < 2)
            return base;

        unsigned long long ret(1);
        while (exponent.any())
        {
            if (exponent.test(0))
                ret *= base;

            base *= base;
            exponent >>= 1;
        }

        return ret;
    }


    bool c_getLine(FILE* p_file, char** p_outputBuffer, n_t* p_n_outputBuffer)
    {
        // Reads a line from a file (up to newline or EOF, not including the newline character).
        // Caller is expected to free *p_outputBuffer in all cases
        // On success:
        //     Sets *p_outputBuffer to a malloc'd buffer containing the extracted line (null terminated)
        //     If n_outputBuffer is not NULL, sets *p_n_outputBuffer to the size of the output buffer (possibly larger than the text length)
        //     Returns true
        // If p_outputBuffer is NULL:
        //     If n_outputBuffer is not NULL, sets *p_n_outputBuffer to 0
        //     Returns false
        // If a memory allocation fails:
        //     Sets *p_outputBuffer to the largest successfully malloc'd buffer filled with extracted text (possibly NULL, otherwise null terminated)
        //     If n_outputBuffer is not NULL, sets *p_n_outputBuffer to the size of the output buffer
        //     Returns false
        // If a file error occurs:
        //     Sets *p_outputBuffer to the largest successfully read to buffer, but with an indeterminate extra amount of text extracted from p_file (null terminated)
        //     If n_outputBuffer is not NULL, sets *p_n_outputBuffer to the size of the output buffer
        //     Returns false

        // Technically this is non-standard in C++ due to the use of malloc/realloc without placement new to initialise the char elements of the buffer to begin their object lifetime.
        // Note that applying placement new on the realloc'd buffer would cause the elements to have 'indeterminate values' and thus UB to read from later, requiring memcpy to be fully C++ compliant.

    #ifdef __cplusplus
        using std::feof;
        using std::ferror;
        using std::fgets;
        using std::malloc;
        using std::min; // C has no min, would have to write one (and one most likely already exists)
        using std::realloc;
    #endif

        // Initial buffer size. Buffer size is doubled with each reallocation
        const n_t n_buffer_initial = 8;

        // RET scope variables
        bool ret = false;
        char* buffer = NULL;
        n_t n_buffer = 0;

        {
            // Null pointer check
            if (!p_outputBuffer)
                goto RET;

            // Create initial buffer
            n_buffer = n_buffer_initial;
            buffer = (char*)(malloc(n_buffer));
            if (!buffer)
                goto RET;

            buffer[0] = '\0';

            // Read from file until successfully extracted entire line, reallocating the buffer as needed
            char* p_buffer(buffer);
            while (!fgets(p_buffer, (int)(buffer + n_buffer - p_buffer), p_file) && !feof(p_file))
            {
                if (ferror(p_file))
                {
                    n_buffer = min(n_buffer_initial, n_buffer / 2);
                    buffer[n_buffer - 1] = '\0';
                    goto RET;
                }

                n_t n_buffer_new = n_buffer * 2;
                void* buffer_new = realloc(buffer, n_buffer_new);
                if (!buffer_new)
                    goto RET;

                buffer = (char*)(buffer_new);
                p_buffer = buffer + n_buffer - 1;
                n_buffer = n_buffer_new;
            }

            // Determine the actual string length and remove trailing newline characters
            n_buffer = n_buffer / 2 - 1 + strlen(p_buffer) + 1;
            while (n_buffer - 1 > 0 && (buffer[n_buffer - 2] == '\n' || buffer[n_buffer - 2] == '\r'))
                buffer[--n_buffer - 1] = '\0';

            // Attempt to shrink buffer to fit
            void* buffer_new = realloc(buffer, n_buffer);
            if (buffer_new)
                buffer = (char*)(buffer_new);

            ret = true;
        }

    RET:
        if (p_outputBuffer)
            *p_outputBuffer = buffer;

        if (p_n_outputBuffer)
            *p_n_outputBuffer = n_buffer;

        return ret;
    }
}
