// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_MEDIA_CAST_SESSION_H_
#define CHROME_RENDERER_MEDIA_CAST_SESSION_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/ip_endpoint.h"

namespace base {
class MessageLoopProxy;
}  

namespace media {
class VideoFrame;
namespace cast {
class FrameInput;
struct AudioSenderConfig;
struct VideoSenderConfig;
}  
}  

namespace content{
class P2PSocketClient;
}  

class CastSessionDelegate;

class CastSession : public base::RefCounted<CastSession> {
 public:
  typedef
  base::Callback<void(const scoped_refptr<media::cast::FrameInput>&)>
  FrameInputAvailableCallback;

  CastSession();

  
  
  
  
  
  void StartAudio(const media::cast::AudioSenderConfig& config,
                  const FrameInputAvailableCallback& callback);
  void StartVideo(const media::cast::VideoSenderConfig& config,
                  const FrameInputAvailableCallback& callback);

  class P2PSocketFactory {
   public:
    virtual ~P2PSocketFactory();

    
    virtual scoped_refptr<content::P2PSocketClient> Create() = 0;
  };

  
  
  
  
  
  
  
  void SetSocketFactory(scoped_ptr<P2PSocketFactory> socket_factory,
                        const net::IPEndPoint& remote_address);

 private:
  friend class base::RefCounted<CastSession>;
  virtual ~CastSession();

  
  
  
  
  scoped_ptr<CastSessionDelegate> delegate_;

  
  const scoped_refptr<base::MessageLoopProxy> io_message_loop_proxy_;

  DISALLOW_COPY_AND_ASSIGN(CastSession);
};

#endif  
