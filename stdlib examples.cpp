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
    using iterator_t = ll_iterator<T>;

    LL* next;
    T data;

    LL(LL* next = {}, const T& data = {})
        : next(next), data(data)
    {}

    iterator_t begin()
    {
        return iterator_t(this);
    }

    iterator_t end()
    {
        return iterator_t();
    }
};

template<typename T>
class ll_iterator
{
protected:
    LL<T>* p_ll;

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
    explicit ll_iterator(LL<T>* p_ll = nullptr)
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

    // Iterator requires this exists and to return an ll_iterator&
    // ForwardIterator requires that (void)++ll_iterator(it), *it is equivalent to *it
    ll_iterator& operator++()
    {
        p_ll = p_ll->next;

        return *this;
    }

    // InputIterator and OutputIterator require this exists
    // InputIterator requires this to return a type whose operator* returns a type convertible to value_type
    // InputIterator and OutputIterator require this is equivalent to the following
    // OutputIterator requires this to return a type convertible to const ll_iterator&
    // OutputIterator requires this to return an assignable type such that *it++ = v is equivalent to *r = v; ++it;
    // ForwardIterator requires this to return an ll_iterator
    // ForwardIterator requires this to return a type whose operator* returns reference
    ll_iterator operator++(int)
    {
        return std::exchange(*this, ++*this);
    }

    // Iterator requires this exists, is an equivalence relation and returns a type contextually convertible to bool
    // ForwardIterator requires that it_a == it_b implies *it_a and *it_b refer to the same object or are both not dereferenceable
    // ForwardIterator requires that it_a == it_b implies ++it_a == ++it_b
    bool operator==(const ll_iterator& rhs) const
    {
        return p_ll == rhs.p_ll;
    }

    // InputIterator requires this exists and is equivalent to the following
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
	for (;;)
	{
		std::cout << "Spell definitely\n";
		std::string line;
		std::getline(std::cin, line);

		try
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
		catch (const std::regex_error& e)
		{
			std::cerr << "Regex error in match example\n";
		}

		try
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
		catch (const std::regex_error& e)
		{
			std::cerr << "Regex error in search example\n";
		}

		try
		{
			std::cout << "These are the known mispellings of definitely that occurred:\n";
			for (std::sregex_iterator it_match(std::begin(line), std::end(line), pattern), it_match_end{}; it_match != it_match_end; ++it_match)
				std::cout << '\t' << (*it_match)[0] << '\n';
		}
		catch (const std::regex_error& e)
		{
			std::cerr << "Regex error in regex_iterator example\n";
		}

		try
		{
			std::cout << "Here is the correct version:\n";
			std::cout << std::regex_replace(line, pattern, "definitely") << '\n';

			std::cout << "Enter two words and they will be swapped:\n";
			std::string words;
			std::getline(std::cin, words);
			std::cout << std::regex_replace(words, std::regex("(.+) (.+)"), "$2 $1\n");
		}
		catch (const std::regex_error& e)
		{
			std::cerr << "Regex error in replace example\n";
		}
	}
}

#if 1
int main()
{
    ll_iterator_example();
}
#endif
