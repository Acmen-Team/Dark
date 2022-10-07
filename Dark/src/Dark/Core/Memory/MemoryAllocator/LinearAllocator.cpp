#include "dkpch.h"

#include "LinearAllocator.h"

namespace Dark {

  LinearAllocator::LinearAllocator(void* start, size_t size)
      : Allocator(start, size), m_CurrentPos(start)
  {
    DK_CORE_ASSERT(start != nullptr && size > 0, "LinearAllocator Error: start == NULL / size > 0");
  }

  //LinearAllocator::LinearAllocator(const LinearAllocator&)
  // {

  // }

  //Dark::LinearAllocator& LinearAllocator::operator=(const LinearAllocator&)
  //  {
  //  }

  LinearAllocator::~LinearAllocator()
  {
    m_CurrentPos = nullptr;
  }

  void* LinearAllocator::Allocate(size_t size, Alignment alignment /* = Alignment::e4BytesAlignment */)
  {
    DK_CORE_ASSERT(size != 0, "size != 0");

    u8 adjustment = AlignForwardAdjustment(m_CurrentPos, alignment);

    if (m_UsedMemory + adjustment + size > m_Size)
      return nullptr;

    uintptr_t alignedAddress = (uintptr_t)m_CurrentPos + adjustment;
    m_CurrentPos             = (void*)(alignedAddress + size);
    m_UsedMemory += size + adjustment;
    m_NumAllocations++;

    return (void*)alignedAddress;
  }

  void LinearAllocator::Deallocate(void* p)
  {
    DK_CORE_ASSERT(false, "Use clear() intead");
  }

  void LinearAllocator::Clear()
  {
    m_UsedMemory     = 0;
    m_NumAllocations = 0;
    m_CurrentPos     = m_Start;
  }

} // namespace Dark