/**
 * @file common.h
 * common internal api header.
 */



#ifndef COMMON_H
#define COMMON_H

#include "typeport.h"
#include "errno.h"

#if defined(WIN32) && !defined(__MINGW32__) && !defined(__CYGWIN__)
#    define CONFIG_WIN32
#endif

#ifdef TCPMP

//#define CONFIG_ZLIB
#if defined(_WIN32)
#if defined(_MSC_VER)
#define CONFIG_WIN32
#define CONFIG_ALIGN
#endif
#if defined(_WIN32_WCE)
#define perror(n)
// #define sscanf stscanf  by thinkingl. wince有sscanf,却没有stscanf的定义.
#endif
#endif

#if defined(__palmos__)
typedef void* FILE;
#include <stdint.h>
#define	ENOMEM 12
#define perror(n)
#define sscanf stscanf
#endif

#define HAVE_AV_CONFIG_H
#undef assert
#define assert(p)
#undef _DEBUG
#undef DEBUG

#endif

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif

#ifdef HAVE_AV_CONFIG_H
/* only include the following when compiling package */
#    include "config.h"

#    include <stdlib.h>
#    include <stdio.h>
#    include <string.h>
#    include <ctype.h>
#    include <limits.h>
#    ifndef __BEOS__
//#        include <errno.h> //Picard
#    else
#        include "berrno.h"
#    endif
//#    include <math.h> //Picard

#    ifndef ENODATA
#        define ENODATA  61
#    endif

//#ifndef ENOMEM //Picard
//#define	ENOMEM 12
//#endif

#include <stddef.h>
#ifndef offsetof
# define offsetof(T,F) ((unsigned int)((char *)&((T *)0)->F))
#endif

#define AVOPTION_CODEC_BOOL(name, help, field) \
    { name, help, offsetof(AVCodecContext, field), FF_OPT_TYPE_BOOL }
#define AVOPTION_CODEC_DOUBLE(name, help, field, minv, maxv, defval) \
    { name, help, offsetof(AVCodecContext, field), FF_OPT_TYPE_DOUBLE, minv, maxv, defval }
#define AVOPTION_CODEC_FLAG(name, help, field, flag, defval) \
    { name, help, offsetof(AVCodecContext, field), FF_OPT_TYPE_FLAG, flag, 0, defval }
#define AVOPTION_CODEC_INT(name, help, field, minv, maxv, defval) \
    { name, help, offsetof(AVCodecContext, field), FF_OPT_TYPE_INT, minv, maxv, defval }
#define AVOPTION_CODEC_STRING(name, help, field, str, val) \
    { name, help, offsetof(AVCodecContext, field), FF_OPT_TYPE_STRING, .defval = val, .defstr = str }
#define AVOPTION_CODEC_RCOVERRIDE(name, help, field) \
    { name, help, offsetof(AVCodecContext, field), FF_OPT_TYPE_RCOVERRIDE, .defval = 0, .defstr = NULL }
#define AVOPTION_SUB(ptr) { .name = NULL, .help = (const char*)ptr }
#define AVOPTION_END() AVOPTION_SUB(NULL)

#endif /* HAVE_AV_CONFIG_H */

/* Suppress restrict if it was not defined in config.h.  */
#ifndef restrict
#    define restrict
#endif

// by thinkingl
#ifndef always_inline
#define always_inline av_always_inline
#endif

#ifndef av_always_inline
#if defined(__GNUC__) && (__GNUC__ > 3 || __GNUC__ == 3 && __GNUC_MINOR__ > 0)
#    define av_always_inline __attribute__((av_always_inline)) inline
#else
#    define av_always_inline inline
#endif
#endif

#ifndef av_noinline
#if defined(__GNUC__) && (__GNUC__ > 3 || __GNUC__ == 3 && __GNUC_MINOR__ > 0)
#    define av_noinline __attribute__((noinline))
#else
#    define av_noinline
#endif
#endif

