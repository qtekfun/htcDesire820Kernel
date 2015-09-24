// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_ANDROID_DEMUXER_ANDROID_H_
#define MEDIA_BASE_ANDROID_DEMUXER_ANDROID_H_

#include "base/basictypes.h"
#include "base/time/time.h"
#include "media/base/demuxer_stream.h"
#include "media/base/media_export.h"

namespace media {

class DemuxerAndroidClient;
struct DemuxerConfigs;
struct DemuxerData;

class MEDIA_EXPORT DemuxerAndroid {
 public:
  virtual ~DemuxerAndroid() {}

  
  
  virtual void Initialize(DemuxerAndroidClient* client) = 0;

  
  virtual void RequestDemuxerConfigs() = 0;

  
  virtual void RequestDemuxerData(media::DemuxerStream::Type type) = 0;

  
  
  
  
  
  
  virtual void RequestDemuxerSeek(const base::TimeDelta& time_to_seek,
                                  bool is_browser_seek) = 0;
};

class MEDIA_EXPORT DemuxerAndroidClient {
 public:
  
  
  
  
  
  
  virtual void OnDemuxerConfigsAvailable(const DemuxerConfigs& params) = 0;

  
  virtual void OnDemuxerDataAvailable(const DemuxerData& params) = 0;

  
  
  
  
  
  
  virtual void OnDemuxerSeekDone(
      const base::TimeDelta& actual_browser_seek_time) = 0;

  
  virtual void OnDemuxerDurationChanged(base::TimeDelta duration) = 0;

 protected:
  virtual ~DemuxerAndroidClient() {}
};

}  

#endif  
