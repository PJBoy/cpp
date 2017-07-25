#include "../utility/utility.h"
#include <chrono>
#include <iostream>


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


#if 0
int main()
{
    time();
}
#endif
