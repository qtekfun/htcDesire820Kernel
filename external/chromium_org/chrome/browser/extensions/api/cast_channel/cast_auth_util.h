// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_CAST_CHANNEL_CAST_AUTH_UTIL_H_
#define CHROME_BROWSER_EXTENSIONS_API_CAST_CHANNEL_CAST_AUTH_UTIL_H_

#include <string>

namespace extensions {
namespace api {
namespace cast_channel {

class CastMessage;

bool AuthenticateChallengeReply(const CastMessage& challenge_reply,
                                const std::string& peer_cert);

}  
}  
}  

#endif  
