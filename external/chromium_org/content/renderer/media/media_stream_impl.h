// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_MEDIA_STREAM_IMPL_H_
#define CONTENT_RENDERER_MEDIA_MEDIA_STREAM_IMPL_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "content/common/content_export.h"
#include "content/public/renderer/render_view_observer.h"
#include "content/renderer/media/media_stream_client.h"
#include "content/renderer/media/media_stream_dispatcher_eventhandler.h"
#include "third_party/WebKit/public/platform/WebMediaStream.h"
#include "third_party/WebKit/public/platform/WebMediaStreamSource.h"
#include "third_party/WebKit/public/platform/WebVector.h"
#include "third_party/WebKit/public/web/WebUserMediaClient.h"
#include "third_party/WebKit/public/web/WebUserMediaRequest.h"
#include "third_party/libjingle/source/talk/app/webrtc/mediastreaminterface.h"

namespace content {
class MediaStreamAudioRenderer;
class MediaStreamDependencyFactory;
class MediaStreamDispatcher;
class MediaStreamSourceExtraData;
class WebRtcAudioRenderer;
class WebRtcLocalAudioRenderer;

class CONTENT_EXPORT MediaStreamImpl
    : public RenderViewObserver,
      NON_EXPORTED_BASE(public blink::WebUserMediaClient),
      NON_EXPORTED_BASE(public MediaStreamClient),
      public MediaStreamDispatcherEventHandler,
      public base::SupportsWeakPtr<MediaStreamImpl>,
      NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  MediaStreamImpl(
      RenderView* render_view,
      MediaStreamDispatcher* media_stream_dispatcher,
      MediaStreamDependencyFactory* dependency_factory);
  virtual ~MediaStreamImpl();

  
  virtual void requestUserMedia(
      const blink::WebUserMediaRequest& user_media_request) OVERRIDE;
  virtual void cancelUserMediaRequest(
      const blink::WebUserMediaRequest& user_media_request) OVERRIDE;

  
  virtual bool IsMediaStream(const GURL& url) OVERRIDE;
  virtual scoped_refptr<VideoFrameProvider> GetVideoFrameProvider(
      const GURL& url,
      const base::Closure& error_cb,
      const VideoFrameProvider::RepaintCB& repaint_cb) OVERRIDE;
  virtual scoped_refptr<MediaStreamAudioRenderer>
      GetAudioRenderer(const GURL& url) OVERRIDE;

  
  virtual void OnStreamGenerated(
      int request_id,
      const std::string& label,
      const StreamDeviceInfoArray& audio_array,
      const StreamDeviceInfoArray& video_array) OVERRIDE;
  virtual void OnStreamGenerationFailed(int request_id) OVERRIDE;
  virtual void OnDeviceStopped(const std::string& label,
                               const StreamDeviceInfo& device_info) OVERRIDE;
  virtual void OnDevicesEnumerated(
      int request_id,
      const StreamDeviceInfoArray& device_array) OVERRIDE;
  virtual void OnDeviceOpened(
      int request_id,
      const std::string& label,
      const StreamDeviceInfo& device_info) OVERRIDE;
  virtual void OnDeviceOpenFailed(int request_id) OVERRIDE;

  
  virtual void FrameDetached(blink::WebFrame* frame) OVERRIDE;
  virtual void FrameWillClose(blink::WebFrame* frame) OVERRIDE;

 protected:
  void OnLocalSourceStop(const blink::WebMediaStreamSource& source);

  void OnLocalMediaStreamStop(const std::string& label);

  
  
  
  
  
  void OnCreateNativeSourcesComplete(
      blink::WebMediaStream* web_stream,
      bool request_succeeded);

  
  
  
  virtual void CompleteGetUserMediaRequest(
      const blink::WebMediaStream& stream,
      blink::WebUserMediaRequest* request_info,
      bool request_succeeded);

  
  
  virtual blink::WebMediaStream GetMediaStream(const GURL& url);

 private:
  
  
  struct UserMediaRequestInfo {
    UserMediaRequestInfo(int request_id,
                         blink::WebFrame* frame,
                         const blink::WebUserMediaRequest& request,
                         bool enable_automatic_output_device_selection);
    ~UserMediaRequestInfo();
    int request_id;
    
    
    bool generated;
    const bool enable_automatic_output_device_selection;
    blink::WebFrame* frame;  
    blink::WebMediaStream web_stream;
    blink::WebUserMediaRequest request;
    std::vector<blink::WebMediaStreamSource> sources;
  };
  typedef ScopedVector<UserMediaRequestInfo> UserMediaRequests;

  struct LocalStreamSource {
    LocalStreamSource(blink::WebFrame* frame,
                      const blink::WebMediaStreamSource& source)
        : frame(frame), source(source) {
    }
    
    
    blink::WebFrame* frame;
    blink::WebMediaStreamSource source;
  };
  typedef std::vector<LocalStreamSource> LocalStreamSources;

  
  
  void CreateWebKitSourceVector(
      const std::string& label,
      const StreamDeviceInfoArray& devices,
      blink::WebMediaStreamSource::Type type,
      blink::WebFrame* frame,
      blink::WebVector<blink::WebMediaStreamSource>& webkit_sources);

  UserMediaRequestInfo* FindUserMediaRequestInfo(int request_id);
  UserMediaRequestInfo* FindUserMediaRequestInfo(
      blink::WebMediaStream* web_stream);
  UserMediaRequestInfo* FindUserMediaRequestInfo(
      const blink::WebUserMediaRequest& request);
  UserMediaRequestInfo* FindUserMediaRequestInfo(const std::string& label);
  void DeleteUserMediaRequestInfo(UserMediaRequestInfo* request);

  
  
  const blink::WebMediaStreamSource* FindLocalSource(
      const StreamDeviceInfo& device) const;

  
  bool FindSourceInRequests(const blink::WebMediaStreamSource& source) const;

  void StopLocalSource(const blink::WebMediaStreamSource& source,
                       bool notify_dispatcher);
  
  
  void StopUnreferencedSources(bool notify_dispatcher);

  scoped_refptr<WebRtcAudioRenderer> CreateRemoteAudioRenderer(
      webrtc::MediaStreamInterface* stream);
  scoped_refptr<WebRtcLocalAudioRenderer> CreateLocalAudioRenderer(
      const blink::WebMediaStreamTrack& audio_track);

  
  
  
  
  
  
  
  bool GetAuthorizedDeviceInfoForAudioRenderer(
      int* session_id, int* output_sample_rate, int* output_buffer_size);

  
  
  MediaStreamDependencyFactory* dependency_factory_;

  
  
  MediaStreamDispatcher* media_stream_dispatcher_;

  UserMediaRequests user_media_requests_;

  LocalStreamSources local_sources_;

  DISALLOW_COPY_AND_ASSIGN(MediaStreamImpl);
};

}  

#endif  
