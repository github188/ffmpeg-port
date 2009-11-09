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


	/** ���������������ؽ�����ID�� */
	DecoderID CreateDecoder(  int eCodecId  );

	/** ���ٽ������� */
	void DestroyDecoder( DecoderID did );

	/** �������ݡ� */
	int InputData( DecoderID decoderId, unsigned char* pBuf, unsigned len );

	/** ���ý���ͼ������ص��� */
	void SetYUVCallBack( DecoderID decoderId, fYUV420CallBack pfYUVcb, void *param );

	int decode_frame(AVCodecContext *avctx, void *data, int *data_size, const uint8_t *buf, int buf_size);

#ifdef __cplusplus

};

#endif



