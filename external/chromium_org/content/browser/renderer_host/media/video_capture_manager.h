// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_BROWSER_RENDERER_HOST_MEDIA_VIDEO_CAPTURE_MANAGER_H_
#define CONTENT_BROWSER_RENDERER_HOST_MEDIA_VIDEO_CAPTURE_MANAGER_H_

#include <map>
#include <set>
#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/process/process_handle.h"
#include "content/browser/renderer_host/media/media_stream_provider.h"
#include "content/browser/renderer_host/media/video_capture_controller_event_handler.h"
#include "content/common/content_export.h"
#include "content/common/media/media_stream_options.h"
#include "media/video/capture/video_capture_device.h"
#include "media/video/capture/video_capture_types.h"

namespace content {
class VideoCaptureController;
class VideoCaptureControllerEventHandler;

class CONTENT_EXPORT VideoCaptureManager : public MediaStreamProvider {
 public:
  
  typedef base::Callback<
      void(const base::WeakPtr<VideoCaptureController>&)> DoneCB;

  VideoCaptureManager();

  
  virtual void Register(MediaStreamProviderListener* listener,
                        base::MessageLoopProxy* device_thread_loop) OVERRIDE;

  virtual void Unregister() OVERRIDE;

  virtual void EnumerateDevices(MediaStreamType stream_type) OVERRIDE;

  virtual int Open(const StreamDeviceInfo& device) OVERRIDE;

  virtual void Close(int capture_session_id) OVERRIDE;

  
  
  
  void UseFakeDevice();

  
  
  
  
  
  
  
  
  
  
  
  
  void StartCaptureForClient(media::VideoCaptureSessionId session_id,
                             const media::VideoCaptureParams& capture_params,
                             base::ProcessHandle client_render_process,
                             VideoCaptureControllerID client_id,
                             VideoCaptureControllerEventHandler* client_handler,
                             const DoneCB& done_cb);

  
  
  
  
  void StopCaptureForClient(VideoCaptureController* controller,
                            VideoCaptureControllerID client_id,
                            VideoCaptureControllerEventHandler* client_handler);

  
  
  void GetDeviceSupportedFormats(int capture_session_id,
                                 media::VideoCaptureFormats* supported_formats);

 private:
  virtual ~VideoCaptureManager();
  struct DeviceEntry;

  
  
  struct DeviceInfo {
    DeviceInfo();
    DeviceInfo(const media::VideoCaptureDevice::Name& name,
               const media::VideoCaptureFormats& supported_formats);
    ~DeviceInfo();

    media::VideoCaptureDevice::Name name;
    media::VideoCaptureFormats supported_formats;
  };
  typedef std::vector<DeviceInfo> DeviceInfos;

  
  
  
  void DestroyDeviceEntryIfNoClients(DeviceEntry* entry);

  
  void OnOpened(MediaStreamType type, int capture_session_id);
  void OnClosed(MediaStreamType type, int capture_session_id);
  void OnDevicesInfoEnumerated(
      MediaStreamType stream_type,
      const DeviceInfos& new_devices_info_cache);

  
  DeviceEntry* GetDeviceEntryForMediaStreamDevice(
      const MediaStreamDevice& device_info);

  
  
  DeviceEntry* GetOrCreateDeviceEntry(int capture_session_id);

  
  DeviceEntry* GetDeviceEntryForController(
      const VideoCaptureController* controller);

  bool IsOnDeviceThread() const;

  
  
  
  
  DeviceInfos GetAvailableDevicesInfoOnDeviceThread(
      MediaStreamType stream_type,
      const DeviceInfos& old_device_info_cache);

  
  
  
  void DoStartDeviceOnDeviceThread(
      DeviceEntry* entry,
      const media::VideoCaptureParams& params,
      scoped_ptr<media::VideoCaptureDevice::Client> client);

  
  
  void DoStopDeviceOnDeviceThread(DeviceEntry* entry);

  DeviceInfo* FindDeviceInfoById(const std::string& id,
                                 DeviceInfos& device_vector);

  
  scoped_refptr<base::MessageLoopProxy> device_loop_;

  
  MediaStreamProviderListener* listener_;
  int new_capture_session_id_;

  
  
  
  
  std::map<int, MediaStreamDevice> sessions_;

  
  
  
  
  
  
  
  
  
  struct DeviceEntry {
    DeviceEntry(MediaStreamType stream_type,
                const std::string& id,
                scoped_ptr<VideoCaptureController> controller);
    ~DeviceEntry();

    const MediaStreamType stream_type;
    const std::string id;

    
    scoped_ptr<VideoCaptureController> video_capture_controller;

    
    scoped_ptr<media::VideoCaptureDevice> video_capture_device;
  };
  typedef std::set<DeviceEntry*> DeviceEntries;
  DeviceEntries devices_;

  
  
  
  
  
  
  
  DeviceInfos devices_info_cache_;

  
  
  
  
  enum {
    DISABLED,
    TEST_PATTERN,
    Y4M_FILE
  } artificial_device_source_for_testing_;

  DISALLOW_COPY_AND_ASSIGN(VideoCaptureManager);
};

}  

#endif  
