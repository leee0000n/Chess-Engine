#pragma once

#include "tree.h"

class optimized_map
{
public:
	optimized_map();

	/// <summary>
	/// Return value at specific key or edit value at key
	/// </summary>
	/// <param name="key"> The key used to search hash array</param>
	/// <returns> reference to value beign stored</returns>
	int& operator[] (int key);

	unsigned int hash(int key);

	int resize(int size);


private:
	Tree** m_hashArray;
	int m_hashArraySize;

};

