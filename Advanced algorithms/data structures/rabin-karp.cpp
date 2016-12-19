#include "../utility/utility.h"
#include <deque>

class RabinKarp
{
	typedef unsigned hash_t;

	Array<char> P;
	n_t n_P;
	hash_t hash_P{ 0 };

	hash_t nextHash(hash_t oldHash, char firstCharacter, char lastCharacter)
	{
		return (oldHash - (firstCharacter << n_P - 1)) * 2 + lastCharacter; // Implicit modulo
	}

public:
	RabinKarp(Array<char> P)
		: P(P)
	{
		n_P = std::size(P);
		for (char c : P)
			hash_P = hash_P * 2 + c; // Implicit modulo
	}

	std::deque<index_t> getMatches(const Array<char>& T) const
	{
		hash_t hash_T(0);
		for (unsigned i(0); i < n_P; ++i)
			hash_T = hash_T * 2 + T[i];
	}
};
