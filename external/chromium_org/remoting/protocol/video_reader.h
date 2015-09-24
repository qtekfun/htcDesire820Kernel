// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef REMOTING_PROTOCOL_VIDEO_READER_H_
#define REMOTING_PROTOCOL_VIDEO_READER_H_

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "remoting/protocol/video_stub.h"

namespace remoting {
namespace protocol {

class Session;
class SessionConfig;

class VideoReader {
 public:
  
  
  typedef base::Callback<void(bool)> InitializedCallback;

  virtual ~VideoReader();

  static scoped_ptr<VideoReader> Create(const SessionConfig& config);

  
  
  virtual void Init(Session* session,
                    VideoStub* video_stub,
                    const InitializedCallback& callback) = 0;
  virtual bool is_connected() = 0;

 protected:
  VideoReader() { }

 private:
  DISALLOW_COPY_AND_ASSIGN(VideoReader);
};

}  
}  

#endif  
