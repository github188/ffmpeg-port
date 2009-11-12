#include "mcudefine.h"

/** 解码器名. */
LPCTSTR MAIN_DECODER_NAME = _T( "mainhdecoder" );

LPCTSTR MAIN_KEDA_DECODER_NAME = _T( "mainkedadecoder" );

/** 程序的界面图片目录。 */
LPCTSTR UI_PIC_DIR = _T( "res\\chs\\" );

/** 最小的存储剩余空间阀值。小于此阀值，抓拍和录像全部停止或失败。 */
const int MIN_STORAGE_SPACE = 5 << 20; // 5M

// 加密密钥。
char s_EncKey[16] = { 'M','_','C','U','b','y','t','h','i','n','k','i','n','g','l','!' };





