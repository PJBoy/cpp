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
Priority queues use a container with front and {push|pop}_back, which mask <algorithm>'s heap functions on the underlying container
make_heap is O(n)
push_heap is O(log(n))
pop_heap is O(log(n))
*/


#include "typedefs.h"
#include <deque>
#include <memory>
#include <type_traits>


struct KV
{
    int k, v;
    bool init{false};

    KV() = default;

    KV(int k, int v)
        : k(k), v(v), init(true)
    {}

    KV(std::pair<int, int> kv)
        : k(kv.first), v(kv.second),  init(true)
    {}

    bool operator==(int rhs) const
    {
        return k == rhs;
    }

    operator bool() const
    {
        return init;
    }
};


template<typename T>
class Array
{
    n_t n;
    std::unique_ptr<T[]> data;

    index_t i{0};

public:
    Array() = default;

    Array(n_t n)
        : n(n)
    {
        data.reset(new T[n]);
    }

    Array(const Array& rhs)
        : Array(rhs.n)
    {
        std::copy(std::cbegin(rhs), std::cend(rhs), std::begin(*this));
    }

    Array& operator=(const Array& rhs)
    {
        n = rhs.n;
        data.reset(new T[n]);
        std::copy(std::cbegin(rhs), std::cend(rhs), std::begin(*this));
		return *this;
    }

    const T& operator[](index_t i) const
    {
        return data[i];
    }

    T& operator[](index_t i)
    {
        return data[i];
    }

    T* begin()
    {
        return &data[0];
    }

    const T* begin() const
    {
        return &data[0];
    }

    const T* cbegin() const
    {
        return begin();
    }

    T* end()
    {
        return &data[n];
    }

    const T* end() const
    {
        return &data[n];
    }

    const T* cend() const
    {
        return end();
    }

    n_t size() const
    {
        return n;
    }

    void push_back(const T& value)
    {
        if (i >= n)
            throw std::out_of_range("Array::push_back: pushed too many values");

        data[i++] = value;
    }

    T pop_back()
    {
        if (i < 1)
            throw std::out_of_range("Array::pop_back: popped too many values");

        return data[--i];
    }

    T* current()
    {
        return &data[i];
    }

    const T* current() const
    {
        return &data[i];
    }

    const T* ccurrent() const
    {
        return &data[i];
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
