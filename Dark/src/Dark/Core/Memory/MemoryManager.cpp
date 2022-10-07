/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-9-17 12:09
Description:Memory Manager
**************************************************/
#include "dkpch.h"

#include "MemoryManager.h"

namespace Dark {

  MemoryManager s_MemoryManager;

  MemoryManager::MemoryManager()
  {
    m_LinearAllocator = new LinearAllocator(malloc(100 * 1024 * 1024), 100 * 1024 * 1024);
  }

  MemoryManager::~MemoryManager()
  {
    m_LinearAllocator->Clear();
    delete m_LinearAllocator;
  }

  void* MemoryManager::AllocateMemory(size_t size)
  {
    return m_LinearAllocator->Allocate(size);
  }

} // namespace Dark