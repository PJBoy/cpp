#include "../utility/utility.h"
#include <random>


class StaticHashTable
{
    class Subtable
    {
        // Hash parameters
        unsigned
            a{1},
            b{0};

        // Will have size n^2 where n is the number of actual values in this hash table
        Array<int> data;
        Array<bool> used;

        index_t hash(int k) const
        {
            return (k*a + b) % std::size(data);
        }

        unsigned uniform_random_int()
        {
            // std::random_device{} is a random number generator, operator() gives a random number used as a seed
            // UPRNG is a uniform psuedo random number generator using the seed
            // The bind is useful when requesting many random numbers

            static std::default_random_engine UPRNG(std::random_device{}());

            return std::uniform_int_distribution<unsigned>(0, std::size(data))(UPRNG);
        }

        void nextHashParameters()
        {
            a = uniform_random_int();
            b = uniform_random_int();
        }

    public:
        Subtable(const Array<KV>& KVs)
        {
            const n_t n(std::size(KVs));

            data = Array<int>(n * n);
            used = Array<bool>(n * n);

            // Find hash function
            reconstruct:
            {
                std::fill(std::begin(used), std::end(used), false);
                for (const KV& kv : KVs)
                {
                    // If found a collision, try again
                    if (used[hash(kv.k)])
                    {
                        nextHashParameters();
                        goto reconstruct;
                    }

                    used[hash(kv.k)] = true;
                }
            }

            // Found hash function, add all the values to the hash table
            for (const KV& kv : KVs)
                data[hash(kv.k)] = kv.v;
        }

        int lookup(int k) const
        {
            if (!used[hash(k)])
                throw std::domain_error("StaticHashTable::Subtable::lookup: key not found");

            return data[hash(k)];
        }
    };


    const n_t n;

    unsigned
        a{1},
        b{0};
    
    Array<Subtable> data;

    index_t hash(int k) const
    {
        return (k*a + b) % n;
    }

    unsigned uniform_random_int()
    {
        // std::random_device{} is a random number generator, operator() gives a random number used as a seed
        // UPRNG is a uniform psuedo random number generator using the seed
        // The bind is useful when requesting many random numbers

        static std::default_random_engine UPRNG(std::random_device{}());
        static std::uniform_int_distribution<unsigned> UID(0, n);

        return UID(UPRNG);
    }

    void nextHashParameters()
    {
        a = uniform_random_int();
        b = uniform_random_int();
    }
    
public:
    StaticHashTable(Array<KV> KVs)
        : n(std::size(KVs))
    {
        // Array of number of collisions from keys
        Array<n_t> hashCounts(n);

        // Find the hash parameters necessary to have less than n collisions
        reconstruct:
        {
            // Initialise hash counts
            std::fill(std::begin(hashCounts), std::end(hashCounts), 0);

            n_t collisions(0);
            for (const KV& kv : KVs)
            {
                n_t& hashCount(hashCounts[hash(kv.k)]);
                ++hashCount;

                // Handle collisions
                if (hashCount > 1)
                {
                    // Require less than n collisions
                    ++collisions;
                    if (collisions >= n)
                    {
                        nextHashParameters();
                        goto reconstruct;
                    }
                }
            }
        }

        // Add data to respective subtables
        Array<Array<KV>> table(n);
        for (index_t i(0); i < n; ++i)
            table[i] = Array<KV>(hashCounts[i]);

        for (const KV& kv : KVs)
            table[hash(kv.k)].push_back({kv.k, kv.v});

        for (index_t i(0); i < n; ++i)
            data[i] = Subtable(table[i]);
    }

    int lookup(int k) const
    {
        return data[hash(k)].lookup(k);
    }
};
