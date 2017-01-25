#include "../utility/utility.h"
#include <deque>


class KMP
{
    const char* P;
    n_t n_P;
    Array<index_t> border;

public:
    KMP(const char P[], n_t n)
        : P(P), n_P(n)
    {
        // Generate border[0..n]

        // border[i] is the length of the longest proper prefix/suffix pair in P[0..i-1]
        // It is the index into the pattern to reset to on mismatch of letter P[i]

        // Small modification on preMp. Adds a recursive step???
        // Algorithm works by matching the pattern to itself at a different offset, using border[0..j] when determining border[i+1]

        border = Array<index_t>(n + 1);

        border[0] = -1;

        if (n == 0)
            return;

        border[1] = 0;

        // 0 <= j < i <= n
        // P[0..j] is the prefix
        // P[i-j..i] is the suffix
        for (index_t i(1), j(0); i < n;)
        {
            // Determine mismatch or shift pattern to the longest known partial match (of previous characters)
            while (j != -1 && P[i] != P[j])
                j = border[j];

            ++i, ++j;
            if (P[i] == P[j]) // <-- Knuth step
                border[i] = border[j];  // If next characters align, then this suffix is no different to the prefix, so use that instead
                                        // If the suffix matches (inc. next character) then this border[i] isn't used. If it doesn't match, then shifting to border[j] is redundant
            else
                border[i] = j;
        }
    }


    std::deque<index_t> getMismatches(const char T[], n_t n_T)
    {
        std::deque<index_t> matches;

        for (index_t i_T(0), i_P(0); i_T < n_T;)
        {
            // Get next alignment or determine mismatch
            while (i_P != -1 && P[i_P] != T[i_T])
                i_P = border[i_P];

            ++i_P;
            ++i_T;
            if (i_P >= n_P)
            {
                matches.push_back(i_T - i_P);
                i_P = border[i_P];
            }
        }

        return matches;
    }
};
