// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_MEDIA_AUDIO_INPUT_DEVICE_MANAGER_H_
#define CONTENT_BROWSER_RENDERER_HOST_MEDIA_AUDIO_INPUT_DEVICE_MANAGER_H_

#include <map>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/threading/thread.h"
#include "content/browser/renderer_host/media/media_stream_provider.h"
#include "content/common/content_export.h"
#include "content/common/media/media_stream_options.h"
#include "content/public/common/media_stream_request.h"

namespace media {
class AudioManager;
}

namespace content {

class CONTENT_EXPORT AudioInputDeviceManager : public MediaStreamProvider {
 public:
  
  
  
  
  static const int kFakeOpenSessionId;

  explicit AudioInputDeviceManager(media::AudioManager* audio_manager);

  
  
  const StreamDeviceInfo* GetOpenedDeviceInfoById(int session_id);

  
  virtual void Register(MediaStreamProviderListener* listener,
                        base::MessageLoopProxy* device_thread_loop) OVERRIDE;
  virtual void Unregister() OVERRIDE;
  virtual void EnumerateDevices(MediaStreamType stream_type) OVERRIDE;
  virtual int Open(const StreamDeviceInfo& device) OVERRIDE;
  virtual void Close(int session_id) OVERRIDE;

  void UseFakeDevice();
  bool ShouldUseFakeDevice() const;

 private:
  typedef std::vector<StreamDeviceInfo> StreamDeviceList;
  virtual ~AudioInputDeviceManager();

  
  void EnumerateOnDeviceThread(MediaStreamType stream_type);
  
  void OpenOnDeviceThread(int session_id, const StreamDeviceInfo& info);

  
  
  void DevicesEnumeratedOnIOThread(MediaStreamType stream_type,
                                   scoped_ptr<StreamDeviceInfoArray> devices);
  
  
  void OpenedOnIOThread(int session_id, const StreamDeviceInfo& info);
  
  
  void ClosedOnIOThread(MediaStreamType type, int session_id);

  
  bool IsOnDeviceThread() const;

  
  
  StreamDeviceList::iterator GetDevice(int session_id);

  
  MediaStreamProviderListener* listener_;
  int next_capture_session_id_;
  bool use_fake_device_;
  StreamDeviceList devices_;

  media::AudioManager* const audio_manager_;  

  
  scoped_refptr<base::MessageLoopProxy> device_loop_;

  DISALLOW_COPY_AND_ASSIGN(AudioInputDeviceManager);
};

}  

#endif  
