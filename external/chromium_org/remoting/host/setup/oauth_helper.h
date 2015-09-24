// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_SETUP_OAUTH_HELPER_H
#define REMOTING_HOST_SETUP_OAUTH_HELPER_H

#include <string>

namespace remoting {

std::string GetOauthScope();

std::string GetDefaultOauthRedirectUrl();

std::string GetOauthStartUrl(const std::string& redirect_url);

std::string GetOauthCodeInUrl(const std::string& url,
                              const std::string& redirect_url);

}  

#endif  
