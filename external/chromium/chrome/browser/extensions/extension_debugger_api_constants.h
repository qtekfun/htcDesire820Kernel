// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_DEBUGGER_API_CONSTANTS_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_DEBUGGER_API_CONSTANTS_H_
#pragma once

namespace extension_debugger_api_constants {

extern const char kCommandKey[];
extern const char kDataKey[];
extern const char kTabIdKey[];

extern const char kOnEvent[];
extern const char kOnDetach[];

extern const char kAlreadyAttachedError[];
extern const char kDebuggerExecuteFailedError[];
extern const char kNoTabError[];
extern const char kNotAttachedError[];
extern const char kPermissionError[];

}  

#endif  
