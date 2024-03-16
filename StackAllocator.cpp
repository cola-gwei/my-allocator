#include  "StackAllocator.h"
#include "Utils.h"
#include <stdlib.h>
#include <memory.h>

#ifdef _DEBUG
#include <iostream>
#endif

namespace allocator{

void StackAllocator::init(){
    if(base != nullptr){
        free(base);
    }

    base = malloc(cap);
    offset = 0;
}

StackAllocator::~StackAllocator(){
    free(base);
    base = nullptr;
}

void* StackAllocator::allocate(const size_t size, const size_t alignment){
    const size_t currentAddress = (size_t) base + offset;

    auto padding = Utils::computePaddingWithHeader(currentAddress, alignment, sizeof(Header));

    if(offset + padding + size > cap){
        return nullptr;
    }

    offset += padding;
    
    const size_t nextAddress = currentAddress + padding;
    const size_t headerAddress = nextAddress - sizeof(Header); 
    Header header{padding};
    memcpy((void*)headerAddress, &header, sizeof(header));

    offset += size;

    #ifdef _DEBUG
    std::cout << "A" << "\t@C " << (void*) currentAddress << "\t@R " << (void*) nextAddress << "\t0 " << offset << "\tP " << padding << std::endl;
    #endif

    used = offset;

    peak = peak > used? peak : used;

    return (void*) nextAddress;
}
 
void StackAllocator::Free(void* ptr){
    const size_t currentAddress = (size_t) ptr;
    const size_t headerAddress = currentAddress - sizeof(Header);
    const Header* headerPtr = (Header*) headerAddress;
    const size_t padding = headerPtr->padding;
    offset = currentAddress - padding - (size_t) base;
    used = offset;

    #ifdef _DEBUG
    std::cout << "F" << "\t@C " << (void*) currentAddress << "\t@F " << (void*) ((char*) m_start_ptr + m_offset) << "\tO " << m_offset << std::endl;
    #endif
}

void StackAllocator::reset(){
    used = 0;
    offset = 0;
    peak = 0;
}

} // namespace allocator