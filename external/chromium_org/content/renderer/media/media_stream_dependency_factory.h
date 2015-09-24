// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_MEDIA_STREAM_DEPENDENCY_FACTORY_H_
#define CONTENT_RENDERER_MEDIA_MEDIA_STREAM_DEPENDENCY_FACTORY_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/threading/thread.h"
#include "content/common/content_export.h"
#include "content/renderer/media/media_stream_extra_data.h"
#include "content/renderer/p2p/socket_dispatcher.h"
#include "third_party/libjingle/source/talk/app/webrtc/peerconnectioninterface.h"
#include "third_party/libjingle/source/talk/app/webrtc/videosourceinterface.h"

namespace base {
class WaitableEvent;
}

namespace talk_base {
class NetworkManager;
class PacketSocketFactory;
class Thread;
}

namespace webrtc {
class PeerConnection;
}

namespace blink {
class WebFrame;
class WebMediaConstraints;
class WebMediaStream;
class WebRTCPeerConnectionHandler;
class WebRTCPeerConnectionHandlerClient;
}

namespace content {

class IpcNetworkManager;
class IpcPacketSocketFactory;
class RTCMediaConstraints;
class VideoCaptureImplManager;
class WebAudioCapturerSource;
class WebRtcAudioCapturer;
class WebRtcAudioDeviceImpl;
class WebRtcLoggingHandlerImpl;
class WebRtcLoggingMessageFilter;
struct StreamDeviceInfo;

#if defined(GOOGLE_TV)
class RTCVideoDecoderFactoryTv;
#endif

class CONTENT_EXPORT MediaStreamDependencyFactory
    : NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  
  typedef base::Callback<void(blink::WebMediaStream* web_stream,
                              bool live)> MediaSourcesCreatedCallback;
  MediaStreamDependencyFactory(
      VideoCaptureImplManager* vc_manager,
      P2PSocketDispatcher* p2p_socket_dispatcher);
  virtual ~MediaStreamDependencyFactory();

  
  
  blink::WebRTCPeerConnectionHandler* CreateRTCPeerConnectionHandler(
      blink::WebRTCPeerConnectionHandlerClient* client);

  
  
  
  
  
  
  
  void CreateNativeMediaSources(
      int render_view_id,
      const blink::WebMediaConstraints& audio_constraints,
      const blink::WebMediaConstraints& video_constraints,
      blink::WebMediaStream* web_stream,
      const MediaSourcesCreatedCallback& sources_created);

  
  
  void CreateNativeLocalMediaStream(
      blink::WebMediaStream* web_stream);

  
  
  
  
  void CreateNativeLocalMediaStream(
      blink::WebMediaStream* web_stream,
      const MediaStreamExtraData::StreamStopCallback& stream_stop);

  
  
  void CreateNativeMediaStreamTrack(const blink::WebMediaStreamTrack& track);

  
  
  bool AddNativeMediaStreamTrack(const blink::WebMediaStream& stream,
                                 const blink::WebMediaStreamTrack& track);

  
  
  bool AddNativeVideoMediaTrack(const std::string& track_id,
                                blink::WebMediaStream* stream,
                                cricket::VideoCapturer* capturer);

  bool RemoveNativeMediaStreamTrack(const blink::WebMediaStream& stream,
                                    const blink::WebMediaStreamTrack& track);

  
  
  
  virtual scoped_refptr<webrtc::PeerConnectionInterface>
      CreatePeerConnection(
          const webrtc::PeerConnectionInterface::IceServers& ice_servers,
          const webrtc::MediaConstraintsInterface* constraints,
          blink::WebFrame* web_frame,
          webrtc::PeerConnectionObserver* observer);

  
  
  virtual webrtc::SessionDescriptionInterface* CreateSessionDescription(
      const std::string& type,
      const std::string& sdp,
      webrtc::SdpParseError* error);

  
  virtual webrtc::IceCandidateInterface* CreateIceCandidate(
      const std::string& sdp_mid,
      int sdp_mline_index,
      const std::string& sdp);

