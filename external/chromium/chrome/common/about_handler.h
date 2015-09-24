// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_ABOUT_HANDLER_H__
#define CHROME_COMMON_ABOUT_HANDLER_H__
#pragma once

#include <stddef.h>

class GURL;

namespace chrome_about_handler {

extern const char* const about_urls[];
extern const size_t about_urls_size;  
extern const char* const kAboutScheme;

bool WillHandle(const GURL& url);

}  

#endif  
