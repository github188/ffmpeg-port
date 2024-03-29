/*
 * 
 */
 
/**
 * @file mem.c
 * default memory allocator for libavcodec.
 */
 
#include "mem.h"

/* here we can use OS dependant allocation functions */
#undef malloc
#undef free
#undef realloc

#ifdef TCPMP
#if defined(TARGET_WIN32) || defined(TARGET_WINCE)
void* malloc_win32(size_t);
void* realloc_win32(void*,size_t);
void free_win32(void*);
#define malloc(n) malloc_win32(n)
#define realloc(p,n) realloc_win32(p,n)
#define free(p) free_win32(p)
#endif
#endif

#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif

/* you can redefine mu_malloc and mu_free in your project to use your
   memory allocator. You do not need to suppress this file because the
   linker will do it automatically */

/** 
 * Memory allocation of size byte with alignment suitable for all
 * memory accesses (including vectors if available on the
 * CPU). mu_malloc(0) must return a non NULL pointer.
 */
void *mu_malloc( mu_uint32 size)
{
    void *ptr;
#ifdef MEMALIGN_HACK
    char diff;
#endif

    /* lets disallow possible ambiguous cases */
    if(size > INT_MAX)
        return NULL;
    
#ifdef MEMALIGN_HACK
    ptr = malloc(size+16+1);
	if ( NULL == ptr )
	{
		return ptr;
	}
	
    diff= ((-(int)ptr - 1)&15) + 1;
    ptr = (char*)ptr + diff;
    ((char*)ptr)[-1]= diff;
#elif defined (HAVE_MEMALIGN) 
    ptr = memalign(16,size);
    /* Why 64? 
       Indeed, we should align it:
         on 4 for 386
         on 16 for 486
	 on 32 for 586, PPro - k6-III
	 on 64 for K7 (maybe for P3 too).
       Because L1 and L2 caches are aligned on those values.
       But I don't want to code such logic here!
     */
     /* Why 16?
        because some cpus need alignment, for example SSE2 on P4, & most RISC cpus
        it will just trigger an exception and the unaligned load will be done in the
        exception handler or it will just segfault (SSE2 on P4)
        Why not larger? because i didnt see a difference in benchmarks ...
     */
     /* benchmarks with p3
        memalign(64)+1		3071,3051,3032
        memalign(64)+2		3051,3032,3041
        memalign(64)+4		2911,2896,2915
        memalign(64)+8		2545,2554,2550
        memalign(64)+16		2543,2572,2563
        memalign(64)+32		2546,2545,2571
        memalign(64)+64		2570,2533,2558
        
        btw, malloc seems to do 8 byte alignment by default here
     */
#else
    ptr = malloc(size);
#endif
    return ptr;
}

/**
 * mu_realloc semantics (same as glibc): if ptr is NULL and size > 0,
 * identical to malloc(size). If size is zero, it is identical to
 * free(ptr) and NULL is returned.  
 */
void *mu_realloc(void *ptr, mu_uint32 size)
{
#ifdef MEMALIGN_HACK
    int diff;
#endif

    /* lets disallow possible ambiguous cases */
    if(size > INT_MAX)
        return NULL;

#ifdef MEMALIGN_HACK
    //FIXME this isnt aligned correctly though it probably isnt needed
    if(!ptr) return mu_malloc(size);
    diff= ((char*)ptr)[-1];
    return (char*)realloc( (char*)ptr - diff, size + diff) + diff;
#else
    return realloc(ptr, size);
#endif
}

/* NOTE: ptr = NULL is explicetly allowed */
void mu_free(void *ptr)
{
    /* XXX: this test should not be needed on most libcs */
    if (ptr)
#ifdef MEMALIGN_HACK
        free( (char*)ptr - ((char*)ptr)[-1]);
#else
        free(ptr);
#endif
}

/**
* realloc which does nothing if the block is large enough
*/
void *mu_fast_realloc(void *ptr, mu_uint32 *size, mu_uint32 min_size)
{
	if(min_size < *size) 
		return ptr;

	*size=  MU_MAX(17*min_size/16 + 32, min_size);

	return mu_realloc(ptr, *size);
}


void *mu_mallocz( mu_uint32 size)
{
	void *ptr;

	ptr = mu_malloc(size);
	if (!ptr)
		return NULL;
	memset(ptr, 0, size);
	return ptr;
}

char *mu_strdup(const char *s)
{
	char *ptr;
	int len;
	len = strlen(s) + 1;
	ptr =(char*) mu_malloc(len);
	if (!ptr)
		return NULL;
	memcpy(ptr, s, len);
	return ptr;
}

/**
* Frees memory and sets the pointer to NULL.
* @param arg pointer to the pointer which should be freed
*/
void mu_freep(void *arg)
{	
	void **ptr= (void**)arg;
	if ( NULL == ptr || NULL == *ptr )
	{
		return;
	}
	mu_free(*ptr);
	*ptr = NULL;
}
