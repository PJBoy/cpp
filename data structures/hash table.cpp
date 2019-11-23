#include "../utility/utility.h"
#include <algorithm>
#include <forward_list>


template<typename T>
class HashTable
{
    using Bucket = std::forward_list<KV<index_t, T>>;
    
    static const n_t n_data{256};

    Bucket data[n_data];

    index_t hash(index_t k_hash) const
    {
        return k_hash % n_data;
    }

    static Bucket::iterator find_before(Bucket& bucket, index_t value)
    {
        if (std::begin(bucket) == std::end(bucket))
            return std::end(bucket);

        if (*std::begin(bucket) == value)
            return bucket.before_begin();

        return std::adjacent_find(std::begin(bucket), std::end(bucket), [&](auto const&, auto const& rhs)
        {
            return rhs == value;
        });
    }

public:
    template<typename TT>
    void add(TT&& v)
    {
        const index_t v_hash(hash(std::hash<T>(v)()));

        data[v_hash].emplace_front(k, std::forward<TT>(v));
    }

    bool lookup(const T& v) const
    {
        const index_t v_hash(hash(std::hash<T>(v)()));

        const Bucket& bucket(data[v_hash]);
        auto it(std::find(std::begin(bucket), std::end(bucket), v_hash));
        return it != std::end(bucket);
    }

    void remove(const T& v)
    {
        const index_t v_hash(hash(std::hash<T>(v)()));

        const Bucket& bucket(data[v_hash]);
        auto it(find_before(bucket, v_hash));
        if (it != std::end(bucket))
            bucket.erase_after(it);
    }
};
