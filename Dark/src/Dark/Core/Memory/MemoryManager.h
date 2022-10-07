/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-9-5 23:17
Description:Memory Manager
**************************************************/
#pragma once
#include "Dark/Core/Memory/MemoryAllocator/LinearAllocator.h"

// http://allenchou.net/memory-management-series/
// http://allenchou.net/2013/05/memory-management-part-1-of-3-the-allocator/
// https://www.gamedev.net/forums/topic/706394-game-engine-architecture-questions/
// https://www.gamedeveloper.com/programming/writing-a-game-engine-from-scratch---part-2-memory
// Github: https://github.com/Vieper1/MemoryManager

namespace Dark {

  class MemoryManager
  {
  public:
    MemoryManager();
    ~MemoryManager();

    void* AllocateMemory(size_t size);

    void* GetStart() const { return m_LinearAllocator->GetStart(); }
    size_t GetSize() const { return m_LinearAllocator->GetSize(); }
    size_t GetUsedMemory() const { return m_LinearAllocator->GetUsedMemory(); }
    size_t GetNumAllocations() const { return m_LinearAllocator->GetNumAllocations(); }

  private:
    LinearAllocator* m_LinearAllocator;
  };

  extern MemoryManager s_MemoryManager;

} // namespace Dark