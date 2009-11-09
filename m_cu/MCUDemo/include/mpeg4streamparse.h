#pragma once

#include "mem.h"

class CMPEG4StreamParse
{
public:
	CMPEG4StreamParse(void);
	~CMPEG4StreamParse(void);

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
