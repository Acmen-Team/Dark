// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "../CoreTypes.h"
//#include "Templates/IsPointer.h"
//#include "HAL/PlatformMemory.h"
//#include "HAL/MemoryBase.h"
//#include <wchar.h>
#include<memory>

enum
{
	// Default allocator alignment. If the default is specified, the allocator applies to engine rules.
	// Blocks >= 16 bytes will be 16-byte-aligned, Blocks < 16 will be 8-byte aligned. If the allocator does
	// not support allocation alignment, the alignment will be ignored.
	DEFAULT_ALIGNMENT = 0,

	// Minimum allocator alignment
	MIN_ALIGNMENT = 8,
};

/*-----------------------------------------------------------------------------
	FMemory.
-----------------------------------------------------------------------------*/

struct Memory
{
	/** Some allocators can be given hints to treat allocations differently depending on how the memory is used, it's lifetime etc. */
	enum AllocationHints
	{
		None = -1,
		Default,
		Temporary,
		SmallPool,

		Max
	};


	/** @name Memory functions (wrapper for FPlatformMemory) */

	static FORCEINLINE void* Memmove(void* Dest, const void* Src, SIZE_T Count)
	{
		return memmove(Dest, Src, Count);
	}

	static FORCEINLINE int32 Memcmp(const void* Buf1, const void* Buf2, SIZE_T Count)
	{
		return memcmp(Buf1, Buf2, Count); //return FPlatformMemory::Memcmp(Buf1, Buf2, Count);
	}

	static FORCEINLINE void* Memset(void* Dest, uint8 Char, SIZE_T Count)
	{
		return memset(Dest, 0, Count);//return FPlatformMemory::Memset(Dest, Char, Count);
	}

	template< class T >
	static FORCEINLINE void Memset(T& Src, uint8 ValueToSet)
	{
		//static_assert(!TIsPointer<T>::Value, "For pointers use the three parameters function");
		Memset(&Src, ValueToSet, sizeof(T));
	}

	static FORCEINLINE void* Memzero(void* Dest, SIZE_T Count)
	{
		return memset(Dest, 0, Count); //return FPlatformMemory::Memzero(Dest, Count);
	}

	template< class T >
	static FORCEINLINE void Memzero(T& Src)
	{
		//static_assert(!TIsPointer<T>::Value, "For pointers use the two parameters function");
		Memzero(&Src, sizeof(T));
	}

	static FORCEINLINE void* Memcpy(void* Dest, const void* Src, SIZE_T Count)
	{
		return memcpy(Dest, Src, Count); //return FPlatformMemory::Memcpy(Dest, Src, Count);
	}

	template< class T >
	static FORCEINLINE void Memcpy(T& Dest, const T& Src)
	{
		//static_assert(!TIsPointer<T>::Value, "For pointers use the three parameters function");
		Memcpy(&Dest, &Src, sizeof(T));
	}

	static FORCEINLINE void* BigBlockMemcpy(void* Dest, const void* Src, SIZE_T Count)
	{
		return memcpy(Dest, Src, Count); //return FPlatformMemory::BigBlockMemcpy(Dest, Src, Count);
	}

	static FORCEINLINE void* StreamingMemcpy(void* Dest, const void* Src, SIZE_T Count)
	{
		return memcpy(Dest, Src, Count); //return FPlatformMemory::StreamingMemcpy(Dest, Src, Count);
	}

	template <typename T>
	static FORCEINLINE void Valswap(T& A, T& B)
	{
		// Usually such an implementation would use move semantics, but
		// we're only ever going to call it on fundamental types and MoveTemp
		// is not necessarily in scope here anyway, so we don't want to
		// #include it if we don't need to.
		T Tmp = A;
		A = B;
		B = Tmp;
	}

