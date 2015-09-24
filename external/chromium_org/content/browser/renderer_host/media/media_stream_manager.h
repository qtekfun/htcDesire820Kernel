// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#ifndef CONTENT_BROWSER_RENDERER_HOST_MEDIA_MEDIA_STREAM_MANAGER_H_
#define CONTENT_BROWSER_RENDERER_HOST_MEDIA_MEDIA_STREAM_MANAGER_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/system_monitor/system_monitor.h"
#include "content/browser/renderer_host/media/media_stream_provider.h"
#include "content/common/content_export.h"
#include "content/common/media/media_stream_options.h"
#include "content/public/browser/media_request_state.h"
#include "content/public/browser/resource_context.h"

namespace base {
class Thread;
}

namespace media {
class AudioManager;
}

namespace content {

class AudioInputDeviceManager;
class FakeMediaStreamUIProxy;
class MediaStreamDeviceSettings;
class MediaStreamRequester;
class MediaStreamUIProxy;
class VideoCaptureManager;

class CONTENT_EXPORT MediaStreamManager
    : public MediaStreamProviderListener,
      public base::MessageLoop::DestructionObserver,
      public base::SystemMonitor::DevicesChangedObserver {
 public:
  
  typedef base::Callback<void(const MediaStreamDevices& devices,
                              scoped_ptr<MediaStreamUIProxy> ui)>
      MediaRequestResponseCallback;

  explicit MediaStreamManager(media::AudioManager* audio_manager);
  virtual ~MediaStreamManager();

  
  VideoCaptureManager* video_capture_manager();

  
  AudioInputDeviceManager* audio_input_device_manager();

  
  
  
  
  
  
  std::string MakeMediaAccessRequest(
      int render_process_id,
      int render_view_id,
      int page_request_id,
      const StreamOptions& options,
      const GURL& security_origin,
      const MediaRequestResponseCallback& callback);

  
  
  
  
  void GenerateStream(MediaStreamRequester* requester,
                      int render_process_id,
                      int render_view_id,
                      const ResourceContext::SaltCallback& sc,
                      int page_request_id,
                      const StreamOptions& components,
                      const GURL& security_origin);

  void CancelRequest(int render_process_id,
                     int render_view_id,
                     int page_request_id);

  
  virtual void CancelRequest(const std::string& label);

  
  void CancelAllRequests(int render_process_id);

  
  
  void StopStreamDevice(int render_process_id,
                        int render_view_id,
                        const std::string& device_id);

  
  
  
  
  
  
  
  virtual std::string EnumerateDevices(MediaStreamRequester* requester,
                                       int render_process_id,
                                       int render_view_id,
                                       const ResourceContext::SaltCallback& sc,
                                       int page_request_id,
                                       MediaStreamType type,
                                       const GURL& security_origin);

  
  
  
  void OpenDevice(MediaStreamRequester* requester,
                  int render_process_id,
                  int render_view_id,
                  const ResourceContext::SaltCallback& sc,
                  int page_request_id,
                  const std::string& device_id,
                  MediaStreamType type,
                  const GURL& security_origin);

  
  
  void EnsureDeviceMonitorStarted();

  
  virtual void Opened(MediaStreamType stream_type,
                      int capture_session_id) OVERRIDE;
  virtual void Closed(MediaStreamType stream_type,
                      int capture_session_id) OVERRIDE;
  virtual void DevicesEnumerated(MediaStreamType stream_type,
                                 const StreamDeviceInfoArray& devices) OVERRIDE;

  
  virtual void OnDevicesChanged(
      base::SystemMonitor::DeviceType device_type) OVERRIDE;

  
  
  
  void UseFakeDevice();

  
  
  void UseFakeUI(scoped_ptr<FakeMediaStreamUIProxy> fake_ui);

  
  
  StreamDeviceInfoArray GetDevicesOpenedByRequest(
      const std::string& label) const;

  
  
  
  
  
  
  
  
  virtual void WillDestroyCurrentMessageLoop() OVERRIDE;

 protected:
  
  MediaStreamManager();

 private:
  
  class DeviceRequest;

  
  struct EnumerationCache {
    EnumerationCache();
    ~EnumerationCache();

    bool valid;
    StreamDeviceInfoArray devices;
  };

  typedef std::map<std::string, DeviceRequest*> DeviceRequests;

  
  
  void InitializeDeviceManagersOnIOThread();

  
  
  void NotifyDevicesChanged(MediaStreamType stream_type,
                            const StreamDeviceInfoArray& devices);

  void HandleAccessRequestResponse(const std::string& label,
                                   const MediaStreamDevices& devices);
  void StopMediaStreamFromBrowser(const std::string& label);

  void DoEnumerateDevices(const std::string& label);

  
  
  
  void HandleRequestDone(const std::string& label,
                         DeviceRequest* request);
  
  
  
  void StopDevice(MediaStreamType type, int session_id);
  
  
  void CloseDevice(MediaStreamType type, int session_id);
  
  
  bool RequestDone(const DeviceRequest& request) const;
  MediaStreamProvider* GetDeviceManager(MediaStreamType stream_type);
  void StartEnumeration(DeviceRequest* request);
  std::string AddRequest(DeviceRequest* request);
  DeviceRequest* FindRequest(const std::string& label) const;
  void DeleteRequest(const std::string& label);
  void ClearEnumerationCache(EnumerationCache* cache);
  
  
  void SetupRequest(const std::string& label);
  
  
  
  bool SetupDeviceCaptureRequest(DeviceRequest* request);
  
  
  
  bool SetupTabCaptureRequest(DeviceRequest* request);
  
  
  
  bool SetupScreenCaptureRequest(DeviceRequest* request);
  
  
  void PostRequestToUI(const std::string& label, DeviceRequest* request);
  
  
  
  
  bool FindExistingRequestedDeviceInfo(
      const DeviceRequest& new_request,
      const MediaStreamDevice& new_device_info,
      StreamDeviceInfo* existing_device_info,
      MediaRequestState* existing_request_state) const;

  void FinalizeGenerateStream(const std::string& label,
                              DeviceRequest* request);
  void FinalizeRequestFailed(const std::string& label,
                             DeviceRequest* request);
  void FinalizeOpenDevice(const std::string& label,
                          DeviceRequest* request);
  void FinalizeMediaAccessRequest(const std::string& label,
                                  DeviceRequest* request,
                                  const MediaStreamDevices& devices);
  void FinalizeEnumerateDevices(const std::string& label,
                                DeviceRequest* request);

  
  
  
  
  
  void StopRemovedDevices(const StreamDeviceInfoArray& old_devices,
                          const StreamDeviceInfoArray& new_devices);
  
  
  void StopRemovedDevice(const MediaStreamDevice& device);

  
  void StartMonitoring();
  void StopMonitoring();

  
  
  bool GetRequestedDeviceCaptureId(const DeviceRequest* request,
                                   MediaStreamType type,
                                   std::string* device_id) const;

  void TranslateDeviceIdToSourceId(DeviceRequest* request,
                                   MediaStreamDevice* device);

  
  
  
  bool TranslateSourceIdToDeviceId(
      MediaStreamType stream_type,
      const ResourceContext::SaltCallback& rc,
      const GURL& security_origin,
      const std::string& source_id,
      std::string* device_id) const;

  
  scoped_ptr<base::Thread> device_thread_;

  media::AudioManager* const audio_manager_;  
  scoped_refptr<AudioInputDeviceManager> audio_input_device_manager_;
  scoped_refptr<VideoCaptureManager> video_capture_manager_;

  
  bool monitoring_started_;

  
  
  EnumerationCache audio_enumeration_cache_;
  EnumerationCache video_enumeration_cache_;

  
  
  int active_enumeration_ref_count_[NUM_MEDIA_TYPES];

  
  DeviceRequests requests_;

  
  
  base::MessageLoop* io_loop_;

  bool use_fake_ui_;
  scoped_ptr<FakeMediaStreamUIProxy> fake_ui_;

  DISALLOW_COPY_AND_ASSIGN(MediaStreamManager);
};

}  

#endif  
