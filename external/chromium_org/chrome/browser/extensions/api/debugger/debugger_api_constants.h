// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_API_DEBUGGER_DEBUGGER_API_CONSTANTS_H_
#define CHROME_BROWSER_EXTENSIONS_API_DEBUGGER_DEBUGGER_API_CONSTANTS_H_

namespace debugger_api_constants {

extern const char kOnEvent[];
extern const char kOnDetach[];

extern const char kAlreadyAttachedError[];
extern const char kAttachToWebUIError[];
extern const char kNoTargetError[];
extern const char kInvalidTargetError[];
extern const char kNotAttachedError[];
extern const char kProtocolVersionNotSupportedError[];
extern const char kSilentDebuggingRequired[];

extern const char kTabTargetType[];
extern const char kBackgroundPageTargetType[];
extern const char kOpaqueTargetType[];

}  

#endif  
