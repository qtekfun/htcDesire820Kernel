/* Portions are Copyright (C) 2011 Google Inc */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Netscape Portable Runtime (NSPR).
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998-2000
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */


#ifndef BASE_PRTIME_H__
#define BASE_PRTIME_H__

#include "base/base_api.h"
#include "base/third_party/nspr/prtypes.h"

#define PR_ASSERT DCHECK

#define LL_I2L(l, i)    ((l) = (PRInt64)(i))
#define LL_MUL(r, a, b) ((r) = (a) * (b))


#define PR_MSEC_PER_SEC		1000UL
#define PR_USEC_PER_SEC		1000000UL
#define PR_NSEC_PER_SEC		1000000000UL
#define PR_USEC_PER_MSEC	1000UL
#define PR_NSEC_PER_MSEC	1000000UL


typedef PRInt64 PRTime;


typedef struct PRTimeParameters {
    PRInt32 tp_gmt_offset;     
    PRInt32 tp_dst_offset;     
} PRTimeParameters;


typedef struct PRExplodedTime {
    PRInt32 tm_usec;		    
    PRInt32 tm_sec;             
	
    PRInt32 tm_min;             
    PRInt32 tm_hour;            
    PRInt32 tm_mday;            
    PRInt32 tm_month;           
    PRInt16 tm_year;            

    PRInt8 tm_wday;		        
    PRInt16 tm_yday;            

    PRTimeParameters tm_params;  
} PRExplodedTime;


typedef PRTimeParameters (PR_CALLBACK *PRTimeParamFn)(const PRExplodedTime *gmt);


NSPR_API(PRTime)
PR_ImplodeTime(const PRExplodedTime *exploded);


NSPR_API(void) PR_NormalizeTime(
    PRExplodedTime *exploded, PRTimeParamFn params);


NSPR_API(PRTimeParameters) PR_GMTParameters(const PRExplodedTime *gmt);



BASE_API PRStatus PR_ParseTimeString (
	const char *string,
	PRBool default_to_gmt,
	PRTime *result);

#endif  
