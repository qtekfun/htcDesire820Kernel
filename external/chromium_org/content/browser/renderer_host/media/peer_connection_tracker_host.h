// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_MEDIA_PEER_CONNECTION_TRACKER_HOST_H_
#define CONTENT_BROWSER_RENDERER_HOST_MEDIA_PEER_CONNECTION_TRACKER_HOST_H_

#include "content/public/browser/browser_message_filter.h"

struct PeerConnectionInfo;

namespace base {
class ListValue;
}  

namespace content {

class PeerConnectionTrackerHost : public BrowserMessageFilter {
 public:
  PeerConnectionTrackerHost(int render_process_id);

  
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) OVERRIDE;
  virtual void OverrideThreadForMessage(const IPC::Message& message,
                                        BrowserThread::ID* thread) OVERRIDE;

 protected:
  virtual ~PeerConnectionTrackerHost();

 private:
  
  void OnAddPeerConnection(const PeerConnectionInfo& info);
  void OnRemovePeerConnection(int lid);
  void OnUpdatePeerConnection(
      int lid, const std::string& type, const std::string& value);
  void OnAddStats(int lid, const base::ListValue& value);

  int render_process_id_;

  DISALLOW_COPY_AND_ASSIGN(PeerConnectionTrackerHost);
};

}  

#endif  
