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



	/** ��������. */
	mu_uint32 InputStream( void *buf, mu_uint32 buflen );

	/** ��ȡ��ǰ֡����. */
	BOOL GetCurFrame( void **buf, mu_uint32 *buflen );

	/** ����. */
	void Reset();

private:

	/** Ѱ��������֡����λ��. */
	mu_int32 FindFrameEnd( ParseContext *pc, void *buf, mu_uint32 buflen );

	/** */
	mu_int32 CombineFrame( ParseContext *pc, mu_int32 next, mu_uint8 ** ppBuf, mu_int32 *pBuf_size );

private:
	ParseContext m_tParseContext;

	/** �Ƿ���������֡. */
	BOOL m_bCompleteFrame;
};
