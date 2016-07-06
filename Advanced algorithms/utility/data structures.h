#pragma once
#include "typedefs.h"
#include <memory>


struct KV
{
    int k, v;
    bool init{false};

    KV() = default;

    KV(int k, int v)
        : k(k), v(v), init(true)
    {}

    KV(std::pair<int, int> kv)
        : k(kv.first), v(kv.second)
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
