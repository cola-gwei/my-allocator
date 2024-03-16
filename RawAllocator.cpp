#include "RawAllocator.h"
#include <stdlib.h>
#include <iostream>

namespace allocator{

   void* RawAllocator::allocate(const size_t cap, const size_t alignment) {
        return malloc(cap);
    }

    void RawAllocator::Free(void* ptr){
        free(ptr);
    }

    void RawAllocator::init(){

    }
}