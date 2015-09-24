// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_PEPPER_PLATFORM_AUDIO_INPUT_H_
#define CONTENT_RENDERER_PEPPER_PEPPER_PLATFORM_AUDIO_INPUT_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "media/audio/audio_input_ipc.h"
#include "media/audio/audio_parameters.h"

class GURL;

namespace base {
class MessageLoopProxy;
}

namespace media {
class AudioParameters;
}

namespace content {

class PepperAudioInputHost;
class PepperMediaDeviceManager;
class RenderViewImpl;


class PepperPlatformAudioInput
    : public media::AudioInputIPCDelegate,
      public base::RefCountedThreadSafe<PepperPlatformAudioInput> {
 public:
  
  
  static PepperPlatformAudioInput* Create(
      const base::WeakPtr<RenderViewImpl>& render_view,
      const std::string& device_id,
      const GURL& document_url,
      int sample_rate,
      int frames_per_buffer,
      PepperAudioInputHost* client);

  
  void StartCapture();
  void StopCapture();
  
  void ShutDown();

  
  virtual void OnStreamCreated(base::SharedMemoryHandle handle,
                               base::SyncSocket::Handle socket_handle,
                               int length,
                               int total_segments) OVERRIDE;
  virtual void OnVolume(double volume) OVERRIDE;
  virtual void OnStateChanged(
      media::AudioInputIPCDelegate::State state) OVERRIDE;
  virtual void OnIPCClosed() OVERRIDE;

 protected:
  virtual ~PepperPlatformAudioInput();

 private:
  friend class base::RefCountedThreadSafe<PepperPlatformAudioInput>;

  PepperPlatformAudioInput();

  bool Initialize(
      const base::WeakPtr<RenderViewImpl>& render_view,
      const std::string& device_id,
      const GURL& document_url,
      int sample_rate,
      int frames_per_buffer,
      PepperAudioInputHost* client);

  
  void InitializeOnIOThread(int session_id);
  void StartCaptureOnIOThread();
  void StopCaptureOnIOThread();
  void ShutDownOnIOThread();

  void OnDeviceOpened(int request_id,
                      bool succeeded,
                      const std::string& label);
  void CloseDevice();
  void NotifyStreamCreationFailed();

  PepperMediaDeviceManager* GetMediaDeviceManager();

  
  
  PepperAudioInputHost* client_;

  
  
  scoped_ptr<media::AudioInputIPC> ipc_;

  scoped_refptr<base::MessageLoopProxy> main_message_loop_proxy_;
  scoped_refptr<base::MessageLoopProxy> io_message_loop_proxy_;

  
  base::WeakPtr<RenderViewImpl> render_view_;

  
  
  std::string label_;

  
  media::AudioParameters params_;

  
  
  bool create_stream_sent_;

  
  
  
  bool pending_open_device_;
  
  int pending_open_device_id_;

  DISALLOW_COPY_AND_ASSIGN(PepperPlatformAudioInput);
};

}  

#endif  
