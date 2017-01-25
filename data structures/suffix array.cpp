#include "../utility/utility.h"
#include <algorithm>
#include <string>


template<typename T>
class SuffixArray
{
    // A custom radix sort that sorts keys such that their corresponding unsigned values are in ascending order, but with -1 as a special value that is the lowest
    template<n_t n_elements = 1, n_t n_bits = sizeof(unsigned) * CHAR_BIT>
    void radix_sort(index_t k[], unsigned v[], n_t n)
    {
        for (index_t element(n_elements); element --> 0;)
            for (index_t i(0); i < n_bits; ++i)
                std::stable_partition(k, k + n, [v, element, i](index_t k) { return !(v[k + element] + 1 & 1 << i); });
    }

    index_t binary_search_min(const index_t suffices_indices[], n_t n, const unsigned v, const index_t i_data)
    {
        index_t begin(0);
        while (n > 0)
        {
            const n_t step(n / 2);
            const index_t i(begin + step);
            const auto item(data[suffices[suffices_indices[i]] + i_data]);
            if (item < v)
            {
                begin = i + 1;
                n -= step + 1;
            }
            else
                n = step;
        }
        if (data[suffices[suffices_indices[begin]] + i_data] != v)
            return n;
        return begin;
    }

    index_t binary_search_max(const index_t suffices_indices[], n_t n, const unsigned v, const index_t i_data)
    {
        index_t begin(0);
        while (n > 0)
        {
            const n_t step(n / 2);
            const index_t i(begin + step);
            if (data[suffices[suffices_indices[i]] + i_data] <= v)
            {
                begin = i + 1;
                n -= step + 1;
            }
            else
                n = step;
        }
        if (!begin || data[suffices[suffices_indices[begin - 1]] + i_data] != v)
            return 0;
        return begin;
    }

public:
    // The data and corresponding array of indices into the data, sorted in lexicographical order of the associated suffices
    const n_t n;
    Array<T> data;
    Array<index_t> suffices;

    SuffixArray(const Array<T>& input)
        : n(std::size(input))
    {
    /*
        _____input______|_______R_1_______|_______R_2_______|______R______
        "abc""def""hij" | "bcd""efh""ij$" | "cde""fhi""j$$" | 1 2 4 5 7 8
        "abc""def""hi$" | "bcd""efh""i$$" | "cde""fhi"      | 1 2 4 5 7
        "abc""def""h$$" | "bcd""efh"      | "cde""fh$"      | 1 2 4 5
        "abc""def"      | "bcd""ef$"      | "cde""f$$"      | 1 2 4 5
        "abc""de$"      | "bcd""e$$"      | "cde"           | 1 2 4
        "abc""d$$"      | "bcd"           | "cd$"           | 1 2
        "abc"           | "bc$"           | "c$$"           | 1 2
        "ab$"           | "b$$"           |                 | 1
        "a$$"           | ""              | ""              | 
        ""              | ""              | ""              |
    */
        
        // Minimal suffix array
        if (!n)
            return;

        // Allocate space for suffices
        suffices = Array<index_t>(n);

        // Allocated space for data, padded up to a length a multiple of three
        const unsigned spill(n % 3);
        if (!spill)
            data = Array<T>(n); // No padding necessary
        else
        {
            // Padding necessary
            const n_t padding(3 - spill);
            data = Array<T>(n + padding);
            std::fill_n(std::begin(data) + n, padding, -1); // Pad with FF's
        }

        // Copy input into data
        std::copy(std::cbegin(input), std::cend(input), std::begin(data));

        // There is no such thing as a good variable name...
        // Let R_0, R_1, R_2 be triplets of characters from the input
        // Let R_12 be the concatenation R_1 R_2, represented as triple-indices, sorted in lexicographical order

        // Construct the arrays R_0, R_12
        const n_t
            n_R_0((n + 2) / 3),
            n_R_12(n - n_R_0);

        Array<index_t>
            R_0(n_R_0),
            R_12(n_R_12);

        // Initialise R_0
        for (index_t i(0); i < n_R_0; ++i)
            R_0[i] = i * 3;

        // Initialise R_12
        for (index_t i(0); i < n_R_12; ++i)
            R_12[i] = i + i / 2 + 1;

        // Sort R_12 based on data
        radix_sort<3>(&R_12[0], &data[0], n_R_12);

        // Construct the suffix array of R_12
        // This is the suffix array of B_1 U B_2
        SuffixArray R_12_sa(R_12);

        // Let R be the index of an element of R_12 in R_12_sa
        Array<index_t> R(n);
        for (index_t i(0); i < n_R_12; ++i)
            R[R_12_sa.data[i]] = i;

        // Sort R_0, based on R + 1
        radix_sort(&R_0[0], &R[1], n_R_0);

        // Sort R_0 based on data
        radix_sort(&R_0[0], &data[0], n_R_0);

        // Merge R_0 with R_12 as the suffix array
        {
            index_t
                i_R_0(0),
                i_R_12(0);

            while (i_R_0 < n_R_0 && i_R_12 < n_R_12)
            {
                // Compare character
                if (data[R_0[i_R_0]] < data[R_12[i_R_12]])
                    suffices.push_back(R_0[i_R_0++]);
                else if (data[R_12[i_R_12]] < data[R_0[i_R_0]])
                    suffices.push_back(R_12[i_R_12++]);

                // Characters are equal, so compare rank of following suffix (which is known, as it is either in B_1 (for entry_R_0) or B_2 (for entry_R_1)
                else if (R[R_0[i_R_0] + 1] < R[R_12[i_R_12] + 1])
                    suffices.push_back(R_0[i_R_0++]);
                else
                    suffices.push_back(R_12[i_R_12++]);
            }
            if (i_R_0 < n_R_0)
                std::copy(std::cbegin(R_0) + i_R_0, std::cend(R_0), suffices.current());
            else
                std::copy(std::cbegin(R_12) + i_R_12, std::cend(R_12), suffices.current());
        }
    }

    std::string operator[](const index_t i)
    {
        if (i >= n)
            return std::string();

        const index_t suffix(suffices[i]);
        std::string ret(std::to_string(data[suffix]));
        for (index_t ii(suffix + 1); ii < n; ++ii)
            ret += ", " + std::to_string(data[ii]);

        return ret;
    }

    interval_t query(const unsigned input[], const n_t n_input)
    {
        // Binary search on each character such that begin is the first suffices index that prefixes the suffix and end is the last

        // Sanity check
        if (n_input > n || n_input == 0)
            return {0, 0};

        const n_t
            max_prefix(n - n_input), // The maximum number of characters that may occur before suffix for the suffix to be sufficiently long
            n_suffices_indices(max_prefix + 1);

        // Indices of the suffices with sufficient length to be valid matches
        Array<index_t> suffices_indices(n_suffices_indices);
        for (index_t i(0), j(0); j < n_suffices_indices; ++i)
            if (suffices[i] < n_suffices_indices)
                suffices_indices[j++] = i; // Indices into suffices eh? ... Yes

        index_t
            begin(0),
            end(n_suffices_indices);

        for (index_t i(0); i < n_input; ++i)
        {
            begin += binary_search_min(&suffices_indices[begin], end - begin, input[i], i);
            if (end <= begin)
                return {0, 0};
            end = begin + binary_search_max(&suffices_indices[begin], end - begin, input[i], i);
            if (end <= begin)
                return {0, 0};
        }

        return {begin + n_input - 1, end + n_input - 1};
    }
};
