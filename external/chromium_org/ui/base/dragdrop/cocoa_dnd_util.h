// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_DRAGDROP_MAC_DND_UTIL_H_
#define UI_BASE_DRAGDROP_MAC_DND_UTIL_H_

#import <Cocoa/Cocoa.h>

#include <vector>

#include "base/strings/string16.h"
#include "ui/base/ui_export.h"

class GURL;

namespace ui {

UI_EXPORT extern NSString* const kChromeDragDummyPboardType;

UI_EXPORT extern NSString* const kChromeDragImageHTMLPboardType;

UI_EXPORT BOOL PopulateURLAndTitleFromPasteboard(GURL* url,
                                                 base::string16* title,
                                                 NSPasteboard* pboard,
                                                 BOOL convert_filenames);

}  

#endif  
