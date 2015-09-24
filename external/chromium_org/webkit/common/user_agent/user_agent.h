// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_COMMON_USER_AGENT_USER_AGENT_H_
#define WEBKIT_COMMON_USER_AGENT_USER_AGENT_H_

#include <string>

#include "base/basictypes.h"
#include "url/gurl.h"
#include "webkit/common/user_agent/webkit_user_agent_export.h"

namespace webkit_glue {

WEBKIT_USER_AGENT_EXPORT void SetUserAgent(const std::string& user_agent,
                                           bool overriding);

WEBKIT_USER_AGENT_EXPORT const std::string& GetUserAgent(const GURL& url);

}  

#endif  
