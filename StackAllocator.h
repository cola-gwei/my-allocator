#pragma once

#include "Allocator.h"

namespace allocator{

class StackAllocator : public Allocator{
    struct Header{
        size_t padding;
    };
public:
    StackAllocator(const size_t cap_){
        Allocator::cap = cap_;
    }

    virtual ~StackAllocator();

    virtual void* allocate(const size_t size, const size_t alignment) override;

    virtual void Free(void* ptr) override;

    virtual void init() override;

    void reset();

private:
    void* base = nullptr;
    size_t offset;
};


}