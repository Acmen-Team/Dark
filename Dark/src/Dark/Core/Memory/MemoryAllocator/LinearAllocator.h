/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-9-22 21:05
Description:Linear Memory Allocator
**************************************************/
#pragma once
#include "Allocator.h"

namespace Dark {

  class LinearAllocator : public Allocator
  {
  public:
    LinearAllocator(void* start, size_t size);
    ~LinearAllocator();

    virtual void* Allocate(size_t size, Alignment alignment = Alignment::e4BytesAlignment) override;
    void Deallocate(void* p) override;
    void Clear();

  private:
    LinearAllocator(const LinearAllocator&);

    //Prevent copies because it might cause errors
    LinearAllocator& operator=(const LinearAllocator&);
    void* m_CurrentPos;
  };

} // namespace Dark