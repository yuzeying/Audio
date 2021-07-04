//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef XAUDIO2_ENDIANESS
#define XAUDIO2_ENDIANESS

#ifdef _XBOX //Big-Endian
	#define fourccRIFF 'RIFF'
	#define fourccDATA 'data'
	#define fourccFMT 'fmt '
	#define fourccWAVE 'WAVE'
	#define fourccXWMA 'XWMA'
	#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
	#define fourccRIFF 'FFIR'
	#define fourccDATA 'atad'
	#define fourccFMT ' tmf'
	#define fourccWAVE 'EVAW'
	#define fourccXWMA 'AMWX'
	#define fourccDPDS 'sdpd'
#endif

#endif

// End of File 
