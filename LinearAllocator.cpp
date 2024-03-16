#include "LinearAllocator.h"
#include "Utils.h"
#include <stdlib.h>
#ifdef _DEBUG
#include<iostream>
#endif

namespace allocator{

    void LinearAllocator::init(){
        if(base!=nullptr){
            free(base);
        }

        base = malloc(cap);
        offset = 0;
    }

    LinearAllocator::~LinearAllocator(){
        free(base);
        base = nullptr;
    }

    void* LinearAllocator::allocate(const size_t size, const size_t alignment){
        size_t padding = 0;
        size_t paddedAddress = 0;
        const size_t currentAddress = (size_t) base + offset;

        if (alignment != 0 && currentAddress % alignment != 0){
            padding = Utils::computePadding(currentAddress, alignment);
        }

        if (offset + padding + size > cap){
            return nullptr;
        }

        offset += padding;
        const size_t nextAddress = currentAddress + padding;
        offset += size;

        #ifdef _DEBUG
        std::cout << "A" << "\t@C " << (void*) currentAddress << "\t@R " << (void*) nextAddress << "\tO " << m_offset << "\tP " << padding << std::endl;
        #endif

        used = offset;
        peak = peak > used? peak : used;

        return (void*)nextAddress;
    }

    void LinearAllocator::Free(void* ptr){
        assert(false && "Use Reset() method");
    }
    
    void LinearAllocator::reset(){
        offset = 0;
        used = 0;
        peak = 0;
    }
}