	static FORCEINLINE void Memswap(void* Ptr1, void* Ptr2, SIZE_T Size)
	{
		//FPlatformMemory::Memswap(Ptr1, Ptr2, Size);
		switch (Size)
		{
		case 0:
			break;

		case 1:
			Valswap(*(uint8*)Ptr1, *(uint8*)Ptr2);
			break;

		case 2:
			Valswap(*(uint16*)Ptr1, *(uint16*)Ptr2);
			break;

		case 3:
			Valswap(*((uint16*&)Ptr1)++, *((uint16*&)Ptr2)++);
			Valswap(*(uint8*)Ptr1, *(uint8*)Ptr2);
			break;

		case 4:
			Valswap(*(uint32*)Ptr1, *(uint32*)Ptr2);
			break;

		case 5:
			Valswap(*((uint32*&)Ptr1)++, *((uint32*&)Ptr2)++);
			Valswap(*(uint8*)Ptr1, *(uint8*)Ptr2);
			break;

		case 6:
			Valswap(*((uint32*&)Ptr1)++, *((uint32*&)Ptr2)++);
			Valswap(*(uint16*)Ptr1, *(uint16*)Ptr2);
			break;

		case 7:
			Valswap(*((uint32*&)Ptr1)++, *((uint32*&)Ptr2)++);
			Valswap(*((uint16*&)Ptr1)++, *((uint16*&)Ptr2)++);
			Valswap(*(uint8*)Ptr1, *(uint8*)Ptr2);
			break;

		case 8:
			Valswap(*(uint64*)Ptr1, *(uint64*)Ptr2);
			break;

		case 16:
			Valswap(((uint64*)Ptr1)[0], ((uint64*)Ptr2)[0]);
			Valswap(((uint64*)Ptr1)[1], ((uint64*)Ptr2)[1]);
			break;

		default:
			//MemswapGreaterThan8(Ptr1, Ptr2, Size);
			break;
		}
	}

	//
	// C style memory allocation stubs that fall back to C runtime
	//
	static FORCEINLINE void* SystemMalloc(SIZE_T Size)
	{
		return ::malloc(Size);
	}

	static FORCEINLINE void SystemFree(void* Ptr)
	{
		::free(Ptr);
	}

	//
	// C style memory allocation stubs.
	//

	//static void* Malloc(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
	//static void* Realloc(void* Original, SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
	//static void Free(void* Original);
	//static SIZE_T GetAllocSize(void* Original);

	/*static FORCEINLINE void* MallocZeroed(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT)
	{
		void* Memory = Malloc(Count, Alignment);
		Memzero(Memory, Count);
		return Memory;
	}*/

	/**
	* For some allocators this will return the actual size that should be requested to eliminate
	* internal fragmentation. The return value will always be >= Count. This can be used to grow
	* and shrink containers to optimal sizes.
	* This call is always fast and threadsafe with no locking.
	*/
	//static SIZE_T QuantizeSize(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);

	/**
	* Releases as much memory as possible. Must be called from the main thread.
	*/
	//static void Trim(bool bTrimThreadCaches = true);

	/**
	* Set up TLS caches on the current thread. These are the threads that we can trim.
	*/
	//static void SetupTLSCachesOnCurrentThread();

	/**
	* Clears the TLS caches on the current thread and disables any future caching.
	*/
	//static void ClearAndDisableTLSCachesOnCurrentThread();

	/**
	 * A helper function that will perform a series of random heap allocations to test
	 * the internal validity of the heap. Note, this function will "leak" memory, but another call
	 * will clean up previously allocated blocks before returning. This will help to A/B testing
	 * where you call it in a good state, do something to corrupt memory, then call this again
	 * and hopefully freeing some pointers will trigger a crash.
	 */
	//static void TestMemory();
	/**
	* Called once main is started and we have -purgatorymallocproxy.
	* This uses the purgatory malloc proxy to check if things are writing to stale pointers.
	*/
	//static void EnablePurgatoryTests();
	/**
	* Called once main is started and we have -purgatorymallocproxy.
	* This uses the purgatory malloc proxy to check if things are writing to stale pointers.
	*/
	//static void EnablePoisonTests();
	/**
	* Set global allocator instead of creating it lazily on first allocation.
	* Must only be called once and only if lazy init is disabled via a macro.
	*/
	//static void ExplicitInit(FMalloc& Allocator);

