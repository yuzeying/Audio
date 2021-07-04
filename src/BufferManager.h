#pragma once
#include <xaudio2.h>
#include "../../MS2/dist/File_Slow/include/File_Slow.h"

class BufferManager {
public:

	static void LoadBuffer_slow(const char* pFileName, WAVEFORMATEXTENSIBLE& wfx, XAUDIO2_BUFFER& buffer) {
		File_Slow::Handle han;
		File_Slow::Error err;

		// Open file
		// Based on the input parameter
		
		err = File_Slow::open(han, pFileName, File_Slow::Mode::READ);

		// Set file to beginning
		err = File_Slow::seek(han, File_Slow::Seek::BEGIN, 0);

		// -------------------------------------------------------
		// Audio Format - wfx
		// Audio Buffer - how to play it back
		// -------------------------------------------------------

		wfx = { 0 };
		buffer = { 0 };

		// -------------------------------------------------------
		// Find and load specific Chunks
		// -------------------------------------------------------

		DWORD dwChunkSize;
		DWORD dwChunkPosition;
		DWORD filetype;
		ChunkError cerror;

		// Scan to the RIFF and load file type
		cerror = FindChunk_Slow(han, fourccRIFF, dwChunkSize, dwChunkPosition);
		assert(cerror == ChunkError::CHUNK_SUCCESS);
		cerror = ReadChunkData_Slow(han, &filetype, sizeof(DWORD), dwChunkPosition);
		assert(cerror == ChunkError::CHUNK_SUCCESS);

		// Make sure its not in the compressed format, WAVE format is uncompressed
		if (filetype != fourccWAVE)
		{
			assert(false);
		}

		// Read the FMT: format
		cerror = FindChunk_Slow(han, fourccFMT, dwChunkSize, dwChunkPosition);
		assert(cerror == ChunkError::CHUNK_SUCCESS);
		cerror = ReadChunkData_Slow(han, &wfx, dwChunkSize, dwChunkPosition);
		assert(cerror == ChunkError::CHUNK_SUCCESS);

		// Scan to the DATA chunk, read the size, allocate buffer of that size
		cerror = FindChunk_Slow(han, fourccDATA, dwChunkSize, dwChunkPosition);
		assert(cerror == ChunkError::CHUNK_SUCCESS);

		BYTE* pDataBuffer = new BYTE[dwChunkSize];
		assert(pDataBuffer != 0);

		// Fill the data...
		cerror = ReadChunkData_Slow(han, pDataBuffer, dwChunkSize, dwChunkPosition);
		assert(cerror == ChunkError::CHUNK_SUCCESS);

		buffer.AudioBytes = dwChunkSize;  //buffer containing audio data
		buffer.pAudioData = pDataBuffer;  //size of the audio buffer in bytes
		buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
		buffer.LoopCount = 0;				  // looping...

		err = File_Slow::close(han);
		assert(err == File_Slow::Error::SUCCESS);
	}
	/*
	static void LoadBuffer(const char* pFileName, WAVEFORMATEXTENSIBLE& wfx, XAUDIO2_BUFFER& buffer)
	{
		// -------------------------------------------------------
		// Open File
		// -------------------------------------------------------

		HANDLE FileHandle;
		FileError ferror;

		// Open file
		// Based on the input parameter
		ferror = File::open(FileHandle, pFileName, FileMode::FILE_READ);
		assert(ferror == FILE_SUCCESS);

		// Set file to beginning
		ferror = File::seek(FileHandle, FileSeek::FILE_SEEK_BEGIN, 0);
		assert(ferror == FILE_SUCCESS);

		// -------------------------------------------------------
		// Audio Format - wfx
		// Audio Buffer - how to play it back
		// -------------------------------------------------------

		wfx = { 0 };
		buffer = { 0 };

		// -------------------------------------------------------
		// Find and load specific Chunks
		// -------------------------------------------------------

		DWORD dwChunkSize;
		DWORD dwChunkPosition;
		DWORD filetype;
		ChunkError cerror;

		// Scan to the RIFF and load file type
		cerror = FindChunk(FileHandle, fourccRIFF, dwChunkSize, dwChunkPosition);
		assert(cerror == ChunkError::CHUNK_SUCCESS);
		cerror = ReadChunkData(FileHandle, &filetype, sizeof(DWORD), dwChunkPosition);
		assert(cerror == ChunkError::CHUNK_SUCCESS);

		// Make sure its not in the compressed format, WAVE format is uncompressed
		if (filetype != fourccWAVE)
		{
			assert(false);
		}

		// Read the FMT: format
		cerror = FindChunk(FileHandle, fourccFMT, dwChunkSize, dwChunkPosition);
		assert(cerror == ChunkError::CHUNK_SUCCESS);
		cerror = ReadChunkData(FileHandle, &wfx, dwChunkSize, dwChunkPosition);
		assert(cerror == ChunkError::CHUNK_SUCCESS);

		// Scan to the DATA chunk, read the size, allocate buffer of that size
		cerror = FindChunk(FileHandle, fourccDATA, dwChunkSize, dwChunkPosition);
		assert(cerror == ChunkError::CHUNK_SUCCESS);

		BYTE* pDataBuffer = new BYTE[dwChunkSize];
		assert(pDataBuffer != 0);

		// Fill the data...
		cerror = ReadChunkData(FileHandle, pDataBuffer, dwChunkSize, dwChunkPosition);
		assert(cerror == ChunkError::CHUNK_SUCCESS);

		buffer.AudioBytes = dwChunkSize;  //buffer containing audio data
		buffer.pAudioData = pDataBuffer;  //size of the audio buffer in bytes
		buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
		buffer.LoopCount = 0;				  // looping...

		ferror = File::close(FileHandle);
		assert(ferror == FILE_SUCCESS);
	}
	*/

	static void Initialize() {
		head = nullptr;
	}

	static void LoadFileIntoBuffer(const char* pFileName, int id) {
		BufferBlock* newBlock = new BufferBlock(pFileName, id, true);
		newBlock = newBlock;
	}

	static void Destroy() {
		BufferBlock* cur = head, *temp = nullptr;
		while (cur != nullptr) {
			temp = cur->next;
			delete cur->buffer.pAudioData;
			delete cur;
			cur = temp;
		}
		head = nullptr;
	}

	static XAUDIO2_BUFFER* GetBufferFromId(int id) {
		BufferBlock* cur = head;
		while (cur != nullptr) {
			if (cur->id == id) return &(cur->buffer);
			cur = cur->next;
		}
		return nullptr;
	}

	static WAVEFORMATEXTENSIBLE* GetWFXFromId(int id) {
		BufferBlock* cur = head;
		while (cur != nullptr) {
			if (cur->id == id) return &(cur->wfx);
			cur = cur->next;
		}
		return nullptr;
	}

	class BufferBlock {
	public:
		/*
		BufferBlock(const char* pFileName, int id) {
			this->next = head;
			head = this;

			this->id = id;
			LoadBuffer(pFileName, wfx, buffer);
		}*/
		BufferBlock(const char* pFileName, int id, bool slow) {
			this->next = head;
			head = this;
			slow = false;

			this->id = id;
			LoadBuffer_slow(pFileName, wfx, buffer);
		}
		BufferBlock* next;
		int id;
		XAUDIO2_BUFFER buffer;
		WAVEFORMATEXTENSIBLE wfx;
	};
private:
	

	static BufferBlock* head;
};

BufferManager::BufferBlock* BufferManager::head = nullptr;
