#include "../utility/utility.h"
#include <algorithm>
#include <array>
#include <fstream>
#include <functional>
#include <limits>
#include <tuple>
#include <type_traits>
#include <utility>


float sum_accurate(const float x[], n_t n)
{
    float sum_float;
    double r[64]{}; // Double precision cascading accumulators
    
    // Loop to process all the numbers
    for (index_t i(0); i < n; ++i)
    {
        // Remove the sign bit
        float v(std::abs(x[i]));

        // Shift the number, regarded as an integer, 23 places right to remove the mantissa and leave the exponent at right of 32-bit field.
        // Then divide by 4 (shift right 2 places) as there will be 4 consecutive exponents sharing 1 accumulator
        const index_t iexp
        (
            std::numeric_limits<float>::is_iec559
                ? (int&)v >> 25
                : std::ilogb(v) - std::numeric_limits<float>::min_exponent + 2 >> 2
        );

        // Add the original number to the appropriate accumulator
        r[iexp] += x[i];
    }

    // Sum of all accumulators
    // Add the accumulators in decreasing order
    double sum_double = 0.0;
    for (index_t i(64); i --> 0;)
        sum_double += r[i];
    sum_float = float(sum_double);

    // Find the exponent of the sum, ready for correction
    float v(std::abs(sum_float));
    const index_t iexp
    (
        std::numeric_limits<float>::is_iec559
            ? (int&)v >> 25
            : std::ilogb(v) - std::numeric_limits<float>::min_exponent + 2 >> 2
    );

    // Subtract the double sum from appropriate accumulator
    r[iexp] -= sum_double;

    // Final correction term
    // Add all the accumulators (including the modified one)
    double delta = 0.0;
    for (index_t i(64); i --> 0;)
        delta += r[i];

    // Add the correction to the sum
    sum_double += delta;

    // Exit the sum function, returning the sum in single precision
    return float(sum_double);
}
template<n_t n>
float sum_accurate(const float(&x)[n]) { return sum_accurate(x, n); }

double sum_accurate(double x[], n_t n)
{
    std::sort(x, &x[n], std::greater<>());

    double
        s = x[0],
        c = 0;

    for (index_t i(1); i < n; ++i)
    {
        const double
            cx = c + x[i],
            cxs = cx + s,
            cx_e = x[i] - (cx - c),
            cxs_e = cx - (cxs - s),
            e = cx_e + cxs_e;
        s = cxs + e;
        c = e - (s - cxs);
    }

    return s;
}
template<n_t n>
double sum_accurate(const double(&x)[n]) { return sum_accurate(x, n); }


template<typename InputIt, typename T>
InputIt find_circular(InputIt begin, InputIt end, InputIt first, const T& value)
{
    InputIt i(std::find(first, end, value));
    return i != end ? i : std::find(begin, first, value);
}


template<typename OutputIt, typename Size, typename T>
OutputIt iota_n(OutputIt first, Size count, T value)
{
    for (Size i(count); i > 0; --i)
    {
        *first++ = value;
        ++value;
    }
    return first;
}


std::string get_file_contents(const char* filepath)
{
    std::string contents;

    std::ifstream f(filepath, std::ios::in | std::ios::binary);
    f.exceptions(std::ios::badbit | std::ios::failbit);
    f.seekg(0, std::ios::end);
    contents.resize(f.tellg());
    f.seekg(0, std::ios::beg);
    f.read(std::data(contents), std::size(contents));

    return contents;
}


template<n_t N, typename... T, index_t... index>
auto zip_helper(const std::tuple<std::array<T, N>...>& arrays, std::index_sequence<index...>) -> std::array<std::tuple<T...>, N>
{
    std::array<std::tuple<T...>, N> ret;
    for (index_t i(0); i < N; ++i)
        ret[i] = std::make_tuple(std::get<index>(arrays)[i]...);

    return ret;
}

template<n_t N, typename... T>
auto zip(const std::tuple<std::array<T, N>...>& arrays) -> std::array<std::tuple<T...>, N>
{
    return zip_helper(arrays, std::index_sequence_for<T...>());
}


template<n_t N, typename... T, index_t... index>
auto unzip_helper(const std::array<std::tuple<T...>, N>& array, std::index_sequence<index...>) -> std::tuple<std::array<T, N>...>
{
	std::tuple<std::array<T, N>...> ret;
	for (index_t i(0); i < N; ++i)
		std::tie(std::get<index>(ret)[i]...) = std::tie(std::get<index>(array[i])...);

	return ret;
}

template<n_t N, typename... T>
auto unzip(const std::array<std::tuple<T...>, N>& array) -> std::tuple<std::array<T, N>...>
{
	return unzip_helper(array, std::index_sequence_for<T...>());
}


class Range
{
public:
	class RangeIterator
	{
		const n_t step;
		index_t i;

	public:
		RangeIterator(index_t i, n_t step)
			: i(i), step(step)
		{}

		index_t operator*() const
		{
			return i;
		}

		void operator++()
		{
			i += step;
		}

		bool operator!=(const RangeIterator& rhs) const
		{
			return **this < *rhs;
		}
	};

private:
	const index_t i_begin{0}, i_end;
	const n_t step{1};

public:
	Range(n_t i_end)
		: i_end(i_end)
	{}

	Range(index_t i_begin, index_t i_end, n_t step = 1)
		: i_begin(i_begin), i_end(i_end), step(step)
	{}

	RangeIterator begin() const
	{
		return {i_begin, step};
	}

	RangeIterator end() const
	{
		return {i_end, step};
	}
};


// Take forward iterators and demonstrate a double buffering example :)
void prepend(int* data, n_t n_data, const int header[], n_t n_header)
{
/*
    Read block 0 (as it will be overwritten by header)
    Overwrite block 0 with header
    Read block 1 (as it will be overwritten by block 0)
    Overwrite block 1 with block 0

    Read block i+1 (as it will be overwritten by block i)
    Overwrite block i+1 with block i
*/

    const int* data_in(data);
    int* data_out(data);

    const n_t n_buffer(10);
    int buffers[2][n_buffer];
    index_t i_buffers(0);

    const auto read([&](n_t n)
    {
    /*
        int(&buffer)[n_buffer](buffers[i_buffers]);
        std::copy(data_in, data_in + n, std::begin(buffer));
    /*/
        std::copy(data_in, data_in + n, std::begin(buffers[i_buffers]));
    //*/
        data_in += n;
    });
    const auto write([&](n_t n)
    {
    /*
        const int(&buffer)[n_buffer](buffers[i_buffers ^ 1]);
        std::copy(std::begin(buffer), std::begin(buffer) + n, data_out);
    /*/
        std::copy(std::begin(buffers[i_buffers ^ 1]), std::begin(buffers[i_buffers ^ 1]) + n, data_out);
    //*/
        data_out += n;
    });

    read(n_buffer);
    std::copy(header, header + n_header, data_out);
    data_out += n_header;
    i_buffers ^= 1;

    for (index_t i_block(0); i_block < n_data / n_buffer - 1; ++i_block)
    {
        read(n_buffer);
        write(n_buffer);
        i_buffers ^= 1;
    }

    read(n_data % n_buffer);
    write(n_buffer);
    i_buffers ^= 1;
    write(n_data % n_buffer);
}


#if 0
#include <iostream>
#include <numeric>

int main()
{
	int data[33];
	std::iota(std::begin(data), std::end(data), 0);
    int header[7];
    std::iota(std::begin(header), std::end(header), 100);

    prepend(data, std::size(data) - std::size(header), header, std::size(header));
    std::cout << data << '\n';
}
#endif
