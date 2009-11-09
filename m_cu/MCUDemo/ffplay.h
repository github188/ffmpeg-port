#pragma once


#include "portab.h"
#include "mcudef.h"

#include "avcodec.h"

static FILE *log_file;



#ifdef __cplusplus
extern "C"{
#endif

//	typedef struct AVFrame;
//	typedef struct AVCodecContext;

	typedef void (*fYUV420CallBack)( AVFrame *pFrame, int width, int height, void *param );
	

	int begin( const char *fname, int hWnd );
	void beginrtsp( char *url, int hWnd );


	/** 创建解码器，返回解码器ID。 */
	DecoderID CreateDecoder(  int eCodecId  );

	/** 销毁解码器。 */
	void DestroyDecoder( DecoderID did );

	/** 传入数据。 */
	int InputData( DecoderID decoderId, unsigned char* pBuf, unsigned len );

	/** 设置解码图像输出回调。 */
	void SetYUVCallBack( DecoderID decoderId, fYUV420CallBack pfYUVcb, void *param );

	int decode_frame(AVCodecContext *avctx, void *data, int *data_size, const uint8_t *buf, int buf_size);

#ifdef __cplusplus

};

#endif



