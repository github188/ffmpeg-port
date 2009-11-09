/*
 * default memory allocator for libavcodec
 * Copyright (c) 2002 Fabrice Bellard.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
/**
 * @file mem.c
 * default memory allocator for libavcodec.
 */
 
#include "avcodec.h"
#include "dsputil.h"

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

/* you can redefine av_malloc and av_free in your project to use your
   memory allocator. You do not need to suppress this file because the
   linker will do it automatically */

/** 
 * Memory allocation of size byte with alignment suitable for all
 * memory accesses (including vectors if available on the
 * CPU). av_malloc(0) must return a non NULL pointer.
 */
void *av_malloc(unsigned int size)
{
    char *ptr;
#ifdef MEMALIGN_HACK
    int diff;
#endif

    /* lets disallow possible ambiguous cases */
    if(size > INT_MAX)
        return NULL;
    
#ifdef MEMALIGN_HACK
    ptr = malloc(size+16+1);
	if( ptr == NULL )
	{
		return ptr;
	}
    diff= ((-(int)ptr - 1)&15) + 1;
    ptr += diff;
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
 * av_realloc semantics (same as glibc): if ptr is NULL and size > 0,
 * identical to malloc(size). If size is zero, it is identical to
 * free(ptr) and NULL is returned.  
 */
void *av_realloc(void *ptr, unsigned int size)
{
#ifdef MEMALIGN_HACK
    int diff;
#endif

    /* lets disallow possible ambiguous cases */
    if(size > INT_MAX)
        return NULL;

#ifdef MEMALIGN_HACK
    //FIXME this isnt aligned correctly though it probably isnt needed
    if(!ptr) return av_malloc(size);
    diff= ((char*)ptr)[-1];
    return (char*)realloc( (char*)ptr - diff, size + diff ) + diff;
#else
    return realloc(ptr, size);
#endif
}

/* NOTE: ptr = NULL is explicetly allowed */
void av_free(void *ptr)
{
    /* XXX: this test should not be needed on most libcs */
    if (ptr)
#ifdef MEMALIGN_HACK
        free( (char*)ptr - ((char*)ptr)[-1]);
#else
        free(ptr);
#endif
}

void avcodec_default_free_buffers(AVCodecContext *s);

void *av_mallocz(unsigned int size)
{
	void *ptr;

	ptr = av_malloc(size);
	if (!ptr)
		return NULL;
	memset(ptr, 0, size);
	return ptr;
}

char *av_strdup(const char *s)
{
	char *ptr;
	int len;
	len = strlen(s) + 1;
	ptr = av_malloc(len);
	if (!ptr)
		return NULL;
	memcpy(ptr, s, len);
	return ptr;
}

/**
* realloc which does nothing if the block is large enough
*/
void *av_fast_realloc(void *ptr, unsigned int *size, unsigned int min_size)
{
	if(min_size < *size) 
		return ptr;

	*size= FFMAX(17*min_size/16 + 32, min_size);

	return av_realloc(ptr, *size);
}


static unsigned int last_static = 0;
static unsigned int allocated_static = 0;
static void** array_static = NULL;

/**
* allocation of static arrays - do not use for normal allocation.
*/
void *av_mallocz_static(unsigned int size)
{
	void *ptr = av_mallocz(size);

	if(ptr){ 
		array_static =av_fast_realloc(array_static, &allocated_static, sizeof(void*)*(last_static+1));
		if(!array_static)
			return NULL;
		array_static[last_static++] = ptr;
	}

	return ptr;
}

/**
* same as above, but does realloc
*/

void *av_realloc_static(void *ptr, unsigned int size)
{
	int i;
	if(!ptr)
		return av_mallocz_static(size);
	/* Look for the old ptr */
	for(i = 0; i < last_static; i++) {
		if(array_static[i] == ptr) {
			array_static[i] = av_realloc(array_static[i], size);
			return array_static[i];
		}
	}
	return NULL;

}

/**
* free all static arrays and reset pointers to 0.
*/
void av_free_static(void)
{
	while(last_static){
		av_freep(&array_static[--last_static]);
	}
	av_freep(&array_static);
}

/**
* Call av_free_static automatically before it's too late
*/

static void do_free() __attribute__ ((destructor));

static void do_free()
{
	av_free_static();
}

/**
* Frees memory and sets the pointer to NULL.
* @param arg pointer to the pointer which should be freed
*/
void av_freep(void *arg)
{
	void **ptr= (void**)arg;
	av_free(*ptr);
	*ptr = NULL;
}
