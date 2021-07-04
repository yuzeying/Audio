//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef XAUDIO2_CHUNK_H
#define XAUDIO2_CHUNK_H

#include "../../MS2/dist/File_Slow/include/File_Slow.h"
//#include "File.h"

enum ChunkError
{
	CHUNK_SUCCESS = 0xC0000000,
	CHUNK_FAIL,

};


ChunkError FindChunk_Slow(File_Slow::Handle hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
ChunkError ReadChunkData_Slow(File_Slow::Handle hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);

//ChunkError FindChunk(FileHandle hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition);
//ChunkError ReadChunkData(FileHandle hFile, void * buffer, DWORD buffersize, DWORD bufferoffset);

#endif

// End of File 
 