#pragma once
#include "Allocator.h"

namespace allocator{

class LinearAllocator : public Allocator{
public:
    LinearAllocator(const size_t cap_){
        Allocator::cap = cap_;
    }

    virtual ~LinearAllocator();

    virtual void* allocate(const size_t size, const size_t alignment) override;

    virtual void Free(void* ptr) override;

    virtual void init() override;

    virtual void reset();

private:
    void* base = nullptr;
    size_t offset;
};

} // namespace allocator