#include "FreeListAllocator.h"
#include "Allocator.h"
#include "Utils.h"
#include <cassert>
#include <cstddef>
#include <limits>
#include <stdlib.h>


#if _DEBUG
#include <iostream>
#endif

namespace allocator {

FreeListAllocator::FreeListAllocator(const size_t cap, const Policy policy)
    : policy(policy) {
  Allocator::cap = cap;
}

void FreeListAllocator::init() {
  if (base != nullptr) {
    free(base);
    base = nullptr;
  }

  base = malloc(cap);
  reset();
}

FreeListAllocator::~FreeListAllocator() {
  free(base);
  base = nullptr;
}

void *FreeListAllocator::allocate(const size_t size, const size_t alignment) {
  const size_t headerSize = sizeof(FreeListAllocator::AllocationHeader);
  const size_t freeHeaderSize = sizeof(FreeListAllocator::FreeHeader);

  size_t padding;
  iterator node;
  find(size, alignment, padding, node);
  assert(node != freeList.end() && "Not enough memory");
  auto requiredSpace = padding + size;             // allocator header + size
  auto dataAddress = (size_t)node->base + padding; // data start position

  auto rest = node->blockSize - requiredSpace;

  if (rest > 0) {
    FreeHeader newNode{(void *)(dataAddress + size), rest};
    freeList.insert(node, newNode); // ok, insert will keep iterator
  }
  freeList.erase(node); // after call, iterator invaild

  auto headerAddress = dataAddress - headerSize;
  auto headerPtr = (AllocationHeader*) headerAddress;
  headerPtr->blockSize = requiredSpace;
  headerPtr->padding = padding;
  used += requiredSpace;
  peak = peak > used ? peak : used;
  return (void *)dataAddress;
}

void FreeListAllocator::Free(void *ptr) {
  const size_t currentAddress = (size_t)ptr;
  const size_t headerAddress =
      currentAddress - sizeof(AllocationHeader);
  const AllocationHeader *allocationHeader =
      (AllocationHeader *)headerAddress;
  const auto thisBase = (size_t)ptr - allocationHeader->padding;
  FreeHeader newNode{(void*)base, allocationHeader->blockSize};

  auto it = freeList.begin();
  iterator prevIt;
  
  while (it != freeList.end()) {
    if ((size_t)thisBase < (size_t)it->base) {
      prevIt = freeList.insert(it, newNode);
      break;
    }
    ++it;
  }

  used -= newNode.blockSize;
  if(it == freeList.end()){
    freeList.insert(it, newNode);
  }else{
    coaleScence(prevIt, it);
  }
}

void FreeListAllocator::coaleScence(iterator prevIt, iterator thisIt) {
    assert(thisIt != freeList.end() && "last node should call this function");
    if((size_t)prevIt->base + prevIt->blockSize == (size_t) thisIt->base){
        prevIt->blockSize += thisIt->blockSize;
        freeList.erase(thisIt);
    }
}

void FreeListAllocator::find(const size_t size, const size_t alignment,
                             size_t &padding, iterator &foundNode) {
    if(policy==FIRST){
        findFirst(size, alignment, padding, foundNode);
    }else{
        findBest(size, alignment, padding, foundNode);
    }
}

void FreeListAllocator::findBest(const size_t size, const size_t alignment,
                                 size_t &padding, iterator &foundNode) {
  auto it = freeList.begin();
  foundNode = freeList.end();
  auto rest = std::numeric_limits<size_t>::max();
  while (it != freeList.end()) {
    padding = Utils::computePaddingWithHeader((size_t)it->base, alignment,
                                              sizeof(AllocationHeader));
    auto requiredSpace = size + padding;
    if (it->blockSize >= requiredSpace) {
        auto thisRest = it->blockSize - requiredSpace;
        if(thisRest < rest){
            foundNode = it;
        }
    }
    ++it;
  }
}

void FreeListAllocator::findFirst(const size_t size, const size_t alignment,
                                  size_t &padding, iterator &foundNode) {
  auto it = freeList.begin();
  foundNode = freeList.end();
  while (it != freeList.end()) {
    padding = Utils::computePaddingWithHeader((size_t)it->base, alignment,
                                              sizeof(AllocationHeader));
    auto requiredSpace = size + padding;
    if (it->blockSize >= requiredSpace) {
        foundNode = it;
        break;
    }
    ++it;
  }
}

void FreeListAllocator::reset() {
  used = 0;
  peak = 0;
  FreeHeader firstNode{(void *)base, cap};
  freeList.push_back(firstNode);
}

} // namespace allocator