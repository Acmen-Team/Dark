#pragma once

#include <vector>
#include "../CoreTypes.h"

template<class T>
class TArray
{
public:
	inline T operator[](const uint32 i) const { return arr[i]; }

	int32	Num()const { return arr.size(); }
	bool	Contains(const T& key) { return arr; }
	void	Add(const T& key) { arr.push_back(key); }
	int32	Remove(const T& key) { return 0; }
	
	void	Empty() { arr.clear(); }
	bool	IsEmpty() { arr.empty(); }

private:
	std::vector<T> arr;
};