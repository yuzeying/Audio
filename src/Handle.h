//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef HANDLE_H
#define HANDLE_H

//---------------------------------------------------------------
// Macros... to keep it clean
// --------------------------------------------------------------
#define HANDLE_VARABLE			Handle handle;

#define HANDLE_CHECK_BEGIN		Handle::Status status = Handle::ValidateHandle(handle);\
								if (status == Handle::Status::SUCCESS) { 

#define HANDLE_CHECK_END		}return status;

#define HANDLE_INVALIDATE       Handle::InvalidateHandle(handle);

//---------------------------------------------------------------
// Handle class
// --------------------------------------------------------------

class Handle
{
public:

	// --------------------------------------------------
	// Status of handles
	// --------------------------------------------------
	enum class Status
	{
		SUCCESS = 0x7EEE0000,

		INSUFFIENT_SPACE,
		INVALID_HANDLE,

		HANDLE_ERROR = 0x7EEEFFFF
	};

	// --------------------------------------------------
	// Specialized types
	// --------------------------------------------------
	typedef unsigned int  ID;
	typedef unsigned int  Index;

public:
	Handle();
	~Handle();

	//-----------------------------------------------------
	// You cannot copy handles
	//-----------------------------------------------------
	Handle(const Handle &) = delete;
	const Handle & operator = (const Handle &) = delete;

	//-----------------------------------------------------
	// You can peek at it, but you can't set it 
	//-----------------------------------------------------
	ID GetID() const;
	Index GetIndex() const;

	//-----------------------------------------------------
	// Tunnel method to prevent dependency on HandleMan
	//-----------------------------------------------------
	static Status ValidateHandle(Handle &handle);
	static Status InvalidateHandle(Handle &handle);


private:
	//-----------------------------------------------------
	// Data must remain private
	//-----------------------------------------------------
	ID		id;
	Index	index;
};


#endif
 
// ---  End of File ---------------