  WebRtcAudioDeviceImpl* GetWebRtcAudioDevice();

#if defined(GOOGLE_TV)
  RTCVideoDecoderFactoryTv* decoder_factory_tv() { return decoder_factory_tv_; }
#endif

  static void AddNativeTrackToBlinkTrack(
      webrtc::MediaStreamTrackInterface* native_track,
      const blink::WebMediaStreamTrack& webkit_track,
      bool is_local_track);

  static webrtc::MediaStreamInterface* GetNativeMediaStream(
      const blink::WebMediaStream& stream);

  static webrtc::MediaStreamTrackInterface* GetNativeMediaStreamTrack(
      const blink::WebMediaStreamTrack& track);

 protected:
  
  virtual scoped_refptr<webrtc::MediaStreamInterface>
      CreateLocalMediaStream(const std::string& label);

  
  virtual scoped_refptr<webrtc::AudioSourceInterface>
      CreateLocalAudioSource(
          const webrtc::MediaConstraintsInterface* constraints);

  
  virtual scoped_refptr<webrtc::VideoSourceInterface>
      CreateLocalVideoSource(
          int video_session_id,
          bool is_screen_cast,
          const webrtc::MediaConstraintsInterface* constraints);

  
  
  
  
  
  
  virtual scoped_refptr<WebAudioCapturerSource> CreateWebAudioSource(
      blink::WebMediaStreamSource* source, RTCMediaConstraints* constraints);

  
  virtual scoped_refptr<webrtc::AudioTrackInterface>
      CreateLocalAudioTrack(
          const std::string& id,
          const scoped_refptr<WebRtcAudioCapturer>& capturer,
          WebAudioCapturerSource* webaudio_source,
          webrtc::AudioSourceInterface* source,
          const webrtc::MediaConstraintsInterface* constraints);

  
  virtual scoped_refptr<webrtc::VideoTrackInterface>
      CreateLocalVideoTrack(const std::string& id,
                            webrtc::VideoSourceInterface* source);

  
  
  virtual scoped_refptr<webrtc::VideoTrackInterface>
      CreateLocalVideoTrack(const std::string& id,
                            cricket::VideoCapturer* capturer);

  virtual bool EnsurePeerConnectionFactory();
  virtual bool PeerConnectionFactoryCreated();

  
  
  
  virtual scoped_refptr<WebRtcAudioCapturer> MaybeCreateAudioCapturer(
      int render_view_id, const StreamDeviceInfo& device_info);

 private:
  
  
  bool CreatePeerConnectionFactory();

  void InitializeWorkerThread(talk_base::Thread** thread,
                              base::WaitableEvent* event);

  void CreateIpcNetworkManagerOnWorkerThread(base::WaitableEvent* event);
  void DeleteIpcNetworkManager();
  void CleanupPeerConnectionFactory();

  scoped_refptr<webrtc::AudioTrackInterface>
  CreateNativeAudioMediaStreamTrack(const blink::WebMediaStreamTrack& track);

  scoped_refptr<webrtc::VideoTrackInterface>
  CreateNativeVideoMediaStreamTrack(const blink::WebMediaStreamTrack& track);

  
  
  IpcNetworkManager* network_manager_;
  scoped_ptr<IpcPacketSocketFactory> socket_factory_;

  scoped_refptr<webrtc::PeerConnectionFactoryInterface> pc_factory_;

#if defined(GOOGLE_TV)
  
  
  RTCVideoDecoderFactoryTv* decoder_factory_tv_;
#endif

  scoped_refptr<VideoCaptureImplManager> vc_manager_;
  scoped_refptr<P2PSocketDispatcher> p2p_socket_dispatcher_;
  scoped_refptr<WebRtcAudioDeviceImpl> audio_device_;

  
  
  talk_base::Thread* signaling_thread_;
  talk_base::Thread* worker_thread_;
  base::Thread chrome_worker_thread_;

  DISALLOW_COPY_AND_ASSIGN(MediaStreamDependencyFactory);
};

}  

#endif  