#ifndef av_pure
#if defined(__GNUC__) && (__GNUC__ > 3 || __GNUC__ == 3 && __GNUC_MINOR__ > 0)
#    define av_pure __attribute__((pure))
#else
#    define av_pure
#endif
#endif

#ifndef av_const
#if defined(__GNUC__) && (__GNUC__ > 2 || __GNUC__ == 2 && __GNUC_MINOR__ > 5)
#    define av_const __attribute__((const))
#else
#    define av_const
#endif
#endif

#ifndef av_cold
#if defined(__GNUC__) && (__GNUC__ > 4 || __GNUC__ == 4 && __GNUC_MINOR__ > 2)
#    define av_cold __attribute__((cold))
#else
#    define av_cold
#endif
#endif




#ifndef attribute_used
#if defined(__GNUC__) && (__GNUC__ > 3 || __GNUC__ == 3 && __GNUC_MINOR__ > 0)
#    define attribute_used __attribute__((used))
#else
#    define attribute_used
#endif
#endif

#ifndef attribute_unused
#if defined(__GNUC__) && (__GNUC__ > 3 || __GNUC__ == 3 && __GNUC_MINOR__ > 0)
#    define attribute_unused __attribute__((unused))
#else
#    define attribute_unused
#endif
#endif

#ifndef av_unused
#define av_unused attribute_unused
#endif

//
//#ifndef EMULATE_INTTYPES
////#   include <inttypes.h> //Picard
//#   include <stdint.h>
//#else
//    typedef signed char  int8_t;
//    typedef signed short int16_t;
//    typedef signed long   int32_t; //Picard
//    typedef unsigned char  uint8_t;
//    typedef unsigned short uint16_t;
//    typedef unsigned long   uint32_t; //Picard
//
//#   ifdef CONFIG_WIN32
//        typedef signed __int64   int64_t;
//        typedef unsigned __int64 uint64_t;
//#   else /* other OS */
//        typedef signed long long   int64_t;
//        typedef unsigned long long uint64_t;
//#   endif /* other OS */
//#endif /* EMULATE_INTTYPES */

#ifndef INT16_MIN
#define INT16_MIN       (-0x7fff-1)
#endif

#ifndef INT16_MAX
#define INT16_MAX       0x7fff
#endif

#ifndef INT64_MIN
#define INT64_MIN       (-0x7fffffffffffffff-1)
#endif

#ifndef INT64_MAX
#define INT64_MAX int64_t_C(9223372036854775807)
#endif

#ifndef UINT64_MAX
#define UINT64_MAX uint64_t_C(0xFFFFFFFFFFFFFFFF)
#endif

#ifdef EMULATE_FAST_INT
typedef signed char int_fast8_t;
typedef signed int  int_fast16_t;
typedef signed int  int_fast32_t;
typedef unsigned char uint_fast8_t;
typedef unsigned int  uint_fast16_t;
typedef unsigned int  uint_fast32_t;
typedef uint64_t      uint_fast64_t;
#endif

#ifndef INT_BIT
#    if INT_MAX != 2147483647
#        define INT_BIT 64
#    else
#        define INT_BIT 32
#    endif
#endif

#if defined(CONFIG_OS2) || defined(CONFIG_SUNOS)
static inline float floorf(float f) { 
    return floor(f); 
}
#endif

#ifdef CONFIG_WIN32

/* windows */

//#    if !defined(__MINGW32__) && !defined(__CYGWIN__) //Picard
#    if defined(_MSC_VER)
#        define int64_t_C(c)     (c ## i64)
#        define uint64_t_C(c)    (c ## i64)

#    ifdef HAVE_AV_CONFIG_H
#            define inline __inline
#    endif

#    else
#        define int64_t_C(c)     (c ## LL)
#        define uint64_t_C(c)    (c ## ULL)
#    endif /* __MINGW32__ */

#    ifdef HAVE_AV_CONFIG_H
#        ifdef _DEBUG
#            define DEBUG
#        endif

#        define snprintf _snprintf
#        define vsnprintf _vsnprintf
#    endif

