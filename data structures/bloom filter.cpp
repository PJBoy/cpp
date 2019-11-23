#include "../utility/utility.h"
#include <random>
#include <ratio>


template<typename T, n_t estimatedNumberOfInsertions = 10, typename maxFalsePositiveProbability = std::ratio<5, 100>>
class BloomFilter
{
    /*
        A bloom filter is a probabilistic membership querying data structure.
        Supports insertion and lookup, but not deletion.
        Lookup may return falsely return positive with low probability.

        Works by applying multiple independent hash functions to the key, all mapping to the same array.
        Insertion writes `true` to all the locations mapped to by the hashing the input key.
        Lookup returns the conjunction of all locations mapped to by hashing the input key.

        For
            k = number of independent hash functions
            m = size of hash table
            n = number of insertions
            t = number of `true`s set in the hash table
        
        The probability of a hash function returning a false positive is t / m.
        t is upper bounded by kn (where every insertion writes `true` for every hash function)
        So the probability is upper bounded by kn / m.
        So for k independent hash functions, the probability of returning a false positive is upper bounded by (kn / m)^k.

        The value of k that minimises (kn / m)^k is m / (n e).
        This gives a false positive probability upper bound of (e'^e')^(m / n) ~= 0.692^(m / n), where e' = 1 / e.
        
        Thus given the predicted number of insertions n and a required false positive probability upper bound p, the table size m is lower bounded by:
            m >= n log(p) / log(e'^e') = e n ln(1/p)
    */

    static const n_t
        //n_data{n_t(estimatedNumberOfInsertions * std::log(double(maxFalsePositiveProbability::den) / maxFalsePositiveProbability::num) * 2.718)}, // Would ideally do this, if log could be constexpr'ified
        n_data{256},
        n_hashes{n_t(n_data / 2.718 / estimatedNumberOfInsertions)};

    unsigned a[n_hashes], b[n_hashes];
    bool data[n_data]{};


    static unsigned uniform_random_int()
    {
        static std::default_random_engine UPRNG(std::random_device{}());
        static std::uniform_int_distribution<unsigned> UID;

        return UID(UPRNG);
    }

    index_t hash(index_t i, index_t k_hash) const
    {
        return (k_hash * a[i] + b[i]) % n_data;
    }

public:
    BloomFilter()
    {
        for (unsigned& v : a)
            v = uniform_random_int();

        for (unsigned& v : b)
            v = uniform_random_int();
    }

    template<typename TT>
    void add(TT&& v)
    {
        const index_t v_hash(std::hash<T>(std::forward<TT>(v))());

        for (index_t i{}; i < n_hashes; ++i)
            data[hash(i, v_hash)] = true;
    }

    template<typename TT>
    bool lookup(TT&& v) const
    {
        const index_t v_hash(std::hash<T>(std::forward<TT>(v))());

        for (index_t i{}; i < n_hashes; ++i)
            if (!data[hash(i, v_hash)])
                return false;

        return true;
    }
};

#if 0
int main()
{
    BloomFilter<int> b;
}
#endif
