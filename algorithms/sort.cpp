#include "../utility/utility.h"


namespace sort
{
    // Insertion sort //
    template<typename T>
    void insertion(T a[], n_t n)
    {
        for (index_t r(1); r < n; r++)
        {
            T v(a[r]);
            index_t l;
            for (l = r; l && v < a[l - 1]; --l)
                a[l] = a[l - 1];

            a[l] = v;
        }
    }
    template<typename T, n_t N>
    void insertion(T(&a)[N]){ insertion(a, N); }

    template<typename T>
    void insertionWithSwaps(T a[], n_t n)
    {
        for (index_t r(1); r < n; r++)
            for (index_t l(r); l && a[l] < a[l - 1]; --l)
                std::swap(a[l], a[l - 1]);
    }
    template<typename T, n_t N>
    void insertionWithSwaps(T(&a)[N]){ insertionWithSwaps(a, N); }


    // Heap sort //
    template<typename T>
    void heap(T a[], n_t n)
    {
        std::make_heap(a, a + n);
        std::sort_heap(a, a + n);
    }
    template<typename T, n_t n>
    void heap(T(&a)[n]){ heap(a, n); }

    
    // Merge sort //
    template<typename T>
    void merge(T a[], n_t n)
    {
        for (n_t n_sub(1); n_sub < n; n_sub *= 2)
            for (index_t i{}; i < n; i += n_sub * 2)
            {
                // Merge a[i..i+n_sub) and a[i+n_sub..i+n_sub*2) to a[i..i+n_sub*2)
                T* const begin(a + i);
                T* const middle(a + std::min(n, i + n_sub));
                T* const end(a + std::min(n, i + n_sub * 2));
                
                std::inplace_merge(begin, middle, end, begin);
            }
    }
    template<typename T, n_t n>
    void merge(T(&a)[n]){ merge(a, n); }


    // Quick sort //
    template<typename T>
    void quick(T a[], n_t n)
    {
        if (n < 2)
            return;

        const T& pivot(a[0]);
        T* const split(std::partition(a + 1, a + n, [&pivot](const T& v){ return v <= pivot; }));
        std::iter_swap(a, split - 1);
        quick(a, std::distance(a, split - 1));
        quick(split, std::distance(split, a + n));
    }
    template<typename T, n_t n>
    void quick(T(&a)[n]){ quick(a, n); }


    // Radix sort //
    // Direct unsigned integers
    template<n_t n_bits = bitSize_v<unsigned>>
    void radix(unsigned a[], n_t n)
    {
        for (index_t i{}; i < n_bits; ++i)
            std::stable_partition(a, a + n, [i](unsigned v) { return !(v & 1 << i); });
    }
    template<n_t n_bits = bitSize_v<unsigned>, n_t n>
    void radix(unsigned(&a)[n]) { radix<n_bits>(a, n); }

    // Direct signed integers
    template<n_t n_bits = bitSize_v<signed>>
    void radix(signed a[], n_t n)
    {
        radix<n_bits - 1>((unsigned*) a, n);
        std::stable_partition(a, a + n, [](int v) { return v & 1 << n_bits; });
    }
    template<n_t n_bits = bitSize_v<signed>, n_t n>
    void radix(signed(&a)[n]){ radix<n_bits>(a, n); }

    // Indirect unsigned integers
    template<n_t n_bits = bitSize_v<unsigned>>
    void radix(index_t k[], unsigned v[], n_t n)
    {
        for (index_t i{}; i < n_bits; ++i)
            std::stable_partition(k, k + n, [v, i](index_t k) { return !(v[k] & 1 << i); });
    }
    template<n_t n_bits = bitSize_v<unsigned>, n_t n>
    void radix(index_t(&k)[n], unsigned(&v)[n]){ radix<n_bits>(k, v, n); }

    // Indirect signed integers
    template<n_t n_bits = bitSize_v<signed>>
    void radix(index_t k[], signed v[], n_t n)
    {
        radix<n_bits - 1>((unsigned*)v, n);
        std::stable_partition(k, k + n, [v](index_t k) { return v[k] & 1 << n_bits; });
    }
    template<n_t n_bits = bitSize_v<signed>, n_t n>
    void radix(index_t(&k)[n], signed(&v)[n]){ radix<n_bits>(k, v, n); }


	// Counting sort //
	template<n_t upperBound = 16>
	void counting(index_t a[], n_t n)
	{
		const n_t n_counts = upperBound;

		n_t counts[n_counts];
        for (index_t i{}; i < n; ++i)
			++counts[a[i]];
		
		auto it_a(std::begin(a));
        for (index_t i{}; i < n_counts; ++i)
			it_a = std::fill_n(it_a, counts[i], i);
	}
	template<n_t counts = 10, n_t n>
	void counting(index_t(&a)[n]){ counting(a, n); }
}
