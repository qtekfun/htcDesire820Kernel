// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_PEERCONNECTION_TRACKER_H_
#define CONTENT_RENDERER_MEDIA_PEERCONNECTION_TRACKER_H_

#include <map>

#include "base/compiler_specific.h"
#include "content/public/renderer/render_process_observer.h"
#include "third_party/WebKit/public/platform/WebMediaStream.h"
#include "third_party/WebKit/public/platform/WebRTCPeerConnectionHandlerClient.h"
#include "third_party/WebKit/public/platform/WebRTCSessionDescription.h"
#include "third_party/libjingle/source/talk/app/webrtc/peerconnectioninterface.h"

namespace blink {
class WebFrame;
class WebRTCICECandidate;
class WebString;
class WebRTCSessionDescription;
}  

namespace webrtc {
class DataChannelInterface;
}  

namespace content {
class RTCMediaConstraints;
class RTCPeerConnectionHandler;

class CONTENT_EXPORT PeerConnectionTracker : public RenderProcessObserver {
 public:
  PeerConnectionTracker();
  virtual ~PeerConnectionTracker();

  enum Source {
    SOURCE_LOCAL,
    SOURCE_REMOTE
  };

  enum Action {
    ACTION_SET_LOCAL_DESCRIPTION,
    ACTION_SET_REMOTE_DESCRIPTION,
    ACTION_CREATE_OFFER,
    ACTION_CREATE_ANSWER
  };

  
  virtual bool OnControlMessageReceived(const IPC::Message& message) OVERRIDE;

  
  
  
  
  
  

  
  
  
  
  
  
  
  void RegisterPeerConnection(
      RTCPeerConnectionHandler* pc_handler,
      const std::vector<webrtc::PeerConnectionInterface::IceServer>& servers,
      const RTCMediaConstraints& constraints,
      const blink::WebFrame* frame);

  
  virtual void UnregisterPeerConnection(RTCPeerConnectionHandler* pc_handler);

  
  
  
  virtual void TrackCreateOffer(RTCPeerConnectionHandler* pc_handler,
                                const RTCMediaConstraints& constraints);
  virtual void TrackCreateAnswer(RTCPeerConnectionHandler* pc_handler,
                                 const RTCMediaConstraints& constraints);

  
  virtual void TrackSetSessionDescription(
      RTCPeerConnectionHandler* pc_handler,
      const blink::WebRTCSessionDescription& desc, Source source);

  
  virtual void TrackUpdateIce(
      RTCPeerConnectionHandler* pc_handler,
      const std::vector<webrtc::PeerConnectionInterface::IceServer>& servers,
      const RTCMediaConstraints& options);

  
  virtual void TrackAddIceCandidate(
      RTCPeerConnectionHandler* pc_handler,
      const blink::WebRTCICECandidate& candidate, Source source);

  
  virtual void TrackAddStream(
      RTCPeerConnectionHandler* pc_handler,
      const blink::WebMediaStream& stream, Source source);

  
  virtual void TrackRemoveStream(
      RTCPeerConnectionHandler* pc_handler,
      const blink::WebMediaStream& stream, Source source);

  
  virtual void TrackCreateDataChannel(
      RTCPeerConnectionHandler* pc_handler,
      const webrtc::DataChannelInterface* data_channel, Source source);

  
  virtual void TrackStop(RTCPeerConnectionHandler* pc_handler);

  
  virtual void TrackSignalingStateChange(
      RTCPeerConnectionHandler* pc_handler,
      blink::WebRTCPeerConnectionHandlerClient::SignalingState state);

  
  
  virtual void TrackIceConnectionStateChange(
      RTCPeerConnectionHandler* pc_handler,
      blink::WebRTCPeerConnectionHandlerClient::ICEConnectionState state);

  
  
  virtual void TrackIceGatheringStateChange(
      RTCPeerConnectionHandler* pc_handler,
      blink::WebRTCPeerConnectionHandlerClient::ICEGatheringState state);

  
  
  virtual void TrackSessionDescriptionCallback(
      RTCPeerConnectionHandler* pc_handler, Action action,
      const std::string& type, const std::string& value);

  
  virtual void TrackOnRenegotiationNeeded(RTCPeerConnectionHandler* pc_handler);

  
  virtual void TrackCreateDTMFSender(
      RTCPeerConnectionHandler* pc_handler,
      const blink::WebMediaStreamTrack& track);

 private:
  
  
  int GetNextLocalID();

  
  void OnGetAllStats();

  void SendPeerConnectionUpdate(RTCPeerConnectionHandler* pc_handler,
                                const std::string& callback_type,
                                const std::string& value);

  
  typedef std::map<RTCPeerConnectionHandler*, int> PeerConnectionIdMap;
  PeerConnectionIdMap peer_connection_id_map_;

  
  int next_lid_;

  DISALLOW_COPY_AND_ASSIGN(PeerConnectionTracker);
};

}  

#endif  