/* CONFIG_WIN32 end */
#elif defined (CONFIG_OS2)
/* OS/2 EMX */

#ifndef int64_t_C
#define int64_t_C(c)     (c ## LL)
#define uint64_t_C(c)    (c ## ULL)
#endif

#ifdef HAVE_AV_CONFIG_H

#ifdef USE_FASTMEMCPY
#include "fastmemcpy.h"
#endif

#include <float.h>

#endif /* HAVE_AV_CONFIG_H */

/* CONFIG_OS2 end */
#else

/* unix */

#ifndef int64_t_C
#define int64_t_C(c)     (c ## LL)
#define uint64_t_C(c)    (c ## ULL)
#endif

#ifdef HAVE_AV_CONFIG_H

#        ifdef USE_FASTMEMCPY
#            include "fastmemcpy.h"
#        endif
#    endif /* HAVE_AV_CONFIG_H */

#endif /* !CONFIG_WIN32 && !CONFIG_OS2 */

#ifdef HAVE_AV_CONFIG_H

#    include "bswap.h"

// Use rip-relative addressing if compiling PIC code on x86-64.
#    if defined(__MINGW32__) || defined(__CYGWIN__) || \
        defined(__OS2__) || (defined (__OpenBSD__) && !defined(__ELF__))
#        if defined(ARCH_X86_64) && defined(PIC)
#            define MANGLE(a) "_" #a"(%%rip)"
#        else
#            define MANGLE(a) "_" #a
#        endif
#    else
#        if defined(ARCH_X86_64) && defined(PIC)
#            define MANGLE(a) #a"(%%rip)"
#        else
#            define MANGLE(a) #a
#        endif
#    endif

/* debug stuff */

#    ifndef DEBUG
//#        define NDEBUG //Picard
#    endif
//#    include <assert.h> //Picard

/* dprintf macros */
#    if defined(CONFIG_WIN32) && !defined(__MINGW32__) && !defined(__CYGWIN__)

inline void dprintf(const char* fmt,...) {}

#    else

#        ifdef DEBUG
#            define dprintf(fmt,...) av_log(NULL, AV_LOG_DEBUG, fmt, __VA_ARGS__)
#        else
#            define dprintf(fmt,...)
#        endif

#    endif /* !CONFIG_WIN32 */

#    define av_abort()      do { av_log(NULL, AV_LOG_ERROR, "Abort at %s:%d\n", __FILE__, __LINE__); abort(); } while (0)

//rounded divison & shift
#define RSHIFT(a,b) ((a) > 0 ? ((a) + ((1<<(b))>>1))>>(b) : ((a) + ((1<<(b))>>1)-1)>>(b))
/* assume b>0 */
#define ROUNDED_DIV(a,b) (((a)>0 ? (a) + ((b)>>1) : (a) - ((b)>>1))/(b))
#define ABS(a) ((a) >= 0 ? (a) : (-(a)))

#define FFMAX(a,b) ((a) > (b) ? (a) : (b))
#define FFMIN(a,b) ((a) > (b) ? (b) : (a))

extern const uint32_t inverse[256];

#if defined(ARCH_X86) || defined(ARCH_X86_64)
#    define FASTDIV(a,b) \
    ({\
        int ret,dmy;\
        asm volatile(\
            "mull %3"\
            :"=d"(ret),"=a"(dmy)\
            :"1"(a),"g"(inverse[b])\
            );\
        ret;\
    })
#elif defined(CONFIG_FASTDIV)
#    define FASTDIV(a,b)   ((uint32_t)((((uint64_t)a)*inverse[b])>>32))
#else
#    define FASTDIV(a,b)   ((a)/(b))
#endif
 
/* define it to include statistics code (useful only for optimizing
   codec efficiency */
//#define STATS

#ifdef STATS

enum {
    ST_UNKNOWN,
    ST_DC,
    ST_INTRA_AC,
    ST_INTER_AC,
    ST_INTRA_MB,
    ST_INTER_MB,
    ST_MV,
    ST_NB,
};

