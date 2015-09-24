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

#ifndef prlink_h___
#define prlink_h___

#include "prtypes.h"

PR_BEGIN_EXTERN_C

typedef struct PRLibrary PRLibrary;

typedef struct PRStaticLinkTable {
    const char *name;
    void (*fp)();
} PRStaticLinkTable;

NSPR_API(PRStatus) PR_SetLibraryPath(const char *path);

NSPR_API(char*) PR_GetLibraryPath(void);

NSPR_API(char*) PR_GetLibraryName(const char *dir, const char *lib);

NSPR_API(void) PR_FreeLibraryName(char *mem);

NSPR_API(PRLibrary*) PR_LoadLibrary(const char *name);


typedef enum PRLibSpecType {
    PR_LibSpec_Pathname,
    PR_LibSpec_MacNamedFragment,
    PR_LibSpec_MacIndexedFragment
} PRLibSpecType;

struct FSSpec; 

typedef struct PRLibSpec {
    PRLibSpecType type;
    union {
        
        const char *pathname;

        
        struct {
            const struct FSSpec *fsspec;
            const char *name;
        } mac_named_fragment;

        
        struct {
            const struct FSSpec *fsspec;
            PRUint32 index;
        } mac_indexed_fragment;
    } value;
} PRLibSpec;


#define PR_LD_LAZY   0x1  
#define PR_LD_NOW    0x2  
#define PR_LD_GLOBAL 0x4  
#define PR_LD_LOCAL  0x8  


NSPR_API(PRLibrary *)
PR_LoadLibraryWithFlags(
    PRLibSpec libSpec,    
    PRIntn flags          
);

NSPR_API(PRStatus) PR_UnloadLibrary(PRLibrary *lib);

NSPR_API(void*) PR_FindSymbol(PRLibrary *lib, const char *name);

typedef void (*PRFuncPtr)();
NSPR_API(PRFuncPtr) PR_FindFunctionSymbol(PRLibrary *lib, const char *name);

NSPR_API(void*) PR_FindSymbolAndLibrary(const char *name,
						      PRLibrary* *lib);

NSPR_API(PRFuncPtr) PR_FindFunctionSymbolAndLibrary(const char *name,
						      PRLibrary* *lib);

NSPR_API(PRLibrary*) PR_LoadStaticLibrary(
    const char *name, const PRStaticLinkTable *table);

NSPR_API(char *) PR_GetLibraryFilePathname(const char *name, PRFuncPtr addr);

PR_END_EXTERN_C

#endif 
