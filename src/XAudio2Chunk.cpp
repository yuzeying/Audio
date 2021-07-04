//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "XAudio2Chunk.h"
#include "XAudio2Endianess.h"

// ----------------------------------------------------------------
// Find wave audio data chunk
// ----------------------------------------------------------------

ChunkError FindChunk_Slow(File_Slow::Handle hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
{
	ChunkError cerror = ChunkError::CHUNK_SUCCESS;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwOffset = 0;

	// Set file pointer to the beginning
	if (File_Slow::Error::SUCCESS != File_Slow::seek(hFile, File_Slow::Seek::BEGIN, 0))
	{
		cerror = ChunkError::CHUNK_FAIL;
	}

	// loop until you find it
	while (cerror == ChunkError::CHUNK_SUCCESS)
	{
		// Read data Type
		if (File_Slow::Error::SUCCESS != File_Slow::read(hFile, &dwChunkType, sizeof(DWORD)))
		{
			cerror = ChunkError::CHUNK_FAIL;
		}

		// Read data size
		if (File_Slow::Error::SUCCESS != File_Slow::read(hFile, &dwChunkDataSize, sizeof(DWORD)))
		{
			cerror = ChunkError::CHUNK_FAIL;
		}

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;

			// Read data size, special format for RIFF header
			if (File_Slow::Error::SUCCESS != File_Slow::read(hFile, &dwFileType, sizeof(DWORD)))
			{
				cerror = ChunkError::CHUNK_FAIL;
			}
			break;

		default:
			// Move file pointer to the next Chunk, continue
			if (File_Slow::Error::SUCCESS != File_Slow::seek(hFile, File_Slow::Seek::CURRENT, dwChunkDataSize))
			{
				cerror = ChunkError::CHUNK_FAIL;
			}
		}

		// Since the headers are only 2 words long... 
		// Keep a running total
		dwOffset += sizeof(DWORD) * 2;

		// Found type
		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			break;
		}

		dwOffset += dwChunkDataSize;

	}  // while()

	return cerror;
}

/*
ChunkError FindChunk(FileHandle hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	ChunkError cerror = ChunkError::CHUNK_SUCCESS;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwOffset = 0;

	// Set file pointer to the beginning
	if (FileError::FILE_SUCCESS != File::seek(hFile, FileSeek::FILE_SEEK_BEGIN, 0))
	{
		cerror = ChunkError::CHUNK_FAIL;
	}

	// loop until you find it
	while (cerror == ChunkError::CHUNK_SUCCESS)
	{
		// Read data Type
		if (FileError::FILE_SUCCESS != File::read(hFile, &dwChunkType, sizeof(DWORD)))
		{
			cerror = ChunkError::CHUNK_FAIL;
		}

		// Read data size
		if (FileError::FILE_SUCCESS != File::read(hFile, &dwChunkDataSize, sizeof(DWORD)))
		{
			cerror = ChunkError::CHUNK_FAIL;
		}

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;

			// Read data size, special format for RIFF header
			if (FileError::FILE_SUCCESS != File::read(hFile, &dwFileType, sizeof(DWORD)))
			{
				cerror = ChunkError::CHUNK_FAIL;
			}
			break;

		default:
			// Move file pointer to the next Chunk, continue
			if (FileError::FILE_SUCCESS != File::seek(hFile, FileSeek::FILE_SEEK_CURRENT, dwChunkDataSize))
			{
				cerror = ChunkError::CHUNK_FAIL;
			}
		}

		// Since the headers are only 2 words long... 
		// Keep a running total
		dwOffset += sizeof(DWORD) * 2;

		// Found type
		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			break;
		}

		dwOffset += dwChunkDataSize;

	}  // while()

	return cerror;
}
*/
// ---------------------------------------------------------------------
// Read wave data chunk, using our FILE system
// ---------------------------------------------------------------------

ChunkError ReadChunkData_Slow(File_Slow::Handle hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{ // File_Slow::Error::SUCCESS != File_Slow

	ChunkError cerror = ChunkError::CHUNK_SUCCESS;

	// Seek to the offset to chunk
	if (File_Slow::Error::SUCCESS != File_Slow::seek(hFile, File_Slow::Seek::BEGIN, bufferoffset))
	{
		cerror = ChunkError::CHUNK_FAIL;
	}

	// Read data chunk 
	if (File_Slow::Error::SUCCESS != File_Slow::read(hFile, buffer, buffersize))
	{
		cerror = ChunkError::CHUNK_FAIL;
	}

	return cerror;
}
/*
ChunkError ReadChunkData(FileHandle hFile, void * buffer, DWORD buffersize, DWORD bufferoffset)
{
	ChunkError cerror = ChunkError::CHUNK_SUCCESS;

	// Seek to the offset to chunk
	if( FileError::FILE_SUCCESS != File::seek(hFile, FileSeek::FILE_SEEK_BEGIN, bufferoffset) )
	{
		cerror = ChunkError::CHUNK_FAIL;
	}

	// Read data chunk 
	if (FileError::FILE_SUCCESS != File::read(hFile, buffer, buffersize))
	{
		cerror = ChunkError::CHUNK_FAIL;
	}

	return cerror;
}*/

// End of File 
