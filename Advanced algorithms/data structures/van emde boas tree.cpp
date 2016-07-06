#include "../utility/utility.h"
#include <algorithm>


// TODO: make summary a vEB tree
class VanEmdeBoasTree
{
    index_t
        n,
        n_data,
        n_datum;
    
    index_t
        min{index_t(-1)},
        max{index_t(-1)};
    
    Array<VanEmdeBoasTree> data;
    Array<bool> summary;

    index_t high(index_t k)
    {
        return k / n_datum;
    }

    index_t low(index_t k)
    {
        return k % n_datum;
    }

public:
    VanEmdeBoasTree() = default;

    VanEmdeBoasTree(n_t n)
        : n(n)
    {
        if (n <= 2)
            return;

        n_t n_semiLogFloor(n_t(std::log2(n) / 2));
        n_datum = 1 << n_semiLogFloor;
        n_data = n_datum << 1;

        data = Array<VanEmdeBoasTree>(n_data);
        summary = Array<bool>(n_data);

        std::fill(std::begin(data), std::end(data), VanEmdeBoasTree(n_datum));
        std::fill(std::begin(summary), std::end(summary), false);
    }

    void add(index_t k)
    {
        if (n <= 2)
        {
            if (min == -1)
                min = max = k;
            else if (min < k)
                max = k;
            else
                min = k;
        }
        else
        {
            const index_t h(high(k));
            summary[h] = true;
            data[h].add(k);
        }
    }

    bool lookup(index_t k)
    {
        if (n <= 2)
            return min == k || max == k;

        const index_t h(high(k));
        if (!summary[h])
            return false;

        return data[h].lookup(k);
    }

    void remove(index_t k)
    {
        if (n <= 2)
        {
            if (min == k)
                min = -1;
            if (max == k)
                max = -1;

            return;
        }
        
        const index_t h(high(k));
        if (!summary[h])
            return;

        data[h].remove(k);
        if (data[h].min == -1)
        {
            summary[h] = false;
            if (std::all_of(std::cbegin(summary), std::cend(summary), [](const int& v){ return v == false; }))
                min = max = -1;
        }
    }

    int minimum()
    {
        return min;
    }

    int maximum()
    {
        return max;
    }

    int predecessor(index_t k)
    {
        if (k <= min)
            return -1;
        if (k > max)
            return max;

        if (n <= 2)
            return min;

        const index_t h(high(k));
        for (index_t i(h + 1); i --> 0;)
            if (summary[i])
            {
                const int p(data[i].predecessor(k));
                if (p != -1)
                    return p;
            }
        
        return min;
    }

    int successor(index_t k)
    {
        if (k >= max)
            return -1;
        if (k < min)
            return min;

        if (n <= 2)
            return max;

        index_t h(high(k));
        for (index_t i(h); i < n_data; ++i)
            if (summary[i])
            {
                int p(data[i].successor(k));
                if (p != -1)
                    return p;
            }
        
        return max;
    }
};
