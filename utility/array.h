#include "typedefs.h"

#include <algorithm>
#include <iterator>
#include <memory>


template<typename T>
class Array
{
    n_t n{};
    std::unique_ptr<T[]> data;

    index_t i{}; // Drop this

public:
    Array() = default;

    Array(n_t n)
        : n(n), data(std::make_unique<T[]>(n))
    {}

    template<typename It>
    Array(It begin, It end)
        : n(std::distance(begin, end)), data(std::make_unique<T[]>(n))
    {
        std::copy(begin, end, std::begin(*this));
    }

    Array(const Array& rhs)
        : Array(rhs.n)
    {
        std::copy(std::begin(rhs), std::end(rhs), std::begin(*this));
    }

    Array& operator=(const Array& rhs)
    {
        n = rhs.n;
        data = std::make_unique<T[]>(n);
        std::copy(std::begin(rhs), std::end(rhs), std::begin(*this));
        return *this;
    }

    Array(Array&& rhs) noexcept
        : n(rhs.n), data(std::move(rhs.data))
    {}

    Array& operator=(Array&& rhs) noexcept
    {
        n = rhs.n;
        data = std::move(rhs.data);
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

template<typename It>
Array(It, It) -> Array<typename std::iterator_traits<It>::value_type>;
