// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_MEDIA_WEBRTC_INTERNALS_H_
#define CONTENT_BROWSER_MEDIA_WEBRTC_INTERNALS_H_

#include "base/memory/singleton.h"
#include "base/observer_list.h"
#include "base/process/process.h"
#include "base/values.h"
#include "content/common/content_export.h"
#include "content/public/browser/browser_child_process_observer.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

namespace content {
class WebRTCInternalsUIObserver;

class CONTENT_EXPORT WebRTCInternals : public BrowserChildProcessObserver,
                                       public NotificationObserver {
 public:
  static WebRTCInternals* GetInstance();

  
  
  
  
  
  
  
  
  void OnAddPeerConnection(int render_process_id,
                           base::ProcessId pid,
                           int lid,
                           const std::string& url,
                           const std::string& servers,
                           const std::string& constraints);

  
  
  void OnRemovePeerConnection(base::ProcessId pid, int lid);

  
  
  
  void OnUpdatePeerConnection(base::ProcessId pid,
                              int lid,
                              const std::string& type,
                              const std::string& value);

  
  
  
  void OnAddStats(base::ProcessId pid, int lid, const base::ListValue& value);

  
  void AddObserver(WebRTCInternalsUIObserver *observer);
  void RemoveObserver(WebRTCInternalsUIObserver *observer);

  
  void SendAllUpdates();

  
  void StartRtpRecording();
  void StopRtpRecording();

 private:
  friend struct DefaultSingletonTraits<WebRTCInternals>;

  WebRTCInternals();
  virtual ~WebRTCInternals();

  void SendUpdate(const std::string& command, base::Value* value);

  
  virtual void BrowserChildProcessCrashed(
      const ChildProcessData& data) OVERRIDE;

  
  virtual void Observe(int type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  
  void OnRendererExit(int render_process_id);

  void SendRtpRecordingUpdate();

  ObserverList<WebRTCInternalsUIObserver> observers_;

  
  
  
  
  
  
  
  
  
  
  
  
  base::ListValue peer_connection_data_;

  NotificationRegistrar registrar_;

  bool is_recording_rtp_;
};

}  

#endif  
