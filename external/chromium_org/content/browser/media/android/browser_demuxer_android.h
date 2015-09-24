// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_MEDIA_ANDROID_BROWSER_DEMUXER_ANDROID_H_
#define CONTENT_BROWSER_MEDIA_ANDROID_BROWSER_DEMUXER_ANDROID_H_

#include "base/id_map.h"
#include "content/public/browser/browser_message_filter.h"
#include "media/base/android/demuxer_android.h"

namespace content {

class CONTENT_EXPORT BrowserDemuxerAndroid : public BrowserMessageFilter {
 public:
  BrowserDemuxerAndroid();

  
  virtual void OverrideThreadForMessage(const IPC::Message& message,
                                        BrowserThread::ID* thread) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) OVERRIDE;

  
  
  
  scoped_ptr<media::DemuxerAndroid> CreateDemuxer(int demuxer_client_id);

 protected:
  friend class base::RefCountedThreadSafe<BrowserDemuxerAndroid>;
  virtual ~BrowserDemuxerAndroid();

 private:
  class Internal;

  
  
  void AddDemuxerClient(int demuxer_client_id,
                        media::DemuxerAndroidClient* client);
  void RemoveDemuxerClient(int demuxer_client_id);

  
  void OnDemuxerReady(int demuxer_client_id,
                      const media::DemuxerConfigs& configs);
  void OnReadFromDemuxerAck(int demuxer_client_id,
                            const media::DemuxerData& data);
  void OnDemuxerSeekDone(int demuxer_client_id,
                         const base::TimeDelta& actual_browser_seek_time);
  void OnDurationChanged(int demuxer_client_id,
                         const base::TimeDelta& duration);

  IDMap<media::DemuxerAndroidClient> demuxer_clients_;

  DISALLOW_COPY_AND_ASSIGN(BrowserDemuxerAndroid);
};

}  

#endif  
