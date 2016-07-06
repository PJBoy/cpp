#include "../utility/utility.h"


// Wrapper class for allowing type distinction between variables with the same dimensions (e.g. radians and degrees)
template<typename T>
class Wrapper
{
protected:
    T v;

public:
    Wrapper() = default;
    
    Wrapper(T v)
        : v(v)
    {}
    
    operator T()
    {
        return v;
    }

#define operator(op)                   \
    Wrapper& operator##op (Wrapper rhs)\
    {                                  \
        v op rhs;                      \
        return *this;                  \
    }
    operator(+=)
    operator(-=)
    operator(*=)
    operator(/=)
    operator(%=)
    operator(&=)
    operator(|=)
    operator(^=)
    operator(<<=)
    operator(>>=)
#undef operator
};

// Example usage
class Int : public Wrapper<int>
{
    using Wrapper::Wrapper;
};


namespace misc
{
template<typename T>
class Array
{
    unsigned n;
    std::unique_ptr<T[]> data;

    unsigned i{0};

public:
    Array() = default;

    Array(unsigned n)
        : n(n)
    {
        data.reset(new T[n]);
    }

    Array(const Array& rhs)
    {
        n = rhs.n;
        i = rhs.i;
        data.reset(new T[n]);
        std::copy(std::cbegin(rhs), std::cend(rhs), std::begin(*this));
    }

    Array(Array&&) = default;

    Array& operator=(const Array& rhs)
    {
        n = rhs.n;
        i = rhs.i;
        data.reset(new T[n]);
        std::copy(std::cbegin(rhs), std::cend(rhs), std::begin(*this));

        return *this;
    }

    Array& operator=(Array&&) = default;

    const T& operator[](unsigned i) const
    {
        assert("Array::operator[] const: i >= n" && i < n);

        return data[i];
    }

    T& operator[](unsigned i)
    {
        assert("Array::operator[] const: i >= n" && i < n);

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

    std::reverse_iterator<T*> rbegin()
    {
        return std::make_reverse_iterator(end());
    }

    std::reverse_iterator<const T*> rbegin() const
    {
        return std::make_reverse_iterator(end());
    }

    std::reverse_iterator<const T*> crbegin() const
    {
        return rbegin();
    }

    std::reverse_iterator<T*> rend()
    {
        return std::make_reverse_iterator(begin());
    }

    std::reverse_iterator<const T*> rend() const
    {
        return std::make_reverse_iterator(begin());
    }

    std::reverse_iterator<const T*> crend() const
    {
        return rend();
    }

    unsigned size() const
    {
        return n;
    }

    void push_back(const T& value)
    {
        assert("Array::push_back: pushed too many values" && i < n);

        data[i++] = value;
    }

    T pop_back()
    {
        assert("Array::pop_back: popped too many values" && i > 0);

        return data[--i];
    }

    T* back()
    {
        return &data[i];
    }

    unsigned back_i() const
    {
        return i;
    }
};


template<typename T>
class CircularArray
{
    Array<T> data;
    n_t n;
    unsigned size{0};
    unsigned next{0};

    index_t i_begin() const
    {
        if (size < n_window)
            return 0;

        return next;
    }

    unsigned pmod(signed x, unsigned n)
    {
        return (x % n + n) % n;
    }

public:
    CircularArray(n_t n)
        : n(n)
    {
        window = Array<T>(n);
    }

    T& operator[](signed i)
    {
        return data[pmod(i_begin() + i, % n)];
    }

    const T& operator[](signed i) const
    {
        return data[pmod(i_begin() + i, n)];
    }

    void push(const T& value)
    {
        data[next] = value;
        next = (next + 1) % n;
        size = std::min(size + 1, n);
    }

    T sum() const
    {
        return std::accumulate(std::cbegin(data), std::cbegin(data) + size, T());
    }

    T mean() const
    {
        return sum() / size;
    }
};


template<typename T>
class MultiArray
{
    Array<T> data;
    unsigned n;                  // Size of all data (product of dimensions)
    Array<unsigned> dimensions;  // Dimensions of the multiarray, used for bounds checking
    Array<unsigned> multipliers; // Multipliers a_i such that coordinates x_i access data[m] where m = x_0 + sum_{i=1} a_{i-1} x_i

public:
    MultiArray() = default;

    MultiArray(std::initializer_list<unsigned> dimensions_in)
    {
        const unsigned n_dimensions(unsigned(std::size(dimensions_in)));

        dimensions = Array<unsigned>(n_dimensions);
        std::copy(std::cbegin(dimensions_in), std::cend(dimensions_in), std::begin(dimensions));

        multipliers = Array<unsigned>(n_dimensions - 1);
        std::partial_sum(std::crbegin(dimensions), std::crend(dimensions) - 1, std::rbegin(multipliers), std::multiplies<>());

        n = dimensions[0] * multipliers[0];
        data = Array<T>(n);
    }

    T& operator[](std::initializer_list<unsigned> coordinates)
    {
        assert("MultiArray::operator[]: coordinate_i >= dimension_i" && std::inner_product(std::cbegin(coordinates), std::cend(coordinates), std::cbegin(dimensions), true, std::logical_and<>(), std::less<>()));

        const unsigned i = std::inner_product(std::cbegin(coordinates), std::cend(coordinates) - 1, std::cbegin(multipliers), *std::crbegin(coordinates));
        return data[i];
    }

    const T& operator[](std::initializer_list<unsigned> coordinates) const
    {
        assert("MultiArray::operator[]: coordinate_i >= dimension_i" && std::inner_product(std::cbegin(coordinates), std::cend(coordinates), std::cbegin(dimensions), true, std::logical_and<>(), std::less<>()));

        const unsigned i = std::inner_product(std::cbegin(coordinates), std::cend(coordinates) - 1, std::cbegin(multipliers), *std::crbegin(coordinates));
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

    unsigned size() const
    {
        return n;
    }
};
};
