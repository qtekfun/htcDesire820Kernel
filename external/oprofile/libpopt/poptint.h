

#ifndef H_POPTINT
#define H_POPTINT

 static inline  void *
_free(  const void * p)
	
{
    if (p != NULL)	free((void *)p);
    return NULL;
}

typedef	unsigned int __pbm_bits;
#define	__PBM_NBITS		(8 * sizeof (__pbm_bits))
#define	__PBM_IX(d)		((d) / __PBM_NBITS)
#define __PBM_MASK(d)		((__pbm_bits) 1 << (((unsigned)(d)) % __PBM_NBITS))
typedef struct {
    __pbm_bits bits[1];
} pbm_set;
#define	__PBM_BITS(set)	((set)->bits)

#define	PBM_ALLOC(d)	calloc(__PBM_IX (d) + 1, sizeof(__pbm_bits))
#define	PBM_FREE(s)	_free(s);
#define PBM_SET(d, s)   (__PBM_BITS (s)[__PBM_IX (d)] |= __PBM_MASK (d))
#define PBM_CLR(d, s)   (__PBM_BITS (s)[__PBM_IX (d)] &= ~__PBM_MASK (d))
#define PBM_ISSET(d, s) ((__PBM_BITS (s)[__PBM_IX (d)] & __PBM_MASK (d)) != 0)

struct optionStackEntry {
    int argc;
 
    const char ** argv;
 
    pbm_set * argb;
    int next;
 
    const char * nextArg;
 
    const char * nextCharArg;
 
    poptItem currAlias;
    int stuffed;
};

struct poptContext_s {
    struct optionStackEntry optionStack[POPT_OPTION_DEPTH];
    struct optionStackEntry * os;
 
    const char ** leftovers;
    int numLeftovers;
    int nextLeftover;
    const struct poptOption * options;
    int restLeftover;
 
    const char * appName;
 
    poptItem aliases;
    int numAliases;
    int flags;
 
    poptItem execs;
    int numExecs;
 
    const char ** finalArgv;
    int finalArgvCount;
    int finalArgvAlloced;
 
    poptItem doExec;
    const char * execPath;
    int execAbsolute;
 
    const char * otherHelp;
    pbm_set * arg_strip;
};

#ifdef HAVE_LIBINTL_H
#include <libintl.h>
#endif

#if defined(HAVE_GETTEXT) && !defined(__LCLINT__)
#define _(foo) gettext(foo)
#else
#define _(foo) foo
#endif

#if defined(HAVE_DCGETTEXT) && !defined(__LCLINT__)
#define D_(dom, str) dgettext(dom, str)
#define POPT_(foo) D_("popt", foo)
#else
#define D_(dom, str) str
#define POPT_(foo) foo
#endif

#define N_(foo) foo

#endif