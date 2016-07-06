#include "../utility/utility.h"
#include <random>


class BloomFilter
{
    static const n_t
        n{256},
        n_hashes{16}; // For least false positive error, let this be: n / (n_insertions * e)

    unsigned a[n_hashes], b[n_hashes];
    bool data[n_hashes][n]{};


    unsigned uniform_random_int()
    {
        // std::random_device{} is a random number generator, operator() gives a random number used as a seed
        // UPRNG is a uniform psuedo random number generator using the seed
        // The bind is useful when requesting many random numbers

        static std::default_random_engine UPRNG(std::random_device{}());
        static std::uniform_int_distribution<unsigned> UID(0, n);

        return UID(UPRNG);
    }

    index_t hash(index_t i, int k) const
    {
        return (k*a[i] + b[i]) % n;
    }

public:
    BloomFilter()
    {
        for (unsigned& v : a)
            v = uniform_random_int();
        for (unsigned& v : b)
            v = uniform_random_int();
    }

    void add(int k)
    {
        for (index_t i(0); i < n_hashes; ++i)
            data[i][hash(i, k)] = true;
    }

    bool lookup(int k) const
    {
        for (index_t i(0); i < n_hashes; ++i)
            if (!data[i][hash(i, k)])
                return false;
        return true;
    }
};
