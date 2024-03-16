#pragma once
#include<cstddef>
#include <cassert>

namespace Utils{
size_t computePadding(size_t currentAddress, size_t alignment);
size_t computePaddingWithHeader(const size_t baseAddress, const size_t alignment, const size_t headerSize);
} // namespace Utils