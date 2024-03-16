#pragma once

#include "Allocator.h"
#include "SinglyLinkedList.h"
#include <list>

namespace allocator{

class FreeListAllocator : public Allocator{
public:
    enum Policy{
        FIRST,
        BEST
    };

private:

    struct FreeHeader{
        void* base;
        size_t blockSize;
    };

    struct AllocationHeader{
        size_t blockSize;
        size_t padding;
    };

    std::list<FreeHeader> freeList;
    using iterator = std::list<FreeHeader>::iterator;
    void* base = nullptr;
    Policy policy;

public:
    FreeListAllocator(const size_t cap_, const Policy policy_);

    virtual ~FreeListAllocator();

    virtual void* allocate(const size_t size, const size_t alignment) override;

    virtual void Free(void* ptr) override;

    virtual void init() override;

    void reset();

private:
    FreeListAllocator(FreeListAllocator& rhs);

    void coaleScence(iterator prevIt, iterator thisIt);
    
    void find(const size_t size, const size_t alignment, size_t& padding
    , iterator&  foundNode);
    
    void findBest(const size_t size, const size_t alignment, size_t& padding
    , iterator&  foundNode);

    void findFirst(const size_t size, const size_t alignment, size_t& padding
    , iterator&  foundNode);
};

} // namespace allocator