// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_SYNC_ENGINE_TRAFFIC_LOGGER_H_
#define CHROME_BROWSER_SYNC_ENGINE_TRAFFIC_LOGGER_H_

namespace sync_pb {
class ClientToServerResponse;
class ClientToServerMessage;
}  

namespace syncer {

void LogClientToServerMessage(const sync_pb::ClientToServerMessage& msg);
void LogClientToServerResponse(
    const sync_pb::ClientToServerResponse& response);

}  

#endif  
