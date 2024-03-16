#include "PoolAllocator.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef _DEBUG
#include <iostream>
#endif

namespace allocator{

PoolAllocator::PoolAllocator(const size_t cap_, const size_t chunkSize_){
    assert(chunkSize_ >= 8 && "Chunk size must be greater or equal to 8");
    assert(cap_ % chunkSize_ == 0 && "Total size must be a multiple of chunk size");
    cap = cap_;
    chunkSize = chunkSize_;
}

void PoolAllocator::init(){
    base = malloc(cap);
    reset();
}

PoolAllocator::~PoolAllocator(){
    free(base);
}

void* PoolAllocator::allocate(const size_t size, const size_t alignment){
    assert(size == chunkSize && "Allocation size must be equal to chunk size");

    void* freePosition = freeList.back();
    freeList.pop_back();
    
    assert(freePosition != nullptr && "The pool allocator is full");

    used += chunkSize;
    peak = peak > used? peak : used;

    return (void*) freePosition;
}

void PoolAllocator::Free(void *ptr){
    used -= chunkSize;
    freeList.push_back(ptr);
}

void PoolAllocator::reset(){
    used = 0;
    peak = 0;
    const size_t numChunks = cap / chunkSize;
    for(size_t i = 0; i < numChunks; ++i){
        size_t address = (size_t) base + i * chunkSize;
        freeList.push_back((void*)address);
    }
}

};