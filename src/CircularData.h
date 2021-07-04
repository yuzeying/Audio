//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CIRCULAR_DATA_H
#define CIRCULAR_DATA_H

#include "CircularIndex.h"
#include <mutex>

class CircularData
{
public:
	// needs to be a power of 2
	static const int CIRCULAR_DATA_SIZE = 32;

public:
	CircularData();
	
	CircularData(const CircularData &) = delete;
	const CircularData &operator = (const CircularData &) = delete;
	~CircularData() = default;

	bool PushBack(int val);
	bool PopFront(int &val);

private:
	int data[CIRCULAR_DATA_SIZE];

	CircularIndex front;
	CircularIndex back;
	bool empty;
	bool full;

	std::mutex mtx;
};

#endif

// End of File 
