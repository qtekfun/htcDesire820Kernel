// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_WEBRTC_AUDIO_DEVICE_TEST_H_
#define CONTENT_TEST_WEBRTC_AUDIO_DEVICE_TEST_H_

#include <string>

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "content/browser/renderer_host/media/mock_media_observer.h"
#include "content/public/renderer/content_renderer_client.h"
#include "ipc/ipc_listener.h"
#include "media/base/audio_hardware_config.h"
#include "media/base/channel_layout.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/webrtc/common_types.h"

namespace IPC {
class Channel;
}

namespace media {
class AudioManager;
}

namespace net {
class URLRequestContext;
}

namespace webrtc {
class VoENetwork;
}

#if defined(OS_WIN)
namespace base {
namespace win {
class ScopedCOMInitializer;
}
}
#endif

namespace content {

class AudioMirroringManager;
class ContentRendererClient;
class MediaStreamManager;
class RenderThreadImpl;
class ResourceContext;
class TestAudioInputRendererHost;
class TestAudioRendererHost;
class TestBrowserThread;
class WebRtcAudioRenderer;
class WebRTCMockRenderProcess;

template<typename T>
class ScopedWebRTCPtr {
 public:
  template<typename Engine>
  explicit ScopedWebRTCPtr(Engine* e)
      : ptr_(T::GetInterface(e)) {}
  explicit ScopedWebRTCPtr(T* p) : ptr_(p) {}
  ~ScopedWebRTCPtr() { reset(); }
  T* operator->() const { return ptr_; }
  T* get() const { return ptr_; }

  
  void reset() {
    if (ptr_) {
      ptr_->Release();
      ptr_ = NULL;
    }
  }

  bool valid() const { return ptr_ != NULL; }

 private:
  T* ptr_;
};

template <typename T>
class WebRTCAutoDelete {
 public:
  WebRTCAutoDelete() : ptr_(NULL) {}
  explicit WebRTCAutoDelete(T* ptr) : ptr_(ptr) {}
  ~WebRTCAutoDelete() { reset(); }

  void reset() {
    if (ptr_) {
      T::Delete(ptr_);
      ptr_ = NULL;
    }
  }

  T* operator->() { return ptr_; }
  T* get() const { return ptr_; }

  bool valid() const { return ptr_ != NULL; }

 protected:
  T* ptr_;
};

class ReplaceContentClientRenderer;

#define MAYBE_WebRTCAudioDeviceTest DISABLED_WebRTCAudioDeviceTest

class MAYBE_WebRTCAudioDeviceTest : public ::testing::Test,
                                    public IPC::Listener {
 public:
  MAYBE_WebRTCAudioDeviceTest();
  virtual ~MAYBE_WebRTCAudioDeviceTest();

  virtual void SetUp() OVERRIDE;
  virtual void TearDown() OVERRIDE;

  
  bool Send(IPC::Message* message);

  void SetAudioHardwareConfig(media::AudioHardwareConfig* hardware_config);

  scoped_refptr<WebRtcAudioRenderer> CreateDefaultWebRtcAudioRenderer(
      int render_view_id);

 protected:
  void InitializeIOThread(const char* thread_name);
  void UninitializeIOThread();
  void CreateChannel(const char* name);
  void DestroyChannel();

  void OnGetAudioHardwareConfig(media::AudioParameters* input_params,
                                media::AudioParameters* output_params);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  void WaitForIOThreadCompletion();
  void WaitForAudioManagerCompletion();
  void WaitForMessageLoopCompletion(base::MessageLoopProxy* loop);

  std::string GetTestDataPath(const base::FilePath::StringType& file_name);

  scoped_ptr<ReplaceContentClientRenderer> saved_content_renderer_;
  base::MessageLoopForUI message_loop_;
  ContentRendererClient content_renderer_client_;
  RenderThreadImpl* render_thread_;  
  scoped_ptr<WebRTCMockRenderProcess> mock_process_;
  scoped_ptr<MediaStreamManager> media_stream_manager_;
  scoped_ptr<media::AudioManager> audio_manager_;
  scoped_ptr<AudioMirroringManager> mirroring_manager_;
  scoped_ptr<net::URLRequestContext> test_request_context_;
  scoped_ptr<ResourceContext> resource_context_;
  scoped_ptr<IPC::Channel> channel_;
  scoped_refptr<TestAudioRendererHost> audio_render_host_;
  scoped_refptr<TestAudioInputRendererHost> audio_input_renderer_host_;

  media::AudioHardwareConfig* audio_hardware_config_;  

  
  scoped_ptr<TestBrowserThread> ui_thread_;
  
  scoped_ptr<TestBrowserThread> io_thread_;

#if defined(OS_WIN)
  
  scoped_ptr<base::win::ScopedCOMInitializer> initialize_com_;
#endif

  
  bool has_input_devices_;
  bool has_output_devices_;

  
  
  bool sandbox_was_enabled_;
};

class WebRTCTransportImpl : public webrtc::Transport {
 public:
  explicit WebRTCTransportImpl(webrtc::VoENetwork* network);
  virtual ~WebRTCTransportImpl();

  virtual int SendPacket(int channel, const void* data, int len) OVERRIDE;
  virtual int SendRTCPPacket(int channel, const void* data, int len) OVERRIDE;

 private:
  webrtc::VoENetwork* network_;
};

}  

#endif  
