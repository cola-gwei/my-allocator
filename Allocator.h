#pragma once

#include <cstddef>

namespace allocator{
using std::size_t;
class Allocator{
public:

    virtual void* allocate(const size_t sz, const size_t alignment)  = 0;

    virtual void Free(void* ptr)  = 0;

    virtual void init() = 0;

    virtual ~Allocator(){

    }

protected:
    std::size_t cap = 0u;
    std::size_t used = 0u;
    std::size_t peak = 0u;
    friend class Benchmark;
};

} //namespace allocator