// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_APPLESCRIPT_ERROR_APPLESCRIPT_H_
#define CHROME_BROWSER_UI_COCOA_APPLESCRIPT_ERROR_APPLESCRIPT_H_

#import <Cocoa/Cocoa.h>

namespace AppleScript {

enum ErrorCode {
  
  errGetProfile = 1,
  
  errBookmarkModelLoad,
  
  errCreateBookmarkFolder,
  
  errCreateBookmarkItem,
  
  errInvalidURL,
  
  errInitiatePrinting,
  
  errInvalidSaveType,
  
  errInvalidMode,
  
  errInvalidTabIndex,
  
  errSetMode,
  
  errWrongIndex
};

void SetError(ErrorCode errorCode);
}

#endif  
