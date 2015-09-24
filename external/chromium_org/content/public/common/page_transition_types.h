// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_PAGE_TRANSITION_TYPES_H_
#define CONTENT_PUBLIC_COMMON_PAGE_TRANSITION_TYPES_H_

#include "base/basictypes.h"
#include "content/common/content_export.h"

namespace content {

enum PageTransition {

#define PAGE_TRANSITION(label, value) PAGE_TRANSITION_ ## label = value,
#include "content/public/common/page_transition_types_list.h"
#undef PAGE_TRANSITION

};

CONTENT_EXPORT bool PageTransitionCoreTypeIs(PageTransition lhs,
                                             PageTransition rhs);

CONTENT_EXPORT PageTransition PageTransitionStripQualifier(
    PageTransition type);

bool PageTransitionIsValidType(int32 type);

CONTENT_EXPORT PageTransition PageTransitionFromInt(int32 type);

CONTENT_EXPORT bool PageTransitionIsMainFrame(PageTransition type);

CONTENT_EXPORT bool PageTransitionIsRedirect(PageTransition type);

CONTENT_EXPORT int32 PageTransitionGetQualifier(PageTransition type);

CONTENT_EXPORT bool PageTransitionIsWebTriggerable(PageTransition type);

CONTENT_EXPORT const char* PageTransitionGetCoreTransitionString(
    PageTransition type);

#if defined(CONTENT_IMPLEMENTATION)
class DontUseOperatorEquals;

DontUseOperatorEquals operator==(PageTransition, PageTransition);
DontUseOperatorEquals operator==(PageTransition, int);
DontUseOperatorEquals operator==(int, PageTransition);
#endif  

}  

#endif  
