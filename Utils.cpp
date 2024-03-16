#include "Utils.h"
#include<cstddef>
#include <cassert>
#include <iostream>

namespace Utils{

    size_t computePadding(size_t currentAddress, size_t alignment){
        assert(alignment!=0 && "alignment shouldn't be zero!");
        return currentAddress % alignment;
    }

    size_t computePaddingWithHeader(const size_t baseAddress, const size_t alignment, const size_t headerSize){
        size_t padding2 = headerSize;
        padding2 += computePadding(baseAddress + headerSize, alignment);
        return padding2;
    }
} // namespace Utils