extern int st_current_index;
extern unsigned int st_bit_counts[ST_NB];
extern unsigned int st_out_bit_counts[ST_NB];

void print_stats(void);
#endif

/* misc math functions */
extern const uint8_t ff_log2_tab[256];

static inline int av_log2(unsigned int v)
{
    int n;

    n = 0;
    if (v & 0xffff0000) {
        v >>= 16;
        n += 16;
    }
    if (v & 0xff00) {
        v >>= 8;
        n += 8;
    }
    n += ff_log2_tab[v];

    return n;
}

static inline int av_log2_16bit(unsigned int v)
{
    int n;

    n = 0;
    if (v & 0xff00) {
        v >>= 8;
        n += 8;
    }
    n += ff_log2_tab[v];

    return n;
}

/* median of 3 */
static inline int mid_pred(int a, int b, int c)
{
#if 0
    int t= (a-b)&((a-b)>>31);
    a-=t;
    b+=t;
    b-= (b-c)&((b-c)>>31);
    b+= (a-b)&((a-b)>>31);

    return b;
#else
    if(a>b){
        if(c>b){
            if(c>a) b=a;
            else    b=c;
        }
    }else{
        if(b>c){
            if(c>a) b=c;
            else    b=a;
        }
    }
    return b;
#endif
}

static inline int clip(int a, int amin, int amax)
{
    if (a < amin)
        return amin;
    else if (a > amax)
        return amax;
    else
        return a;
}

static inline int clip_uint8(int a)
{
    if (a&(~255)) return (-a)>>31;
    else          return a;
}

/* math */
extern const uint8_t ff_sqrt_tab[128];

int64_t ff_gcd(int64_t a, int64_t b);

static inline int ff_sqrt(int a)
{
    int ret=0;
    int s;
    int ret_sq=0;
    
    if(a<128) return ff_sqrt_tab[a];
    
    for(s=15; s>=0; s--){
        int b= ret_sq + (1<<(s*2)) + (ret<<s)*2;
        if(b<=a){
            ret_sq=b;
            ret+= 1<<s;
        }
    }
    return ret;
}

/**
 * converts fourcc string to int
 */
static inline int ff_get_fourcc(const char *s){
    assert( strlen(s)==4 );

    return (s[0]) + (s[1]<<8) + (s[2]<<16) + (s[3]<<24);
}

#define MKTAG(a,b,c,d) (a | (b << 8) | (c << 16) | (d << 24))
#define MKBETAG(a,b,c,d) (d | (c << 8) | (b << 16) | (a << 24))


#if defined(ARCH_X86) || defined(ARCH_X86_64)
#define MASK_ABS(mask, level)\
            asm volatile(\
		"cdq			\n\t"\
		"xorl %1, %0		\n\t"\
		"subl %1, %0		\n\t"\
		: "+a" (level), "=&d" (mask)\
	    );
#else
#define MASK_ABS(mask, level)\
            mask= level>>31;\
            level= (level^mask)-mask;
#endif


#if __CPU__ >= 686 && !defined(RUNTIME_CPUDETECT)
#define COPY3_IF_LT(x,y,a,b,c,d)\
asm volatile (\
    "cmpl %0, %3	\n\t"\
    "cmovl %3, %0	\n\t"\
    "cmovl %4, %1	\n\t"\
    "cmovl %5, %2	\n\t"\
    : "+r" (x), "+r" (a), "+r" (c)\
    : "r" (y), "r" (b), "r" (d)\
);
#else
#define COPY3_IF_LT(x,y,a,b,c,d)\
if((y)<(x)){\
     (x)=(y);\
     (a)=(b);\
     (c)=(d);\
}
#endif

