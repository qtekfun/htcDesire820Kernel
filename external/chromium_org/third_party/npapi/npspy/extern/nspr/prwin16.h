/* 
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 * 
 * The Original Code is the Netscape Portable Runtime (NSPR).
 * 
 * The Initial Developer of the Original Code is Netscape
 * Communications Corporation.  Portions created by Netscape are 
 * Copyright (C) 1998-2000 Netscape Communications Corporation.  All
 * Rights Reserved.
 * 
 * Contributor(s):
 * 
 * Alternatively, the contents of this file may be used under the
 * terms of the GNU General Public License Version 2 or later (the
 * "GPL"), in which case the provisions of the GPL are applicable 
 * instead of those above.  If you wish to allow use of your 
 * version of this file only under the terms of the GPL and not to
 * allow others to use your version of this file under the MPL,
 * indicate your decision by deleting the provisions above and
 * replace them with the notice and other provisions required by
 * the GPL.  If you do not delete the provisions above, a recipient
 * may use your version of this file under either the MPL or the
 * GPL.
 */

#ifndef prwin16_h___
#define prwin16_h___

#if (defined(XP_PC) && !defined(_WIN32) && !defined(XP_OS2) && defined(MOZILLA_CLIENT)) || defined(WIN16)
#include <stdio.h>

PR_BEGIN_EXTERN_C
typedef PRInt32 (PR_CALLBACK *PRStdinRead)( void *buf, PRInt32 amount);
typedef PRInt32 (PR_CALLBACK *PRStdoutWrite)( void *buf, PRInt32 amount);
typedef PRInt32 (PR_CALLBACK *PRStderrWrite)( void *buf, PRInt32 amount);

NSPR_API(PRStatus)
PR_MD_RegisterW16StdioCallbacks( 
    PRStdinRead inReadf,            
    PRStdoutWrite outWritef,        
    PRStderrWrite errWritef         
    );

NSPR_API(PRInt32)
_PL_W16StdioWrite( void *buf, PRInt32 amount );

NSPR_API(PRInt32)
_PL_W16StdioRead( void *buf, PRInt32 amount );

#define PR_STDIO_INIT() PR_MD_RegisterW16StdioCallbacks( \
    _PL_W16StdioRead, _PL_W16StdioWrite, _PL_W16StdioWrite ); \
    PR_INIT_CALLBACKS();

struct PRMethodCallbackStr {
    int     (PR_CALLBACK *auxOutput)(const char *outputString);
    size_t  (PR_CALLBACK *strftime)(char *s, size_t len, const char *fmt, const struct tm *p);
    void *  (PR_CALLBACK *malloc)( size_t size );
    void *  (PR_CALLBACK *calloc)(size_t n, size_t size );
    void *  (PR_CALLBACK *realloc)( void* old_blk, size_t size );
    void    (PR_CALLBACK *free)( void *ptr );
    void *  (PR_CALLBACK *getenv)( const char *name);
    int     (PR_CALLBACK *putenv)( const char *assoc);
};

NSPR_API(void) PR_MDRegisterCallbacks(struct PRMethodCallbackStr *);

int PR_CALLBACK _PL_W16CallBackPuts( const char *outputString );
size_t PR_CALLBACK _PL_W16CallBackStrftime( 
    char *s, 
    size_t len, 
    const char *fmt,
    const struct tm *p );
void * PR_CALLBACK _PL_W16CallBackMalloc( size_t size );
void * PR_CALLBACK _PL_W16CallBackCalloc( size_t n, size_t size );
void * PR_CALLBACK _PL_W16CallBackRealloc( 
    void *old_blk, 
    size_t size );
void   PR_CALLBACK _PL_W16CallBackFree( void *ptr );
void * PR_CALLBACK _PL_W16CallBackGetenv( const char *name );
int PR_CALLBACK _PL_W16CallBackPutenv( const char *assoc );

NSPR_API(int)     PR_MD_printf(const char *, ...);
NSPR_API(void)    PR_MD_exit(int);
NSPR_API(size_t)  PR_MD_strftime(char *, size_t, const char *, const struct tm *); 
NSPR_API(int)     PR_MD_sscanf(const char *, const char *, ...);
NSPR_API(void*)   PR_MD_malloc( size_t size );
NSPR_API(void*)   PR_MD_calloc( size_t n, size_t size );
NSPR_API(void*)   PR_MD_realloc( void* old_blk, size_t size );
NSPR_API(void)    PR_MD_free( void *ptr );
NSPR_API(char*)   PR_MD_getenv( const char *name );
NSPR_API(int)     PR_MD_putenv( const char *assoc );
NSPR_API(int)     PR_MD_fprintf(FILE *fPtr, const char *fmt, ...);

#define PR_INIT_CALLBACKS()                         \
    {                                               \
        static struct PRMethodCallbackStr cbf = {   \
            _PL_W16CallBackPuts,                    \
            _PL_W16CallBackStrftime,                \
            _PL_W16CallBackMalloc,                  \
            _PL_W16CallBackCalloc,                  \
            _PL_W16CallBackRealloc,                 \
            _PL_W16CallBackFree,                    \
            _PL_W16CallBackGetenv,                  \
            _PL_W16CallBackPutenv,                  \
        };                                          \
        PR_MDRegisterCallbacks( &cbf );             \
    }


NSPR_API(void *) PR_W16GetExceptionContext(void);
NSPR_API(void) PR_W16SetExceptionContext(void *context);

PR_END_EXTERN_C
#else
#define PR_STDIO_INIT()
#endif 

#endif 








