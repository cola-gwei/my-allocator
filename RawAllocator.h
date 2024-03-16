#pragma once

#include "Allocator.h"

namespace allocator{

class RawAllocator : public Allocator{
public:
    RawAllocator() {
    }

    virtual void* allocate(const size_t cap, const size_t alignment) override;

    virtual void Free(void* ptr) override;

    virtual void init() override;

    virtual ~RawAllocator(){

    }
};

} // namespace allocator
