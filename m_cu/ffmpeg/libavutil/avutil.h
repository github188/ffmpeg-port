#ifndef AVUTIL_H
#define AVUTIL_H

/**
 * @file avutil.h
 * external api header.
 */


#ifdef __cplusplus
extern "C" {
#endif


#define AV_VERSION_INT(a, b, c) (a<<16 | b<<8 | c)
#define AV_VERSION_DOT(a, b, c) a ##.## b ##.## c
#define AV_VERSION(a, b, c) AV_VERSION_DOT(a, b, c)


#define AV_STRINGIFY(s)	AV_TOSTRING(s)
#define AV_TOSTRING(s) #s

#define LIBAVUTIL_VERSION_INT ((49<<16)+(0<<8)+0)
#define LIBAVUTIL_VERSION     49.0.0
#define LIBAVUTIL_BUILD       LIBAVUTIL_VERSION_INT

#define LIBAVUTIL_IDENT       "Lavu" AV_STRINGIFY(LIBAVUTIL_VERSION)


#include "common.h"
#include "mathematics.h"
#include "rational.h"
#include "integer.h"
#include "intfloat_readwrite.h"
#include "errno.h"

#ifdef __cplusplus
}
#endif

#endif /* AVUTIL_H */
