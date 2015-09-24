// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_PEER_CONNECTION_HANDLER_BASE_H_
#define CONTENT_RENDERER_MEDIA_PEER_CONNECTION_HANDLER_BASE_H_

#include <map>
#include <string>

#include "base/memory/ref_counted.h"
#include "base/message_loop/message_loop_proxy.h"
#include "content/common/content_export.h"
#include "third_party/WebKit/public/platform/WebMediaStream.h"
#include "third_party/WebKit/public/platform/WebMediaStreamTrack.h"
#include "third_party/libjingle/source/talk/app/webrtc/mediastream.h"
#include "third_party/libjingle/source/talk/app/webrtc/peerconnectioninterface.h"

namespace content {
class MediaStreamDependencyFactory;
class RemoteMediaStreamImpl;

class CONTENT_EXPORT PeerConnectionHandlerBase
    : NON_EXPORTED_BASE(public webrtc::PeerConnectionObserver) {
 public:
  PeerConnectionHandlerBase(
      MediaStreamDependencyFactory* dependency_factory);

 protected:
  virtual ~PeerConnectionHandlerBase();

  void AddStream(const blink::WebMediaStream& stream);
  bool AddStream(const blink::WebMediaStream& stream,
                 const webrtc::MediaConstraintsInterface* constraints);
  void RemoveStream(const blink::WebMediaStream& stream);

  
  
  MediaStreamDependencyFactory* dependency_factory_;

  
  
  scoped_refptr<webrtc::PeerConnectionInterface> native_peer_connection_;

  typedef std::map<webrtc::MediaStreamInterface*,
      content::RemoteMediaStreamImpl*> RemoteStreamMap;
  RemoteStreamMap remote_streams_;

  
  
  scoped_refptr<base::MessageLoopProxy> message_loop_proxy_;

  DISALLOW_COPY_AND_ASSIGN(PeerConnectionHandlerBase);
};

}  

#endif  
