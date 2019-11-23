#include "../utility/utility.h"

#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Indexer
{
    inline static n_t nextId, objectsAlive;
};

template<typename T>
class DebugAllocator : protected Indexer
{
    template<typename T>
    friend class DebugAllocator;

public:
    // Allocator requires this exists
    using value_type = T;

    using pointer = value_type*;

private:
    using char_buffer_t = char[sizeof(value_type)];

    index_t id;

    static std::string formatPointer(const void* p)
    {
        std::ostringstream s;
        s << "[93m0x" << std::hex << std::uppercase << uintptr_t(p) << "[0m";
        return s.str();
    }

    static std::string formatFunctionName(std::string_view functionName)
    {
        return "DebugAllocator::[96m" + std::string(functionName) + "[0m";
    }

    static std::string indentation()
    {
        constexpr n_t indentationSize(4);
        return std::string(objectsAlive * indentationSize, ' ');
    }

public:
    DebugAllocator() noexcept
        : id(nextId++)
    {
        std::cerr << indentation() << formatFunctionName(__func__) << '(' << id << ")\n";
        ++objectsAlive;
    }

    // Allocator requires this to construct that object that can deallocate rhs' allocations
    // Allocator requires this to not throw
    DebugAllocator(const DebugAllocator& rhs) noexcept
        : id(nextId++)
    {
        std::cerr << indentation() << formatFunctionName(__func__) << '(' << id << ", " << rhs.id << ")\n";
        ++objectsAlive;
    }

    // Allocator requires this to construct that object that can deallocate rhs' allocations
    // Allocator requires this to not throw
    DebugAllocator(DebugAllocator&& rhs) noexcept
        : id(nextId++)
    {
        std::cerr << indentation() << formatFunctionName(__func__) << '(' << id << ", move(" << rhs.id << "))\n";
        ++objectsAlive;
    }

    // Allocator requires this to construct that object that can deallocate rhs' allocations
    // Allocator requires this to not throw
    template<typename T_rhs>
    DebugAllocator(const DebugAllocator<T_rhs>& rhs) noexcept
        : id(nextId++)
    {
        std::cerr << indentation() << formatFunctionName(__func__) << '(' << id << ", " << rhs.id << ")\n";
        ++objectsAlive;
    }

    // Allocator requires this to construct that object that can deallocate rhs' allocations
    // Allocator requires this to not throw
    template<typename T_rhs>
    DebugAllocator(DebugAllocator<T_rhs>&& rhs) noexcept
        : id(nextId++)
    {
        std::cerr << indentation() << formatFunctionName(__func__) << '(' << id << ", move(" << rhs.id << "))\n";
        ++objectsAlive;
    }

    // Allocator requires this to construct that object that can deallocate rhs' allocations
    // Allocator requires this to not throw
    template<typename T_rhs>
    DebugAllocator& operator=(const DebugAllocator<T_rhs>& rhs) noexcept
    {
        std::cerr << indentation() << id << " = " << formatFunctionName(__func__) << '(' << id << ", " << rhs.id << ")\n";
    }

    // Allocator requires this to construct that object that can deallocate rhs' allocations
    // Allocator requires this to not throw
    template<typename T_rhs>
    DebugAllocator& operator=(DebugAllocator<T_rhs>&& rhs) noexcept
    {
        std::cerr << indentation() << id << " = " << formatFunctionName(__func__) << '(' << id << ", move(" << rhs.id << "))\n";
    }

    ~DebugAllocator() noexcept
    {
        --objectsAlive;
        std::cerr << indentation() << formatFunctionName(__func__) << '(' << id << ")\n";
    }

    // Allocator requires this to be true only if rhs can deallocate memory provided by *this
    // Allocator requires this to be an equivalence relation.
    // Allocator requires this to not throw
    template<typename T_rhs>
    bool operator==(const DebugAllocator<T_rhs>& rhs) const noexcept
    {
        std::cerr << indentation() << std::boolalpha << true << " = " << formatFunctionName(__func__) << '(' << id << ", " << rhs.id << ")\n";
        return true;
    }

    // Allocator requires this to return !(*this == rhs)
    template<typename T_rhs>
    bool operator!=(const DebugAllocator<T_rhs>& rhs) const noexcept
    {
        std::cerr << indentation() << std::boolalpha << false << " = " << formatFunctionName(__func__) << '(' << id << ", " << rhs.id << ")\n";
        return false;
    }

    // Allocator requires this to allocate memory for n objects of value_type, but to not construct them
    pointer allocate(size_t n) const
    {
        value_type* const p(reinterpret_cast<value_type*>(new char_buffer_t[n]));
        std::cerr << indentation() << formatPointer(p) << " = " << formatFunctionName(__func__) << '(' << id << ", " << n << ")\n";
        return p;
    }

    // Allocator requires this to deallocate memory for (p,n) where p = allocate(n) was previously requested
    // Allocator requires this to not throw
    void deallocate(pointer p, size_t n) const noexcept
    {
        std::cerr << indentation() << formatFunctionName(__func__) << '(' << id << ", " << formatPointer(p) << ", " << n << ")\n";
        delete[] reinterpret_cast<char_buffer_t*>(p);
    }

    DebugAllocator select_on_container_copy_construction() const noexcept
    {
        std::cerr << indentation() << formatFunctionName(__func__) << '(' << id << ")\n";
        return {};
    }

    friend void swap(DebugAllocator& lhs, DebugAllocator& rhs) noexcept
    {
        std::cerr << indentation() << formatFunctionName(__func__) << '(' << lhs.id << ", " << rhs.id << ")\n";
    }
};

#if 0
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

    decltype(v) v_copy(v);
    std::cout << "Copied\n";

    swap(v, v_copy);
    std::cout << "Swapped\n";
}
#endif