	/**
	* Functions to handle special memory given to the title from the platform
	* This memory is allocated like a stack, it's never really freed
	*/
	/*static void RegisterPersistentAuxiliary(void* InMemory, SIZE_T InSize);
	static void* MallocPersistentAuxiliary(SIZE_T InSize, uint32 InAlignment = 0);
	static void FreePersistentAuxiliary(void* InPtr);
	static bool IsPersistentAuxiliaryActive();
	static void DisablePersistentAuxiliary();
	static void EnablePersistentAuxiliary();
	static SIZE_T GetUsedPersistentAuxiliary();*/
private:
	//static void GCreateMalloc();
	//// These versions are called either at startup or in the event of a crash
	//static void* MallocExternal(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
	//static void* ReallocExternal(void* Original, SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
	//static void FreeExternal(void* Original);
	//static SIZE_T GetAllocSizeExternal(void* Original);
	//static SIZE_T QuantizeSizeExternal(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
};

//#if !defined(FMEMORY_INLINE_GMalloc)
//#define FMEMORY_INLINE_GMalloc GMalloc
//#endif
//
//void* Memory::Malloc(SIZE_T Count, uint32 Alignment)
//{
//	void* Ptr;
//	if (!FMEMORY_INLINE_GMalloc)
//	{
//		Ptr = MallocExternal(Count, Alignment);
//	}
//	else
//	{
//		DoGamethreadHook(0);
//		FScopedMallocTimer Timer(0);
//		Ptr = FMEMORY_INLINE_GMalloc->Malloc(Count, Alignment);
//	}
//	// optional tracking of every allocation
//	LLM_IF_ENABLED(FLowLevelMemTracker::Get().OnLowLevelAlloc(ELLMTracker::Default, Ptr, Count, ELLMTag::Untagged, ELLMAllocType::FMalloc));
//	return Ptr;
//}
//
//FMEMORY_INLINE_FUNCTION_DECORATOR void* FMemory::Realloc(void* Original, SIZE_T Count, uint32 Alignment)
//{
//	// optional tracking -- a realloc with an Original pointer of null is equivalent
//	// to malloc() so there's nothing to free
//	LLM_REALLOC_SCOPE(Original);
//	LLM_IF_ENABLED(if (Original != nullptr) FLowLevelMemTracker::Get().OnLowLevelFree(ELLMTracker::Default, Original, ELLMAllocType::FMalloc));
//
//	void* Ptr;
//	if (!FMEMORY_INLINE_GMalloc)
//	{
//		Ptr = ReallocExternal(Original, Count, Alignment);
//	}
//	else
//	{
//		DoGamethreadHook(1);
//		FScopedMallocTimer Timer(1);
//		Ptr = FMEMORY_INLINE_GMalloc->Realloc(Original, Count, Alignment);
//	}
//
//	// optional tracking of every allocation - a realloc with a Count of zero is equivalent to a call 
//	// to free() and will return a null pointer which does not require tracking. If realloc returns null
//	// for some other reason (like failure to allocate) there's also no reason to track it
//	LLM_IF_ENABLED(if (Ptr != nullptr) FLowLevelMemTracker::Get().OnLowLevelAlloc(ELLMTracker::Default, Ptr, Count, ELLMTag::Untagged, ELLMAllocType::FMalloc));
//
//	return Ptr;
//}
//
//FMEMORY_INLINE_FUNCTION_DECORATOR void FMemory::Free(void* Original)
//{
//	if (!Original)
//	{
//		FScopedMallocTimer Timer(3);
//		return;
//	}
//
//	// optional tracking of every allocation
//	LLM_IF_ENABLED(FLowLevelMemTracker::Get().OnLowLevelFree(ELLMTracker::Default, Original, ELLMAllocType::FMalloc));
//
//	if (!FMEMORY_INLINE_GMalloc)
//	{
//		FreeExternal(Original);
//		return;
//	}
//	DoGamethreadHook(2);
//	FScopedMallocTimer Timer(2);
//	FMEMORY_INLINE_GMalloc->Free(Original);
//}
//
//FMEMORY_INLINE_FUNCTION_DECORATOR SIZE_T FMemory::GetAllocSize(void* Original)
//{
//	if (!FMEMORY_INLINE_GMalloc)
//	{
//		return GetAllocSizeExternal(Original);
//	}
//
//	SIZE_T Size = 0;
//	return FMEMORY_INLINE_GMalloc->GetAllocationSize(Original, Size) ? Size : 0;
//}