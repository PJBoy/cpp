#include "../utility/utility.h"
#include <algorithm>
#include <forward_list>


class HashTable
{
    using Bucket = std::forward_list<KV>;
    
    static const n_t n{256};

    Bucket data[n];

    Bucket& getBucket(int k)
    {
        return data[hash(k)];
    }

    const Bucket& getBucket(int k) const
    {
        return data[hash(k)];
    }

    index_t hash(int k) const
    {
        return k % n;
    }

public:
    void add(int k, int v)
    {
        getBucket(k).emplace_front(k, v);
    }

    int lookup(int k) const
    {
        const Bucket& bucket(getBucket(k));
        auto entry(std::find(std::cbegin(bucket), std::cend(bucket), k));
        if (entry == std::cend(bucket))
            throw std::domain_error("HashTable::lookup: key not found");

        return entry->v;
    }

    void remove(int k)
    {
        Bucket& bucket(getBucket(k));
        std::remove(std::begin(bucket), std::end(bucket), k);
    }
};
