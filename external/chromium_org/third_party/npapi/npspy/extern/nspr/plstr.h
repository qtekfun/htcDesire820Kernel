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
 *   Roland Mainz <roland mainz@informatik.med.uni-giessen.de>
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

#ifndef _plstr_h
#define _plstr_h

/*
 * plstr.h
 *
 * This header file exports the API to the NSPR portable library or string-
 * handling functions.  
 * 
 * This API was not designed as an "optimal" or "ideal" string library; it 
 * was based on the good ol' unix string.3 functions, and was written to
 *
 *  1) replace the libc functions, for cross-platform consistancy, 
 *  2) complete the API on platforms lacking common functions (e.g., 
 *     strcase*), and
 *  3) to implement some obvious "closure" functions that I've seen
 *     people hacking around in our code.
 *
 * Point number three largely means that most functions have an "strn"
 * limited-length version, and all comparison routines have a non-case-
 * sensitive version available.
 */

#include "prtypes.h"

PR_BEGIN_EXTERN_C

PR_EXTERN(PRUint32)
PL_strlen(const char *str);


PR_EXTERN(PRUint32)
PL_strnlen(const char *str, PRUint32 max);


PR_EXTERN(char *)
PL_strcpy(char *dest, const char *src);


PR_EXTERN(char *)
PL_strncpy(char *dest, const char *src, PRUint32 max);


PR_EXTERN(char *)
PL_strncpyz(char *dest, const char *src, PRUint32 max);


PR_EXTERN(char *)
PL_strdup(const char *s);


PR_EXTERN(void)
PL_strfree(char *s);


PR_EXTERN(char *)
PL_strndup(const char *s, PRUint32 max);


PR_EXTERN(char *)
PL_strcat(char *dst, const char *src);


PR_EXTERN(char *)
PL_strncat(char *dst, const char *src, PRUint32 max);


PR_EXTERN(char *)
PL_strcatn(char *dst, PRUint32 max, const char *src);


PR_EXTERN(PRIntn)
PL_strcmp(const char *a, const char *b);


PR_EXTERN(PRIntn)
PL_strncmp(const char *a, const char *b, PRUint32 max);


PR_EXTERN(PRIntn)
PL_strcasecmp(const char *a, const char *b);


PR_EXTERN(PRIntn)
PL_strncasecmp(const char *a, const char *b, PRUint32 max);


PR_EXTERN(char *)
PL_strchr(const char *s, char c);


PR_EXTERN(char *)
PL_strrchr(const char *s, char c);


PR_EXTERN(char *)
PL_strnchr(const char *s, char c, PRUint32 n);


PR_EXTERN(char *)
PL_strnrchr(const char *s, char c, PRUint32 n);



PR_EXTERN(char *)
PL_strpbrk(const char *s, const char *list);


PR_EXTERN(char *)
PL_strprbrk(const char *s, const char *list);


PR_EXTERN(char *)
PL_strnpbrk(const char *s, const char *list, PRUint32 n);


PR_EXTERN(char *)
PL_strnprbrk(const char *s, const char *list, PRUint32 n);


PR_EXTERN(char *)
PL_strstr(const char *big, const char *little);


PR_EXTERN(char *)
PL_strrstr(const char *big, const char *little);


PR_EXTERN(char *)
PL_strnstr(const char *big, const char *little, PRUint32 n);


PR_EXTERN(char *)
PL_strnrstr(const char *big, const char *little, PRUint32 max);


PR_EXTERN(char *)
PL_strcasestr(const char *big, const char *little);


PR_EXTERN(char *)
PL_strcaserstr(const char *big, const char *little);


PR_EXTERN(char *)
PL_strncasestr(const char *big, const char *little, PRUint32 max);


PR_EXTERN(char *)
PL_strncaserstr(const char *big, const char *little, PRUint32 max);


PR_EXTERN(char *)
PL_strtok_r(char *s1, const char *s2, char **lasts);


PR_END_EXTERN_C

#endif 
