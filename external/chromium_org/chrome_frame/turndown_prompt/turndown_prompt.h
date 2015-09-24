// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_TURNDOWN_PROMPT_TURNDOWN_PROMPT_H_
#define CHROME_FRAME_TURNDOWN_PROMPT_TURNDOWN_PROMPT_H_

#include <atlbase.h>
#include <atlcom.h>

interface IWebBrowser2;

namespace turndown_prompt {

bool IsPromptSuppressed();

void Configure(IWebBrowser2* web_browser);

};  

#endif  
