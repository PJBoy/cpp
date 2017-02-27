#include "utility/utility.h"
#include <chrono>
#include <forward_list>
#include <iostream>
#include <iterator>
#include <random>
#include <regex>


// Examples for random number generation //
template<typename T = int>
T uniform_random_int(T begin, T end)
{
    // std::random_device{} is a random number generator, operator() gives a random number used as a seed
    // UPRNG is a uniform psuedo random number generator using the seed
    // The bind is useful when requesting many random numbers

    static std::default_random_engine UPRNG(std::random_device{}());

    std::uniform_int_distribution<T> UID(begin, end);
    std::function<T()> uniform_random_int(std::bind(UID, UPRNG));

    return uniform_random_int();
}

bool coin_flip()
{
    static std::default_random_engine UPRNG(std::random_device{}());
    
    return std::bernoulli_distribution()(UPRNG);
}

template<typename RandomIt>
void shuffle(RandomIt begin, RandomIt end)
{
    std::shuffle(begin, end, std::default_random_engine(std::random_device{}()));
}


// Example for timing with the clock //
void time()
{
    using clock = std::chrono::high_resolution_clock;
    clock::time_point clock_start, clock_end;
    
    clock_start = clock::now();

    // --- code --- //

    clock_end = clock::now();

    std::cout << "Elapsed time: " << std::chrono::duration<double>(clock_end - clock_start).count() << " (s)\n";
}


// Example of a custom iterator //
template<typename T>
class ll_iterator;

template<typename T>
struct LL
{
    LL* next;
    T data;

    LL(LL* next = {}, const T& data = {})
        : next(next), data(data)
    {}

    ll_iterator<T> begin()
    {
        return this;
    }

    ll_iterator<T> end()
    {
        return {};
    }
};

template<typename T>
class ll_iterator : public std::iterator<std::input_iterator_tag, int>
{
protected:
    LL<T>* p_ll;

public:
    ll_iterator(LL<T>* p_ll = nullptr)
        : p_ll(p_ll)
    {}

    T& operator*()
    {
        return p_ll->data;
    }

    const T& operator*() const
    {
        return p_ll->data;
    }

    T* operator->()
    {
        return &p_ll->data;
    }

    const T* operator->() const
    {
        return &p_ll->data;
    }

    ll_iterator& operator++()
    {
        p_ll = p_ll->next;

        return *this;
    }

    ll_iterator operator++(int)
    {
        return std::exchange(*this, ++*this);
    }

    bool operator==(const ll_iterator& rhs) const
    {
        return p_ll == rhs.p_ll;
    }

    bool operator!=(const ll_iterator& rhs) const
    {
        return !(*this == rhs);
    }
};

void ll_iterator_example()
{
    LL<int> d(nullptr, 3), c(&d, 2), b(&c, 1), a(&b, 0);

    for (int v : a)
        std::cout << v << '\n';

    std::forward_list<int> fl(std::begin(a), std::end(a));
    std::cout << fl << '\n';
}

// Example of regular expressions //
void regex()
{
	const std::regex pattern("defi(?:n(?:ate|[ae]n?t)|ant)ly", std::regex::icase | std::regex::optimize);
	for (std::string line; std::getline(std::cin, line);)
	{
		{
			if (std::regex_match(line, pattern))
				std::cout << "That was a known mispelling of definitely\n";

			std::smatch match;
			if (std::regex_match(line, match, pattern))
				std::cout << "That was a known mispelling of definitely. List of submatches:\n";

			std::string separator;
			for (const std::ssub_match& subMatch : match)
				std::cout << '\t' << subMatch << '\n';

			std::cout << "Match 0 is: " << match[0] << '\n';
		}

		{
			if (std::regex_search(line, pattern))
				std::cout << "That contained a known mispelling of definitely\n";

			std::smatch match;
			if (std::regex_search(line, match, pattern))
				std::cout << "That contained a known mispelling of definitely. List of submatches:\n";

			for (const std::ssub_match& subMatch : match)
				std::cout << '\t' << subMatch << '\n';

			std::cout << "Match 0 is: " << match[0] << '\n';
		}

		{
			std::cout << "These are the known mispellings of definitely that occurred:\n";
			for (std::sregex_iterator it_match(std::begin(line), std::end(line), pattern), it_match_end{}; it_match != it_match_end; ++it_match)
				std::cout << '\t' << (*it_match)[0] << '\n';
		}
	}
}

#if 1
int main()
{
	regex();
}
#endif
