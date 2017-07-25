#include "../utility/utility.h"
#include <iostream>
#include <iterator>
#include <regex>


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

#if 0
int main()
{
    regex();
}
#endif
