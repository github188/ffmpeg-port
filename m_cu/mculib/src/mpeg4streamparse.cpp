#include "mpeg4streamparse.h"

#define END_NOT_FOUND (-100)

/**
* Required number of additionally allocated bytes at the end of the input bitstream for decoding.
* this is mainly needed because some optimized bitstream readers read 
* 32 or 64 bit at once and could read over the end<br>
* Note, if the first 23 bits of the additional bytes are not 0 then damaged
* MPEG bitstreams could cause overread and segfault
*/
#define FF_INPUT_BUFFER_PADDING_SIZE 8

CMPEG4StreamParse::CMPEG4StreamParse(void)
{
}

CMPEG4StreamParse::~CMPEG4StreamParse(void)
{
}

mu_uint32 CMPEG4StreamParse::InputStream(void *mediaBuf, mu_uint32 bufLen)
{
	mu_int32 next = this->FindFrameEnd( &this->m_tParseContext, mediaBuf, bufLen );

	if ( END_NOT_FOUND != next )
	{
		next --;
	}


	mu_int32 nBufLen = bufLen;
	mu_uint8 *pBuf = ( mu_uint8 * )mediaBuf;

	mu_int32 combineLen = this->CombineFrame( &this->m_tParseContext, next, &pBuf, &nBufLen );

	if ( combineLen < 0 )
	{
		return bufLen;
	}
	else
	{
		return next ;
	}
}


BOOL CMPEG4StreamParse::GetCurFrame( void **buf, mu_uint32 *buflen )
{
	if ( m_bCompleteFrame )
	{
		*buf = m_tParseContext.buffer;
		*buflen = m_tParseContext.overread_index;
	}
	return m_bCompleteFrame;
}

mu_int32 CMPEG4StreamParse::FindFrameEnd( ParseContext *pc, void *mediaBuf, mu_uint32 bufLen )
{
	int vop_found, i;
	mu_int32 state;
	int buf_size = bufLen;
	mu_uint8 *buf = ( mu_uint8 * )mediaBuf;

	vop_found= pc->frame_start_found;
	state= pc->state;

	i=0;
	if(!vop_found)
	{
		for(i=0; i<buf_size; i++)
		{
			state= (state<<8) | buf[i];
			if(state == 0x1B6)
			{
				i++;
				vop_found=1;
				break;
			}
		}
	}

	if(vop_found){
		/* EOF considered as end of frame */
		if (buf_size == 0)
			return 0;
		for(; i<buf_size; i++)
		{
			state= (state<<8) | buf[i];
			if((state&0xFFFFFF00) == 0x100)
			{
				pc->frame_start_found=0;
				pc->state = -1; 
				return i-3;
			}
		}
	}
	pc->frame_start_found= vop_found;
	pc->state= state;
	return END_NOT_FOUND;
}

mu_int32 CMPEG4StreamParse::CombineFrame( ParseContext *pc, mu_int32 next, mu_uint8 ** ppBuf, mu_int32 *pBuf_size )
{
	mu_int32 *buf_size = pBuf_size;
	mu_uint8 **buf = ppBuf;

#if 0
	if(pc->overread){
		printf("overread %d, state:%X next:%d index:%d o_index:%d\n", pc->overread, pc->state, next, pc->index, pc->overread_index);
		printf("%X %X %X %X\n", (*buf)[0], (*buf)[1],(*buf)[2],(*buf)[3]);
	}
#endif

	m_bCompleteFrame = ( END_NOT_FOUND != next );

	/* copy overreaded bytes from last frame into buffer */
	for(; pc->overread>0; pc->overread--){
		pc->buffer[pc->index++]= pc->buffer[pc->overread_index++];
	}

	/* flush remaining if EOF */
	if(!*buf_size && next == END_NOT_FOUND){
		next= 0;
	}

	pc->last_index= pc->index;

	/* copy into buffer end return */
	if(next == END_NOT_FOUND){
		pc->buffer= (mu_uint8*)mu_fast_realloc(pc->buffer, ( mu_uint32* )&pc->buffer_size, (*buf_size) + pc->index + FF_INPUT_BUFFER_PADDING_SIZE);

		memcpy(&pc->buffer[pc->index], *buf, *buf_size);
		pc->index += *buf_size;
		return -1;
	}

	*buf_size=
		pc->overread_index= pc->index + next;

	/* append to buffer */
	//	if(pc->index)
	{
		pc->buffer= (mu_uint8*)mu_fast_realloc(pc->buffer, (  mu_uint32*  )&pc->buffer_size, next + pc->index + FF_INPUT_BUFFER_PADDING_SIZE);

		memcpy(&pc->buffer[pc->index], *buf, next + FF_INPUT_BUFFER_PADDING_SIZE );
		pc->index = 0;
		*buf= pc->buffer;
	}

	/* store overread bytes */
	for(;next < 0; next++)
	{
		pc->state = (pc->state<<8) | pc->buffer[pc->last_index + next];
		pc->overread++;
	}

#if 0
	if(pc->overread){
		printf("overread %d, state:%X next:%d index:%d o_index:%d\n", pc->overread, pc->state, next, pc->index, pc->overread_index);
		printf("%X %X %X %X\n", (*buf)[0], (*buf)[1],(*buf)[2],(*buf)[3]);
	}
#endif

	return 0;
}

