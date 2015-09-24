// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_DEMUXER_H_
#define MEDIA_BASE_DEMUXER_H_

#include <vector>

#include "base/time/time.h"
#include "media/base/data_source.h"
#include "media/base/demuxer_stream.h"
#include "media/base/media_export.h"
#include "media/base/pipeline_status.h"

namespace media {

class TextTrackConfig;

class MEDIA_EXPORT DemuxerHost : public DataSourceHost {
 public:
  
  
  virtual void SetDuration(base::TimeDelta duration) = 0;

  
  
  virtual void OnDemuxerError(PipelineStatus error) = 0;

  
  virtual void AddTextStream(DemuxerStream* text_stream,
                             const TextTrackConfig& config) = 0;

  
  virtual void RemoveTextStream(DemuxerStream* text_stream) = 0;

 protected:
  virtual ~DemuxerHost();
};

class MEDIA_EXPORT Demuxer {
 public:
  
  
  
  typedef base::Callback<void(const std::string& type,
                              const std::vector<uint8>& init_data)> NeedKeyCB;

  Demuxer();
  virtual ~Demuxer();

  
  
  
  
  virtual void Initialize(DemuxerHost* host,
                          const PipelineStatusCB& status_cb,
                          bool enable_text_tracks) = 0;

  
  
  virtual void Seek(base::TimeDelta time,
                    const PipelineStatusCB& status_cb) = 0;

  
  
  
  
  virtual void Stop(const base::Closure& callback) = 0;

  
  
  
  
  
  
  virtual void OnAudioRendererDisabled() = 0;

  
  
  virtual DemuxerStream* GetStream(DemuxerStream::Type type) = 0;

  
  virtual base::TimeDelta GetStartTime() const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(Demuxer);
};

}  

#endif  
