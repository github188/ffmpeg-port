/**
 * 
 *	ÄÚ´æ¹ÜÀí.
 * 
 */

/**
 * @file mem.h
 * Memory handling functions.
 */


#include "mcudefine.h"



#ifndef MU_MEM_H
#define MU_MEM_H


#ifdef __cplusplus
extern "C"{
#endif

#ifdef __ICC
    #define DECLARE_ALIGNED(n,t,v)      t v __attribute__ ((aligned (n)))
    #define DECLARE_ASM_CONST(n,t,v)    const t __attribute__ ((aligned (n))) v
#elif defined(__GNUC__)
    #define DECLARE_ALIGNED(n,t,v)      t v __attribute__ ((aligned (n)))
    #define DECLARE_ASM_CONST(n,t,v)    static const t v attribute_used __attribute__ ((aligned (n)))
#elif defined(_MSC_VER)
    #define DECLARE_ALIGNED(n,t,v)      __declspec(align(n)) t v
    #define DECLARE_ASM_CONST(n,t,v)    __declspec(align(n)) static const t v
#elif defined(HAVE_INLINE_ASM)
    #error The asm code needs alignment, but we do not know how to do it for this compiler.
#else
    #define DECLARE_ALIGNED(n,t,v)      t v
    #define DECLARE_ASM_CONST(n,t,v)    static const t v
#endif

#if defined(__GNUC__) && (__GNUC__ > 3 || __GNUC__ == 3 && __GNUC_MINOR__ > 0)
    #define mu_malloc_attrib __attribute__((__malloc__))
#else
    #define mu_malloc_attrib
#endif

#if defined(__GNUC__) && (__GNUC__ > 4 || __GNUC__ == 4 && __GNUC_MINOR__ > 2)
    #define av_alloc_size(n) __attribute__((alloc_size(n)))
#else
    #define av_alloc_size(n)
#endif

/**
 * Allocate a block of \p size bytes with alignment suitable for all
 * memory accesses (including vectors if available on the CPU).
 * @param size Size in bytes for the memory block to be allocated.
 * @return Pointer to the allocated block, NULL if it cannot allocate
 * it.
 * @see av_mallocz()
 */
MU_DECLSPEC void * MU_CALL mu_malloc( mu_uint32 size) mu_malloc_attrib av_alloc_size(1);

/**
 * Allocate or reallocate a block of memory.
 * If \p ptr is NULL and \p size > 0, allocate a new block. If \p
 * size is zero, free the memory block pointed by \p ptr.
 * @param size Size in bytes for the memory block to be allocated or
 * reallocated.
 * @param ptr Pointer to a memory block already allocated with
 * mu_malloc(z)() or mu_realloc() or NULL.
 * @return Pointer to a newly reallocated block or NULL if it cannot
 * reallocate or the function is used to free the memory block.
 * @see av_fast_realloc()
 */
MU_DECLSPEC void * MU_CALL mu_realloc(void *ptr, mu_uint32 size) av_alloc_size(2);

/**
 * Free a memory block which has been allocated with mu_malloc(z)() or
 * mu_realloc().
 * @param ptr Pointer to the memory block which should be freed.
 * @note ptr = NULL is explicitly allowed.
 * @note It is recommended that you use av_freep() instead.
 * @see av_freep()
 */
MU_DECLSPEC void  MU_CALL mu_free(void *ptr);

/** 
*/
MU_DECLSPEC void * MU_CALL mu_fast_realloc(void *ptr, mu_uint32 *size, mu_uint32 min_size);

/**
 * Allocate a block of \p size bytes with alignment suitable for all
 * memory accesses (including vectors if available on the CPU) and
 * set to zeroes all the bytes of the block.
 * @param size Size in bytes for the memory block to be allocated.
 * @return Pointer to the allocated block, NULL if it cannot allocate
 * it.
 * @see mu_malloc()
 */
MU_DECLSPEC void * MU_CALL mu_mallocz( mu_uint32 size) mu_malloc_attrib av_alloc_size(1);

/**
 * Duplicate the string \p s.
 * @param s String to be duplicated.
 * @return Pointer to a newly allocated string containing a
 * copy of \p s or NULL if it cannot be allocated.
 */
MU_DECLSPEC char * MU_CALL mu_strdup(const char *s) mu_malloc_attrib;

/**
 * Free a memory block which has been allocated with mu_malloc(z)() or
 * mu_realloc() and set to NULL the pointer to it.
 * @param ptr Pointer to the pointer to the memory block which should
 * be freed.
 * @see mu_free()
 */
MU_DECLSPEC void  MU_CALL mu_freep(void *ptr);

#ifdef __cplusplus
}
#endif

#endif /* MU_MEM_H */
