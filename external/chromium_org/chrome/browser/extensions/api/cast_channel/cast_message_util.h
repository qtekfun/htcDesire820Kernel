// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_CAST_CHANNEL_CAST_MESSAGE_UTIL_H_
#define CHROME_BROWSER_EXTENSIONS_API_CAST_CHANNEL_CAST_MESSAGE_UTIL_H_

#include <string>

namespace extensions {
namespace api {
namespace cast_channel {

class CastMessage;
class DeviceAuthMessage;
struct MessageInfo;

bool MessageInfoToCastMessage(const MessageInfo& message,
                              CastMessage* message_proto);

bool CastMessageToMessageInfo(const CastMessage& message_proto,
                              MessageInfo* message);

std::string CastMessageToString(const CastMessage& message_proto);

std::string AuthMessageToString(const DeviceAuthMessage& message);

void CreateAuthChallengeMessage(CastMessage* message_proto);

bool IsAuthMessage(const CastMessage& message);

}  
}  
}  

#endif  
