// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_DRAG_UTIL_H_
#define CHROME_BROWSER_UI_COCOA_DRAG_UTIL_H_

#import <Cocoa/Cocoa.h>

class GURL;
class Profile;

namespace drag_util {

GURL GetFileURLFromDropData(id<NSDraggingInfo> info);

BOOL IsUnsupportedDropData(Profile* profile, id<NSDraggingInfo> info);

}  

#endif  
