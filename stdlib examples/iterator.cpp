#include "../utility/utility.h"
#include <forward_list>
#include <iostream>
#include <iterator>


// Example of a custom iterator //
template<typename T>
struct LL
{
    class Iterator
    {
    protected:
        LL<T>* p_ll{nullptr};

    public:
        // Iterator requires these typedefs exist (or iterator_traits is specialised equivalently)
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;

        // InputIterator requires this is convertible to value_type
        // ForwardIterator requires that this is precisely value_type& (but why?)
        using reference = value_type&;

        // ForwardIterator requires a default constructor
        Iterator() = default;

        explicit Iterator(LL<T>* p_ll)
            : p_ll(p_ll)
        {}

        // Iterator requires this exists
        // InputIterator requires this returns reference
        // InputIterator requires it_a == it_b implies *it_a is equivalent to *it_b
        // InputIterator requires (void)*it, *it is equivalent to *it
        // OutputIterator requires this returns an assignable type
        reference operator*()
        {
            return p_ll->data;
        }

        const reference operator*() const
        {
            return p_ll->data;
        }

        // InputIterator requires this exists and it->m is equivalent to (*it).m
        T* operator->()
        {
            return &**this;
        }

        const T* operator->() const
        {
            return &**this;
        }

        // Iterator requires this exists and to return an Iterator&
        // ForwardIterator requires that (void)++Iterator(it), *it is equivalent to *it
        Iterator& operator++()
        {
            p_ll = p_ll->next;

            return *this;
        }

        // InputIterator and OutputIterator require this exists
        // InputIterator requires this to return a type whose operator* returns a type convertible to value_type
        // InputIterator and OutputIterator require this is equivalent to the following
        // OutputIterator requires this to return a type convertible to const Iterator&
        // OutputIterator requires this to return an assignable type such that *it++ = v is equivalent to *r = v; ++it;
        // ForwardIterator requires this to return an Iterator
        // ForwardIterator requires this to return a type whose operator* returns reference
        Iterator operator++(int)
        {
            return std::exchange(*this, ++*this);
        }

        // Iterator requires this exists, is an equivalence relation and returns a type contextually convertible to bool
        // ForwardIterator requires that it_a == it_b implies *it_a and *it_b refer to the same object or are both not dereferenceable
        // ForwardIterator requires that it_a == it_b implies ++it_a == ++it_b
        bool operator==(const Iterator& rhs) const
        {
            return p_ll == rhs.p_ll;
        }

        // InputIterator requires this exists and is equivalent to the following
        bool operator!=(const Iterator& rhs) const
        {
            return !(*this == rhs);
        }
    };

    LL* next;
    T data;

    LL(LL* next = {}, const T& data = {})
        : next(next), data(data)
    {}

    Iterator begin()
    {
        return Iterator(this);
    }

    Iterator end()
    {
        return {};
    }
};

void llIteratorExample()
{
    LL<int> d(nullptr, 3), c(&d, 2), b(&c, 1), a(&b, 0);

    for (int v : a)
        std::cout << v << '\n';

    std::forward_list<int> fl(std::begin(a), std::end(a));
    std::cout << fl << '\n';
}


template<typename Container0, typename Container1>
class ZipView
{
public:
    template<typename It0, typename It1>
    class Iterator
    {
        using Traits_It0 = std::iterator_traits<It0>;
        using Traits_It1 = std::iterator_traits<It1>;
        using value_type_It0 = typename Traits_It0::value_type;
        using value_type_It1 = typename Traits_It1::value_type;
        using reference_It0 = typename Traits_It0::reference;
        using reference_It1 = typename Traits_It1::reference;
        using pointer_It0 = typename Traits_It0::pointer;
        using pointer_It1 = typename Traits_It1::pointer;

    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = std::pair<value_type_It0, value_type_It1>;
        using difference_type = void;
        using pointer = std::pair<pointer_It0, pointer_It1>;

        // InputIterator requires this is convertible to value_type
        using reference = std::pair<reference_It0, reference_It1>;
        static_assert(std::is_convertible_v<reference, value_type>);

    private:
        It0 it0;
        It1 it1;

    public:
        Iterator(It0 it0, It1 it1)
            : it0(it0), it1(it1)
        {}

        // Iterator requires this exists
        // InputIterator requires this returns reference
        // InputIterator requires it_a == it_b implies *it_a is equivalent to *it_b
        // InputIterator requires (void)*it, *it is equivalent to *it
        // OutputIterator requires this returns an assignable type
        static_assert(std::is_assignable_v<reference, value_type>);
        reference operator*()
        {
            return {*it0, *it1};
        }

        // InputIterator requires this exists and it->m is equivalent to (*it).m
        pointer operator->()
        {
            return &**this;
        }

        // Iterator requires this exists and to return an Iterator&
        Iterator& operator++()
        {
            ++it0;
            ++it1;
            return *this;
        }

        // InputIterator and OutputIterator require this exists
        // InputIterator requires this to return a type whose operator* returns a type convertible to value_type
        // InputIterator and OutputIterator require this is equivalent to the following
        // OutputIterator requires this to return a type convertible to const Iterator&
        // OutputIterator requires this to return an assignable type such that *it++ = v is equivalent to *r = v; ++it;
        static_assert(std::is_convertible_v<std::result_of_t<decltype(&Iterator::operator*)(Iterator)>, value_type>);
        Iterator operator++(int)
        {
            return std::exchange(*this, ++*this);
        }

        // Iterator requires this exists, is an equivalence relation and returns a type contextually convertible to bool
        bool operator==(const Iterator& rhs)
        {
            return
                   it0 == rhs.it0
                || it1 == rhs.it1;
        }

        // InputIterator requires this exists and is equivalent to the following
        bool operator!=(const Iterator& rhs)
        {
            return !(*this == rhs);
        }
    };

private:
    Container0& data0;
    Container1& data1;

    template<typename It0, typename It1>
    Iterator<It0, It1> makeIterator(It0&& it0, It1&& it1)
    {
        return {std::forward<It0>(it0), std::forward<It1>(it1)};
    }

public:
    ZipView(Container0& data0, Container1& data1)
        : data0(data0), data1(data1)
    {}

    auto begin()
    {
        return makeIterator(std::begin(data0), std::begin(data1));
    }

    auto end()
    {
        return makeIterator(std::end(data0), std::end(data1));
    }
};

template<typename Container0, typename Container1>
ZipView<Container0, Container1> makeZipView(Container0& data0, Container1& data1)
{
    return {data0, data1};
}

template<typename It>
auto makeForwardList(It&& it0, It&& it1) -> std::forward_list<typename std::iterator_traits<It>::reference>
{
    return {std::forward<It>(it0), std::forward<It>(it1)};
}

void zipViewExample()
{
    std::vector<int> v0{1, 2, 3};
    std::vector<char> v1{'a', 'b', 'c'};
    auto zip(makeZipView(v0, v1));

    std::cout << makeForwardList(std::begin(zip), std::end(zip)) << '\n';
}

#if 0
int main()
{
    zipViewExample();
}
#endif
