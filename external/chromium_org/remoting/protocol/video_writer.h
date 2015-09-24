// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef REMOTING_PROTOCOL_VIDEO_WRITER_H_
#define REMOTING_PROTOCOL_VIDEO_WRITER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "remoting/protocol/video_stub.h"

namespace remoting {
namespace protocol {

class Session;
class SessionConfig;

class VideoWriter : public VideoStub {
 public:
  virtual ~VideoWriter();

  
  
  typedef base::Callback<void(bool)> InitializedCallback;

  static scoped_ptr<VideoWriter> Create(const SessionConfig& config);

  
  virtual void Init(Session* session, const InitializedCallback& callback) = 0;

  
  
  virtual void Close() = 0;

  
  virtual bool is_connected() = 0;

 protected:
  VideoWriter() { }

 private:
  DISALLOW_COPY_AND_ASSIGN(VideoWriter);
};

}  
}  

#endif  
