#include "../utility/utility.h"
#include <array>
#include <random> 


class CuckooHashTable
{
    struct Entry : public KV<int, int>
    {
        bool useHash2;

        Entry() = default;

        Entry(int k, int v, bool useHash2)
            : KV(k, v), useHash2(useHash2)
        {}
    };


    static const n_t n{256};

    int
        a{1},
        b{0},
        a2{2},
        b2{1};

    std::array<Entry, n> data;


    unsigned uniform_random_int()
    {
        // std::random_device{} is a random number generator, operator() gives a random number used as a seed
        // UPRNG is a uniform psuedo random number generator using the seed
        // The bind is useful when requesting many random numbers

        static std::default_random_engine UPRNG(std::random_device{}());
        static std::uniform_int_distribution<unsigned> UID(0, n);

        return UID(UPRNG);
    }

    int hash(int k, bool useHash2 = false) const
    {
        if (useHash2)
            return (k*a2 + b2) % n;
        return (k*a + b) % n;
    }

    void rehash()
    {
        a = uniform_random_int();
        b = uniform_random_int();
        a2 = uniform_random_int();
        b2 = uniform_random_int();

        // copy all values and reinsert them after clearing table (from inits)
        decltype(data) data_copy;
        data_copy = data;
        std::fill(std::begin(data), std::end(data), Entry());
        for (const Entry& entry : data_copy)
            add(entry.k, entry.v);
    }

public:
    void add(int k, int v, bool useHash2 = false, n_t attemptsLeft = n)
    {
        Entry& entry(data[hash(k, useHash2)]);
        
        if (!entry)
        {
            entry = Entry(k, v, useHash2);
            return;
        }

        if (attemptsLeft == 0)
        {
            rehash();
            add(k, v, useHash2);
            return;
        }

        Entry old(entry);
        entry = Entry(k, v, useHash2);
        add(old.k, old.v, !old.useHash2, attemptsLeft - 1);
    }

    int lookup(int k) const
    {
        const Entry& entry(data[hash(k, false)]);
        if (entry.init && entry.k == k)
            return entry.v;
        
        const Entry& entry2(data[hash(k, true)]);
        if (entry.init && entry2.k == k)
            return entry2.v;
        
        throw std::domain_error("CuckooHashTable::lookup: key not found");
    }

    void remove(int k)
    {
        data[hash(k)].init = false;
    }
};
