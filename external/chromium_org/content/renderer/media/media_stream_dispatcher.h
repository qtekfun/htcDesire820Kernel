// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_MEDIA_STREAM_DISPATCHER_H_
#define CONTENT_RENDERER_MEDIA_MEDIA_STREAM_DISPATCHER_H_

#include <list>
#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "content/common/content_export.h"
#include "content/common/media/media_stream_options.h"
#include "content/public/renderer/render_view_observer.h"
#include "content/renderer/media/media_stream_dispatcher_eventhandler.h"

namespace base {
class MessageLoopProxy;
}

namespace content {

class RenderViewImpl;

class CONTENT_EXPORT MediaStreamDispatcher
    : public RenderViewObserver,
      public base::SupportsWeakPtr<MediaStreamDispatcher> {
 public:
  explicit MediaStreamDispatcher(RenderViewImpl* render_view);
  virtual ~MediaStreamDispatcher();

  
  
  
  virtual void GenerateStream(
      int request_id,
      const base::WeakPtr<MediaStreamDispatcherEventHandler>& event_handler,
      const StreamOptions& components,
      const GURL& security_origin);

  
  virtual void CancelGenerateStream(
      int request_id,
      const base::WeakPtr<MediaStreamDispatcherEventHandler>& event_handler);

  
  virtual void StopStreamDevice(const StreamDeviceInfo& device_info);

  
  void EnumerateDevices(
      int request_id,
      const base::WeakPtr<MediaStreamDispatcherEventHandler>& event_handler,
      MediaStreamType type,
      const GURL& security_origin);

  
  void StopEnumerateDevices(
      int request_id,
      const base::WeakPtr<MediaStreamDispatcherEventHandler>& event_handler);

  
  void OpenDevice(
      int request_id,
      const base::WeakPtr<MediaStreamDispatcherEventHandler>& event_handler,
      const std::string& device_id,
      MediaStreamType type,
      const GURL& security_origin);

  
  virtual void CancelOpenDevice(
      int request_id,
      const base::WeakPtr<MediaStreamDispatcherEventHandler>& event_handler);

  
  void CloseDevice(const std::string& label);

  
  virtual bool IsStream(const std::string& label);
  
  
  virtual int video_session_id(const std::string& label, int index);
  
  virtual int audio_session_id(const std::string& label, int index);

 protected:
  int GetNextIpcIdForTest() { return next_ipc_id_; }

 private:
  FRIEND_TEST_ALL_PREFIXES(MediaStreamDispatcherTest, BasicVideoDevice);
  FRIEND_TEST_ALL_PREFIXES(MediaStreamDispatcherTest, TestFailure);
  FRIEND_TEST_ALL_PREFIXES(MediaStreamDispatcherTest, CancelGenerateStream);

  struct Request;

  
  
  struct Stream;

  
  virtual bool Send(IPC::Message* message) OVERRIDE;

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  void OnStreamGenerated(
      int request_id,
      const std::string& label,
      const StreamDeviceInfoArray& audio_array,
      const StreamDeviceInfoArray& video_array);
  void OnStreamGenerationFailed(int request_id);
  void OnDeviceStopped(const std::string& label,
                       const StreamDeviceInfo& device_info);
  void OnDevicesEnumerated(
      int request_id,
      const StreamDeviceInfoArray& device_array);
  void OnDevicesEnumerationFailed(int request_id);
  void OnDeviceOpened(
      int request_id,
      const std::string& label,
      const StreamDeviceInfo& device_info);
  void OnDeviceOpenFailed(int request_id);

  
  scoped_refptr<base::MessageLoopProxy> main_loop_;

  int next_ipc_id_;
  typedef std::map<std::string, Stream> LabelStreamMap;
  LabelStreamMap label_stream_map_;

  
  
  typedef std::list<Request> RequestList;
  RequestList requests_;

  DISALLOW_COPY_AND_ASSIGN(MediaStreamDispatcher);
};

}  

#endif  
