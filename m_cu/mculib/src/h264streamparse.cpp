#include "h264streamparse.h"
#include "mem.h"

#include <stdlib.h>

//decoderextend::decoderextend(void)
//{
//}
//
//decoderextend::~decoderextend(void)
//{
//}


//static ParseContext s_tParseContext;




/**
* finds the end of the current frame in the bitstream.
* @return the position of the first byte of the next frame, or -1
*/
//static int find_frame_end( const u8 *buf, int buf_size)
//{
//	int i;
//	u32 state;
//	ParseContext *pc = &s_tParseContext;
//	//printf("first %02X%02X%02X%02X\n", buf[0], buf[1],buf[2],buf[3]);
//	//    mb_addr= pc->mb_addr - 1;
//	state= pc->state;
//	for(i=0; i<=buf_size; i++){
//		if((state&0xFFFFFF1F) == 0x101 || (state&0xFFFFFF1F) == 0x102 || (state&0xFFFFFF1F) == 0x105){
//			printf("find_frame_end new startcode = %08x, frame_start_found = %d, pos = %d\n", state, pc->frame_start_found, i);
//			if(pc->frame_start_found){
//				// If there isn't one more byte in the buffer
//				// the test on first_mb_in_slice cannot be done yet
//				// do it at next call.
//				if (i >= buf_size) break;
//				if (buf[i] & 0x80) {
//					// first_mb_in_slice is 0, probably the first nal of a new
//					// slice
//					printf("find_frame_end frame_end_found, state = %08x, pos = %d\n", state, i);
//					pc->state=-1; 
//					pc->frame_start_found= 0;
//					return i-4;
//				}
//			}
//			pc->frame_start_found = 1;
//		}
//		if((state&0xFFFFFF1F) == 0x107 || (state&0xFFFFFF1F) == 0x108 || (state&0xFFFFFF1F) == 0x109){
//			if(pc->frame_start_found){
//				pc->state=-1; 
//				pc->frame_start_found= 0;
//				return i-4;               
//			}
//		}
//		if (i<buf_size)
//		{
//			state= (state<<8) | buf[i];
//		}
//	}
//
//	pc->state= state;
//	return END_NOT_FOUND;
//}
//
//
///**
//* combines the (truncated) bitstream to a complete frame
//* @returns -1 if no complete frame could be created
//*/
//int ff_combine_frame(ParseContext *pc, int next, u8 **buf, int *buf_size)
//{
//#if 0
//	if(pc->overread){
//		printf("overread %d, state:%X next:%d index:%d o_index:%d\n", pc->overread, pc->state, next, pc->index, pc->overread_index);
//		printf("%X %X %X %X\n", (*buf)[0], (*buf)[1],(*buf)[2],(*buf)[3]);
//	}
//#endif
//
//	/* copy overreaded bytes from last frame into buffer */
//	for(; pc->overread>0; pc->overread--){
//		pc->buffer[pc->index++]= pc->buffer[pc->overread_index++];
//	}
//
//	/* flush remaining if EOF */
//	if(!*buf_size && next == END_NOT_FOUND){
//		next= 0;
//	}
//
//	pc->last_index= pc->index;
//
//	/* copy into buffer end return */
//	if(next == END_NOT_FOUND){
//		pc->buffer= (u8*)mu_fast_realloc(pc->buffer, (unsigned int*)&pc->buffer_size, (*buf_size) + pc->index + FF_INPUT_BUFFER_PADDING_SIZE);
//
//		memcpy(&pc->buffer[pc->index], *buf, *buf_size);
//		pc->index += *buf_size;
//		return -1;
//	}
//
//	*buf_size=
//		pc->overread_index= pc->index + next;
//
//	/* append to buffer */
//	if(pc->index){
//		pc->buffer= (u8*)mu_fast_realloc(pc->buffer, (unsigned int*)&pc->buffer_size, next + pc->index + FF_INPUT_BUFFER_PADDING_SIZE);
//
//		memcpy(&pc->buffer[pc->index], *buf, next + FF_INPUT_BUFFER_PADDING_SIZE );
//		pc->index = 0;
//		*buf= pc->buffer;
//	}
//
//	/* store overread bytes */
//	for(;next < 0; next++){
//		pc->state = (pc->state<<8) | pc->buffer[pc->last_index + next];
//		pc->overread++;
//	}
//
//#if 0
//	if(pc->overread){
//		printf("overread %d, state:%X next:%d index:%d o_index:%d\n", pc->overread, pc->state, next, pc->index, pc->overread_index);
//		printf("%X %X %X %X\n", (*buf)[0], (*buf)[1],(*buf)[2],(*buf)[3]);
//	}
//#endif
//
//	return 0;
//}
//
///**
//* returns the number of bytes consumed for building the current frame
//*/
//static int get_consumed_bytes( int pos, int buf_size){
//	pos -= s_tParseContext.last_index;
//	if(pos<0) pos=0; // FIXME remove (unneeded?)
//
//	return pos;
//}

