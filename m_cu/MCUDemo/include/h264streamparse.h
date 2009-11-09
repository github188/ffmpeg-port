#pragma once


#include "mem.h"


#define END_NOT_FOUND (-100)

/**
* Required number of additionally allocated bytes at the end of the input bitstream for decoding.
* this is mainly needed because some optimized bitstream readers read 
* 32 or 64 bit at once and could read over the end<br>
* Note, if the first 23 bits of the additional bytes are not 0 then damaged
* MPEG bitstreams could cause overread and segfault
*/
#define FF_INPUT_BUFFER_PADDING_SIZE 8

class CH264StreamParse
{

private:
	typedef struct ParseContext{

		ParseContext()
		{
			memset( this, 0, sizeof(ParseContext) );
		}

		~ParseContext()
		{
			mu_freep( &buffer );
		}

		mu_uint8 *buffer;
		int index;
		int last_index;
		int buffer_size;
		mu_uint32 state;             ///< contains the last few bytes in MSB order
		int frame_start_found;
		int overread;               ///< the number of bytes which where irreversibly read from the next frame
		int overread_index;         ///< the index into ParseContext.buffer of the overreaded bytes
	} ParseContext;

public:
	CH264StreamParse();
	~CH264StreamParse();


	/** 输入数据. */
	mu_uint32 InputStream( void *buf, mu_uint32 buflen );

	/** 获取当前帧数据. */
	BOOL GetCurFrame( void **buf, mu_uint32 *buflen );

	/** 重置. */
	void Reset();

private:

	/** 寻找码流中帧结束位置. */
	mu_int32 FindFrameEnd( ParseContext *pc, void *buf, mu_uint32 buflen );

	/** */
	mu_int32 CombineFrame( ParseContext *pc, mu_int32 next, mu_uint8 ** ppBuf, mu_int32 *pBuf_size );

private:
	ParseContext m_tParseContext;

	/** 是否有完整的帧. */
	BOOL m_bCompleteFrame;
};