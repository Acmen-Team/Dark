/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-9-22 0:18
Description:Memory Allocator
**************************************************/
#pragma once

namespace Dark {

  typedef unsigned char u8;

  enum Alignment
  {
    eNone              = 0,
    e4BytesAlignment   = 4,
    e8BytesAlignment   = 8,
    e16BytesAlignment  = 16,
    e32BytesAlignment  = 32,
    e64BytesAlignment  = 64,
    e128BytesAlignment = 128,
  };

  class Allocator
  {
  public:
    Allocator(void* start, size_t size)
    {
      m_Start          = start;
      m_Size           = size;
      m_UsedMemory     = 0;
      m_NumAllocations = 0;
    }

    virtual ~Allocator()
    {
      DK_CORE_ASSERT((m_NumAllocations == 0 && m_UsedMemory == 0), "Has no clear memory");
      m_Size  = 0;
      m_Start = nullptr;
    }

    virtual void* Allocate(size_t size, Alignment alignment = Alignment::e4BytesAlignment) = 0;
    virtual void Deallocate(void* p)                                                       = 0;

    void* GetStart() const { return m_Start; }
    size_t GetSize() const { return m_Size; }
    size_t GetUsedMemory() const { return m_UsedMemory; }
    size_t GetNumAllocations() const { return m_NumAllocations; }

  protected:
    void* m_Start;
    size_t m_Size;
    size_t m_UsedMemory;
    size_t m_NumAllocations;
  };

  template <typename T>
  T* CreateAllocate(Allocator& allocator)
  {
    return new (allocator.Allocate(sizeof(T), __alignof(T))) T;
  }

  template <typename T>
  T* CreateAllocate(Allocator& allocator, const T& t)
  {
    return new (allocator.Allocate(sizeof(T), __alignof(T))) T(t);
  }

  template <typename T>
  void DestroyAllocate(Allocator& allocator, T& object)
  {
    object.~T();
    allocator.Deallocate(&object);
  }

  inline void* AlignForward(void* address, u8 alignment)
  {
    return (void*)((u8)(reinterpret_cast<u8*>(address) + static_cast<u8>(alignment - 1)) & static_cast<u8>(~(alignment - 1)));
  }

  inline u8 AlignForwardAdjustment(void* address, u8 alignment)
  {
    u8 adjustment = alignment - (reinterpret_cast<u8>(address) & static_cast<u8>(alignment - 1));

    if (adjustment == alignment)
      return Alignment::eNone;

    //already aligned
    return adjustment;
  }

  inline u8 AlignForwardAdjustmentWithHeader(void* address, u8 alignment, u8 headerSize)
  {
    u8 adjustment  = AlignForwardAdjustment(address, alignment);
    u8 neededSpace = headerSize;

    if (adjustment < neededSpace)
    {
      neededSpace -= adjustment;

      //Increase adjustment to fit header
      adjustment += alignment * (neededSpace / alignment);

      if (neededSpace % alignment > 0)
        adjustment += alignment;
    }

    return adjustment;
  }

} // namespace Dark