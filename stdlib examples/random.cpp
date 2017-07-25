#include "../utility/utility.h"
#include <random>


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
