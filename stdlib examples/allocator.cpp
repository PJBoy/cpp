#include "../utility/utility.h"
#include <ios>
#include <iostream>
#include <string>
#include <vector>

template<typename T>
class DebugAllocator
{
public:
    // Allocator requires this exists
    using value_type = T;

    using pointer = value_type*;
    using size_type = std::make_unsigned_t<typename std::pointer_traits<pointer>::difference_type>;

    DebugAllocator() noexcept
    {
        std::clog << "DebugAllocator::DebugAllocator(0x" << std::hex << this << ")\n";
    }

    template<typename T_rhs>
    DebugAllocator(const DebugAllocator<T_rhs>& rhs) noexcept
    {
        std::clog << "DebugAllocator::DebugAllocator(0x" << std::hex << this << ", 0x" << &rhs << ")\n";
    }

    template<typename T_rhs>
    DebugAllocator(DebugAllocator<T_rhs>&& rhs) noexcept
    {
        std::clog << "DebugAllocator::DebugAllocator(0x" << std::hex << this << ", move(0x" << &rhs << "))\n";
    }

    template<typename T_rhs>
    DebugAllocator& operator=(const DebugAllocator<T_rhs>& rhs) noexcept
    {
        std::clog << std::hex << this << " = DebugAllocator::operator=(0x" << this << ", 0x" << &rhs << ")\n";
    }

    template<typename T_rhs>
    DebugAllocator& operator=(const DebugAllocator<T_rhs>&& rhs) noexcept
    {
        std::clog << std::hex << this << " = DebugAllocator::operator=(0x" << this << ", move(0x" << &rhs << "))\n";
    }

    ~DebugAllocator() noexcept
    {
        std::clog << "DebugAllocator::~DebugAllocator(0x" << std::hex << this << ")\n";
    }

    // Allocator requires this to allocate memory for n objects of value_type, but to not construct them
    pointer allocate(size_type n) const
    {
        value_type* const p((value_type*) new char[sizeof(value_type) * n]);
        std::clog << std::hex << p << " = DebugAllocator::allocate(0x" << this << ", " << std::dec << sizeof(value_type) * n << ")\n";
        return p;
    }

    // Allocator requires this to deallocate memory for (p,n) where p = allocate(n) was previously requested
    // Allocator requires this to not throw
    void deallocate(pointer p, size_type n) const noexcept
    {
        std::clog << "DebugAllocator::deallocate(0x" << std::hex << this << ", 0x" << p << ", " << std::dec << sizeof(value_type) * n << ")\n";
        delete[] (value_type*) p;
    }

    // Allocator requires this to be true only if rhs can deallocate memory provided by *this
    // Allocator requires this to be an equivalence relation.
    // Allocator requires this to not throw
    bool operator==(const DebugAllocator& rhs) const noexcept
    {
        std::clog << std::boolalpha << true << " = DebugAllocator::operator==(" << std::hex << this << ", 0x" << &rhs << ")\n";
        return true;
    }

    // Allocator requires the following
    bool operator!=(const DebugAllocator& rhs) const noexcept
    {
        std::clog << std::boolalpha << true << " = DebugAllocator::operator!=(" << std::hex << this << ", 0x" << &rhs << ")\n";
        return false;
    }
};

#if 1
int main()
{
    std::vector<int, DebugAllocator<int>> v{3};
    v.push_back(1);
    v.push_back(2);
    v.emplace_back(1);
    v.insert(std::begin(v) + 2, 4);
    v.erase(std::begin(v) + 3);

    std::string separator;
    for (int& x : v)
    {
        std::cout << separator << std::move(x);
        separator = ", ";
    }

    std::cout << '\n';
}
#endif
