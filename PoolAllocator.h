#pragma once

#include "Allocator.h"
#include <list>

namespace allocator{

class PoolAllocator: public Allocator{
private:
    std::list<void*> freeList; 

    void *base = nullptr;
    size_t chunkSize;

public:
    PoolAllocator(const size_t cap, const size_t chunk);    

    virtual ~PoolAllocator();

    virtual void* allocate(const size_t size, const size_t alignment) override;

    virtual void Free(void* ptr) override;

    virtual void init() override;

    virtual void reset();

private: 
    PoolAllocator(PoolAllocator &other) = delete;
};

}