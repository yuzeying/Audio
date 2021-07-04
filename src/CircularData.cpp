//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "CircularData.h"

CircularData::CircularData()
	: front(CIRCULAR_DATA_SIZE),
	back(CIRCULAR_DATA_SIZE),
	empty(true),
	full(false)
{
	// initialized data
	for (int i = 0; i < CIRCULAR_DATA_SIZE; i++)
	{
		this->data[i] = 0;
	}
}

bool CircularData::PushBack(int val)
{
	this->mtx.lock();

	bool status = true;

	// Do we have space to add?
	if (this->front != this->back || this->empty)
	{
		// add data
		this->data[ this->back.getIndex() ] = val;
		this->back++;

		this->empty = false;

		// check if this add made it full
		if (this->front == this->back)
		{
			this->full = true;
		}
	}
	else
	{
		status = false;
	}

	this->mtx.unlock();

	return status;
}

bool CircularData::PopFront(int &val)
{
	this->mtx.lock();
	
	bool status = true;

	// Is there data to process?
	if (this->front != this->back || this->full)
	{
		// Grab one
		val = this->data[ this->front.getIndex() ] ;
		this->front++;

		this->full = false;

		// check if this Pop made it Empty
		if (this->front == this->back)
		{
			this->empty = true;
		}
	}
	else
	{
		status = false;
	}

	this->mtx.unlock();

	return status;
}

// End of File 
