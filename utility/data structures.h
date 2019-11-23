#pragma once
/*
    container||_____random______||_______end_______||                   summary                    ||   implementation   ||                                           notes
    _________||_access_|_insert_||_access_|_insert_||______________________________________________||____________________||__________________________________________________________________________________________
     vector  || O(1)   | O(n)   || O(1)   | O(n)/n || fast except slow insert,        continuous   || resizable array    ||
     deque   || O(1)   | O(n)   || O(1)   | O(1)   || fast except slow random insert, discontinuous|| array of arrays    ||
     list    || O(n)   | O(1)   || O(1)   | O(1)   || fast except slow random access, discontinuous|| doubly linked list || forward_list exists as a constant space optimisation that only supports forward iteration

    ___container___||_operations_||__________summary___________||_______implementation________||_____________________________________________________________notes____________________________________________________________
     set           || O(log(n))  || supports {prede|suc}cessor || balanced binary search tree || map stores kv-pairs and provides operator[] like Python, multiset allows for cardinality, multimap allows for non unique keys
     unordered_set || O(n)/n     || has bucket interface       || hash table                  || as above

    Stacks use a container with back and {push|pop}_back (basically not forward_list)
    Queues use a container with front, back, push_back and pop_front (deque or list)
    Priority queues use a container with front and {push|pop}_back, which apply <algorithm>'s heap functions on the underlying container
        make_heap is O(n)
        push_heap is O(log(n))
        pop_heap is O(log(n))
*/


#include "typedefs.h"

#include <deque>
#include <memory>
#include <type_traits>


template<typename Key, typename Value>
struct KV
{
    Key k;
    Value v;
    bool init{};

    constexpr KV() = default;

    constexpr KV(Key k, Value v)
        : k(k), v(v), init(true)
    {}

    constexpr KV(std::pair<Key, Value> kv)
        : k(kv.first), v(kv.second), init(true)
    {}

    constexpr bool operator==(Key rhs) const noexcept
    {
        return k == rhs;
    }

    constexpr operator bool() const noexcept
    {
        return init;
    }
};


struct Graph
{
	struct Edge
	{
		index_t i_end;
		unsigned weight;

		Edge(index_t i_end, unsigned weight)
			: i_end(i_end), weight(weight)
		{}
	};

	struct Vertex
	{
		std::deque<Edge> edges;

		Vertex(decltype(edges) edges = {})
			: edges(edges)
		{}
	};

	std::deque<Vertex> vertices;
};
