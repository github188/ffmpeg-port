#ifndef DEFINE_H
#define DEFINE_H
#define ENABLE_GRAY 0

#ifndef INT_MAX
#define INT_MAX (1<<30)
#endif

#ifndef INT64_MAX
#define INT64_MAX (1<<30)
#endif

#ifndef INT_MIN
#define INT_MIN (-1<<30)
#endif
#define ENABLE_THREADS 0

#define restrict 
#define ENABLE_SMALL 0
#define INT64_C __int64

#ifndef snprintf
#define snprintf sprintf
#endif





#define ENABLE_H264_DECODER 1
#endif