//int DecodeExtend( void *pBuf, size_t len, void ** frameBufBegin, int *pFrameBufLen )
//{
//	u8 *buf = ( u8 * )pBuf;
//	int next= find_frame_end( buf, len );
//
//	int buf_size = len;
//	if( ff_combine_frame(&s_tParseContext, next, &buf, &buf_size) < 0 )
//	{
//		*frameBufBegin = NULL;
//		*pFrameBufLen = 0;
//		
//		return buf_size;
//	}
//	else
//	{
//		*frameBufBegin = s_tParseContext.buffer;
//		*pFrameBufLen = s_tParseContext.buffer_size;
////		int nlen = get_consumed_bytes( 0, buf_size );
//		int nlen = next;
//		return nlen;
//	}
//	
//
//}

CH264StreamParse::CH264StreamParse()
{
	m_bCompleteFrame = FALSE;
}

CH264StreamParse::~CH264StreamParse()
{

}

mu_uint32 CH264StreamParse::InputStream( void *mediaBuf, mu_uint32 bufLen )
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

mu_int32 CH264StreamParse::FindFrameEnd( ParseContext *pc, void *streamBuf, mu_uint32 buf_size )
{
	mu_uint32 i;
	mu_int32 state;
	mu_uint8 *buf = (mu_uint8*)streamBuf;
	//printf("first %02X%02X%02X%02X\n", buf[0], buf[1],buf[2],buf[3]);
	//    mb_addr= pc->mb_addr - 1;
	state= pc->state;
	for(i=0; i<=buf_size; i++){
		if((state&0xFFFFFF1F) == 0x101 || (state&0xFFFFFF1F) == 0x102 || (state&0xFFFFFF1F) == 0x105){
			// printf("find_frame_end new startcode = %08x, frame_start_found = %d, pos = %d\n", state, pc->frame_start_found, i);
			if(pc->frame_start_found){
				// If there isn't one more byte in the buffer
				// the test on first_mb_in_slice cannot be done yet
				// do it at next call.
				if (i >= buf_size) break;
				if (buf[i] & 0x80) {
					// first_mb_in_slice is 0, probably the first nal of a new
					// slice
					// printf("find_frame_end frame_end_found, state = %08x, pos = %d\n", state, i);
					pc->state = (-1); 
					pc->frame_start_found= 0;

//					int kkw = i-5;
//					printf("buf data: %02X%02X%02X%02X%02X\n", buf[kkw++], buf[kkw++],buf[kkw++],buf[kkw++],buf[kkw++]);

					return i-4;
				}
			}
			pc->frame_start_found = 1;
		}
		if((state&0xFFFFFF1F) == 0x107 || (state&0xFFFFFF1F) == 0x108 || (state&0xFFFFFF1F) == 0x109){
			if(pc->frame_start_found){
				pc->state = -1; 
				pc->frame_start_found= 0;
				return i-4;               
			}
		}
		if (i<buf_size)
		{
			state= (state<<8) | buf[i];
		}
	}

	pc->state= state;
	return END_NOT_FOUND;
}

mu_int32 CH264StreamParse::CombineFrame( ParseContext *pc, mu_int32 next, mu_uint8 ** ppBuf, mu_int32 *pBuf_size )
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

void CH264StreamParse::Reset()
{
	memset( &m_tParseContext, 0, sizeof(  m_tParseContext ) );
	m_bCompleteFrame = FALSE;
}

BOOL CH264StreamParse::GetCurFrame( void **buf, mu_uint32 *buflen )
{
	if ( m_bCompleteFrame )
	{
		*buf = m_tParseContext.buffer;
		*buflen = m_tParseContext.overread_index;
	}
	return m_bCompleteFrame;
}

