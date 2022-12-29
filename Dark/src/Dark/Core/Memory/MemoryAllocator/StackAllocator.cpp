#include "dkpch.h"

#include "StackAllocator.h"

namespace Dark {

  StackAllocator::StackAllocator(void* start, size_t size)
      : Allocator(start, size), m_CurrentPos(start)
  {
    DK_CORE_ASSERT(size > 0, "size > 0");
#if DK_DEBUG
    m_PrevPosition = nullptr;
#endif
  }

  StackAllocator::~StackAllocator()
  {
#if DK_DEBUG
    m_PrevPosition = nullptr;
#endif

    m_CurrentPos = nullptr;
  }

  void* StackAllocator::Allocate(size_t size, Alignment alignment /* = Alignment::e4BytesAlignment */)
  {
    DK_CORE_ASSERT(size != 0, "size != 0");
    u8 adjustment = AlignForwardAdjustmentWithHeader(m_CurrentPos, alignment, sizeof(AllocationHeader));

//    if (_used_memory + adjustment + size > _size)
//      return nullptr;
//
//    void* aligned_address = pointer_math::add(_current_pos, adjustment);
//
//    //Add Allocation Header
//    AllocationHeader* header = (AllocationHeader*)(pointer_math::subtract(aligned_address, sizeof(AllocationHeader)));
//    header->adjustment       = adjustment;
//
//#if _DEBUG
//    header->prev_address = _prev_position;
//    _prev_position       = aligned_address;
//#endif
//
//    _current_pos = pointer_math::add(aligned_address, size);
//    _used_memory += size + adjustment;
//    _num_allocations++;

    return nullptr;
  }

  void StackAllocator::Deallocate(void* p)
  {
  }

} // namespace Dark