#if defined(ARCH_X86) || defined(ARCH_X86_64) || defined(ARCH_POWERPC)
#if defined(ARCH_X86_64)
static inline uint64_t read_time(void)
{
	uint64_t a, d;
	asm volatile(	"rdtsc\n\t"
		: "=a" (a), "=d" (d)
	);
	return (d << 32) | (a & 0xffffffff);
}
#elif defined(ARCH_X86)
static inline long long read_time(void)
{
	long long l;
	asm volatile(	"rdtsc\n\t"
		: "=A" (l)
	);
	return l;
}
#else //FIXME check ppc64
static inline uint64_t read_time(void)
{
    uint32_t tbu, tbl, temp;

     /* from section 2.2.1 of the 32-bit PowerPC PEM */
     __asm__ __volatile__(
         "1:\n"
         "mftbu  %2\n"
         "mftb   %0\n"
         "mftbu  %1\n"
         "cmpw   %2,%1\n"
         "bne    1b\n"
     : "=r"(tbl), "=r"(tbu), "=r"(temp)
     :
     : "cc");

     return (((uint64_t)tbu)<<32) | (uint64_t)tbl;
}
#endif

#define START_TIMER \
uint64_t tend;\
uint64_t tstart= read_time();\

#define STOP_TIMER(id) \
tend= read_time();\
{\
  static uint64_t tsum=0;\
  static int tcount=0;\
  static int tskip_count=0;\
  if(tcount<2 || tend - tstart < 8*tsum/tcount){\
      tsum+= tend - tstart;\
      tcount++;\
  }else\
      tskip_count++;\
  if(256*256*256*64%(tcount+tskip_count)==0){\
      av_log(NULL, AV_LOG_DEBUG, "%Ld dezicycles in %s, %d runs, %d skips\n", tsum*10/tcount, id, tcount, tskip_count);\
  }\
}
#else
#define START_TIMER 
#define STOP_TIMER(id) {}
#endif

/* avoid usage of various functions */
#define malloc please_use_av_malloc
#define free please_use_av_free
#define realloc please_use_av_realloc
#define time time_is_forbidden_due_to_security_issues
#define rand rand_is_forbidden_due_to_state_trashing
#define srand srand_is_forbidden_due_to_state_trashing
#define sprintf sprintf_is_forbidden_due_to_security_issues_use_snprintf
#define strcat strcat_is_forbidden_due_to_security_issues_use_pstrcat
#if !(defined(LIBAVFORMAT_BUILD) || defined(_FRAMEHOOK_H))
#define printf please_use_av_log
#define fprintf please_use_av_log
#endif

#define CHECKED_ALLOCZ(p, size)\
{\
    p= av_mallocz(size);\
    if(p==NULL && (size)!=0){\
        perror("malloc");\
        goto fail;\
    }\
}

#ifndef HAVE_LRINTF
/* XXX: add ISOC specific test to avoid specific BSD testing. */
/* better than nothing implementation. */
/* btw, rintf() is existing on fbsd too -- alex */
static av_always_inline long int lrintf(float x)
{
#ifdef CONFIG_WIN32
#  ifdef ARCH_X86
    int32_t i;
    asm volatile(
        "fistpl %0\n\t"
        : "=m" (i) : "t" (x) : "st"
    );
    return i;
#  else
    /* XXX: incorrect, but make it compile */
    return (int)(x + (x < 0 ? -0.5 : 0.5));
#  endif /* ARCH_X86 */
#else
    return (int)(x); //Picard
#endif /* CONFIG_WIN32 */
}
#else
#ifndef _ISOC9X_SOURCE
#define _ISOC9X_SOURCE
#endif
#include <math.h>
#endif /* HAVE_LRINTF */

#endif /* HAVE_AV_CONFIG_H */

#if defined(TCPMP) && defined(_WIN32) && !defined(_MSC_VER) //Picard
#define toupper(a) (((a)>='a' && (a)<='z')?(a)+'A'-'a':(a))
#endif

// thinkingl 

#define FFSWAP(type,a,b) do{type SWAP_tmp= b; b= a; a= SWAP_tmp;}while(0)

#define FF_ARRAY_ELEMS(a) (sizeof(a) / sizeof((a)[0]))

#endif /* COMMON_H */
