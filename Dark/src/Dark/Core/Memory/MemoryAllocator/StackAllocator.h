/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-9-22 21:32
Description:Stack Memory Allocator
**************************************************/
#pragma once
#include "Allocator.h"

namespace Dark {

  class StackAllocator : public Allocator
  {
  public:
    StackAllocator(void* start, size_t size);
    ~StackAllocator();

    virtual void* Allocate(size_t size, Alignment alignment /* = Alignment::e4BytesAlignment */) override;
    virtual void Deallocate(void* p) override;

  private:
    struct AllocationHeader
    {
#if DK_DEBUG
      void* prevAddress;
#endif

      u8 adjustment;
    };

#if DK_DEBUG
    void* m_PrevPosition;
#endif

    void* m_CurrentPos;
  };

} // namespace Dark