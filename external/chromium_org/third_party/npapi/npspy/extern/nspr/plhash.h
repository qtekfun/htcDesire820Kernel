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

#ifndef plhash_h___
#define plhash_h___
#include <stdio.h>
#include "prtypes.h"

PR_BEGIN_EXTERN_C

typedef struct PLHashEntry  PLHashEntry;
typedef struct PLHashTable  PLHashTable;
typedef PRUint32 PLHashNumber;
#define PL_HASH_BITS 32  
typedef PLHashNumber (PR_CALLBACK *PLHashFunction)(const void *key);
typedef PRIntn (PR_CALLBACK *PLHashComparator)(const void *v1, const void *v2);

#if defined(XP_OS2_VACPP) && defined(VACPP_FLIP) 
PR_END_EXTERN_C                                  
#endif
typedef PRIntn (PR_CALLBACK *PLHashEnumerator)(PLHashEntry *he, PRIntn i, void *arg);

#if defined(XP_OS2_VACPP) && defined(VACPP_FLIP)
PR_BEGIN_EXTERN_C
#endif

#define HT_ENUMERATE_NEXT       0       
#define HT_ENUMERATE_STOP       1       
#define HT_ENUMERATE_REMOVE     2       
#define HT_ENUMERATE_UNHASH     4       

typedef struct PLHashAllocOps {
    void *              (PR_CALLBACK *allocTable)(void *pool, PRSize size);
    void                (PR_CALLBACK *freeTable)(void *pool, void *item);
    PLHashEntry *       (PR_CALLBACK *allocEntry)(void *pool, const void *key);
    void                (PR_CALLBACK *freeEntry)(void *pool, PLHashEntry *he, PRUintn flag);
} PLHashAllocOps;

#define HT_FREE_VALUE   0               
#define HT_FREE_ENTRY   1               

struct PLHashEntry {
    PLHashEntry         *next;          
    PLHashNumber        keyHash;        
    const void          *key;           
    void                *value;         
};

struct PLHashTable {
    PLHashEntry         **buckets;      
    PRUint32              nentries;       
    PRUint32              shift;          
    PLHashFunction      keyHash;        
    PLHashComparator    keyCompare;     
    PLHashComparator    valueCompare;   
    const PLHashAllocOps *allocOps;     
    void                *allocPriv;     
#ifdef HASHMETER
    PRUint32              nlookups;       
    PRUint32              nsteps;         
    PRUint32              ngrows;         
    PRUint32              nshrinks;       
#endif
};

PR_EXTERN(PLHashTable *)
PL_NewHashTable(PRUint32 numBuckets, PLHashFunction keyHash,
                PLHashComparator keyCompare, PLHashComparator valueCompare,
                const PLHashAllocOps *allocOps, void *allocPriv);

PR_EXTERN(void)
PL_HashTableDestroy(PLHashTable *ht);

PR_EXTERN(PLHashEntry *)
PL_HashTableAdd(PLHashTable *ht, const void *key, void *value);

PR_EXTERN(PRBool)
PL_HashTableRemove(PLHashTable *ht, const void *key);

PR_EXTERN(void *)
PL_HashTableLookup(PLHashTable *ht, const void *key);

PR_EXTERN(void *)
PL_HashTableLookupConst(PLHashTable *ht, const void *key);

PR_EXTERN(PRIntn)
PL_HashTableEnumerateEntries(PLHashTable *ht, PLHashEnumerator f, void *arg);

PR_EXTERN(PLHashNumber)
PL_HashString(const void *key);

PR_EXTERN(PRIntn)
PL_CompareStrings(const void *v1, const void *v2);

PR_EXTERN(PRIntn)
PL_CompareValues(const void *v1, const void *v2);

PR_EXTERN(PLHashEntry **)
PL_HashTableRawLookup(PLHashTable *ht, PLHashNumber keyHash, const void *key);

PR_EXTERN(PLHashEntry **)
PL_HashTableRawLookupConst(PLHashTable *ht, PLHashNumber keyHash,
                           const void *key);

PR_EXTERN(PLHashEntry *)
PL_HashTableRawAdd(PLHashTable *ht, PLHashEntry **hep, PLHashNumber keyHash,
                   const void *key, void *value);

PR_EXTERN(void)
PL_HashTableRawRemove(PLHashTable *ht, PLHashEntry **hep, PLHashEntry *he);

PR_EXTERN(PRIntn)
PL_HashTableDump(PLHashTable *ht, PLHashEnumerator dump, FILE *fp);

PR_END_EXTERN_C

#endif 
