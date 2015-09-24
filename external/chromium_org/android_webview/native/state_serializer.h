// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef ANDROID_WEBVIEW_NATIVE_STATE_SERIALIZER_H_
#define ANDROID_WEBVIEW_NATIVE_STATE_SERIALIZER_H_

#include "base/compiler_specific.h"

class Pickle;
class PickleIterator;

namespace content {

class NavigationEntry;
class WebContents;

}  

namespace android_webview {


bool WriteToPickle(const content::WebContents& web_contents,
                   Pickle* pickle) WARN_UNUSED_RESULT;

bool RestoreFromPickle(PickleIterator* iterator,
                       content::WebContents* web_contents) WARN_UNUSED_RESULT;


namespace internal {
bool WriteHeaderToPickle(Pickle* pickle) WARN_UNUSED_RESULT;
bool RestoreHeaderFromPickle(PickleIterator* iterator) WARN_UNUSED_RESULT;
bool WriteNavigationEntryToPickle(const content::NavigationEntry& entry,
                                  Pickle* pickle) WARN_UNUSED_RESULT;
bool RestoreNavigationEntryFromPickle(
    PickleIterator* iterator,
    content::NavigationEntry* entry) WARN_UNUSED_RESULT;

}  

}  

#endif  
