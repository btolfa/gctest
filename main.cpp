#include <functional>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

#include "gc.h"

template <int Size> void Do(std::function<void*(size_t)> allocator, int index)
{
    void* data = allocator(Size);
    if (data == nullptr)
    {
        std::stringstream stream;
        stream << "Can't alloc for " << index << " iteration";
        std::string message = stream.str();
        throw std::runtime_error(message);
    }
}

template <size_t Size> void Do(std::function<void*(size_t)> allocator, int index, std::vector<size_t> &storage)
{
    void* data = allocator(Size);
    if (data == nullptr)
    {
        std::stringstream stream;
        stream << "Can't alloc for " << index << " iteration";
        std::string message = stream.str();
        throw std::runtime_error(message);
    }
    storage.push_back(reinterpret_cast<size_t>(data));
}

template <int Size, size_t Count> void test(std::function<void*(size_t)> allocator)
{
    for (int iteration = 0; iteration < Count; ++iteration)
    {
        Do<Size>(allocator, iteration);
    }
    GC_gcollect();
    void* data = allocator(Size);
    if (data == nullptr)
    {
        throw std::runtime_error("Can't alloc after garbage collection");
    }
}

template <size_t Size, size_t Count> void testWithSavingAddresses(std::function<void*(size_t)> allocator)
{
    std::vector<size_t> storage;
    for (int iteration = 0; iteration < Count; ++iteration)
    {
        Do<Size>(allocator, iteration, storage);
    }
    GC_gcollect();
    void* data = allocator(Size);
    if (data == nullptr)
    {
        throw std::runtime_error("Can't alloc after garbage collection");
    }
}

int main()
{
    // example 1 : works in C++, works in C#
    //test<1000000, 40000>(GC_malloc);
    //test<1000000, 40000>(GC_malloc_ignore_off_page);
    // example 2 : works in C++, works in C#
    //test<1000000, 40000>(GC_malloc_atomic);
    // example 3 : failed at approx 150 iteration in C++, works in C#
    //test<10000000, 4000>(GC_malloc);
    // example 4 : failed at approx 350 iteration in C++, works in C#
    //test<10000000, 4000>(GC_malloc_atomic);
    // example 5 : failed at approx 17 iteration in C++, works in C#
    test<100000000, 40000>(GC_malloc_ignore_off_page);
    // example 6 : failed at approx 17 iteration in C++, works in C#
    //test<100000000, 400>(GC_malloc_atomic_ignore_off_page);
    // example 7 : failed at approx 1600 iteration in C++
    //testWithSavingAddresses<1000000, 40000>(GC_malloc);
    // example 8 : failed at approx 1600 iteration in C++
    //testWithSavingAddresses<1000000, 40000>(GC_malloc_atomic);
    return 0;
}