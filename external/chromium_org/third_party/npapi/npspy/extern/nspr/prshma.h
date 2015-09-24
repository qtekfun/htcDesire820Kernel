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
 * Copyright (C) 1999-2000 Netscape Communications Corporation.  All
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


#ifndef prshma_h___
#define prshma_h___

#include "prtypes.h"
#include "prio.h"
#include "prproces.h"

PR_BEGIN_EXTERN_C

NSPR_API( PRFileMap *)
PR_OpenAnonFileMap(
    const char *dirName,
    PRSize      size, 
    PRFileMapProtect prot
);  

NSPR_API(PRStatus) 
PR_ProcessAttrSetInheritableFileMap( 
    PRProcessAttr   *attr,
    PRFileMap       *fm, 
    const char      *shmname
);

NSPR_API( PRFileMap *)
PR_GetInheritedFileMap( 
    const char *shmname 
);

NSPR_API( PRStatus )
PR_ExportFileMapAsString( 
    PRFileMap *fm,
    PRSize    bufsize,
    char      *buf
);
#define PR_FILEMAP_STRING_BUFSIZE 128

NSPR_API( PRFileMap * )
PR_ImportFileMapFromString( 
    const char *fmstring
);

PR_END_EXTERN_C
#endif 
