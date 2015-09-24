// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_AUDIO_MESSAGE_FILTER_H_
#define CONTENT_RENDERER_MEDIA_AUDIO_MESSAGE_FILTER_H_

#include "base/gtest_prod_util.h"
#include "base/id_map.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/sync_socket.h"
#include "base/synchronization/lock.h"
#include "content/common/content_export.h"
#include "ipc/ipc_channel_proxy.h"
#include "media/audio/audio_output_ipc.h"
#include "media/base/audio_hardware_config.h"

namespace base {
class MessageLoopProxy;
}

namespace content {

class CONTENT_EXPORT AudioMessageFilter
    : public IPC::ChannelProxy::MessageFilter {
 public:
  explicit AudioMessageFilter(
      const scoped_refptr<base::MessageLoopProxy>& io_message_loop);

  
  static AudioMessageFilter* Get();

  
  
  
  
  
  scoped_ptr<media::AudioOutputIPC> CreateAudioOutputIPC(int render_view_id);

  
  
  
  void SetAudioHardwareConfig(media::AudioHardwareConfig* config);

  
  scoped_refptr<base::MessageLoopProxy> io_message_loop() const {
    return io_message_loop_;
  }

 protected:
  virtual ~AudioMessageFilter();

 private:
  FRIEND_TEST_ALL_PREFIXES(AudioMessageFilterTest, Basic);
  FRIEND_TEST_ALL_PREFIXES(AudioMessageFilterTest, Delegates);

  
  
  class AudioOutputIPCImpl;

  
  void Send(IPC::Message* message);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnFilterAdded(IPC::Channel* channel) OVERRIDE;
  virtual void OnFilterRemoved() OVERRIDE;
  virtual void OnChannelClosing() OVERRIDE;

  
  void OnStreamCreated(int stream_id, base::SharedMemoryHandle handle,
#if defined(OS_WIN)
                       base::SyncSocket::Handle socket_handle,
#else
                       base::FileDescriptor socket_descriptor,
#endif
                       uint32 length);

  
  
  void OnStreamStateChanged(int stream_id,
                            media::AudioOutputIPCDelegate::State state);

  
  void OnOutputDeviceChanged(int stream_id, int new_buffer_size,
                             int new_sample_rate);

  
  IPC::Channel* channel_;

  
  
  IDMap<media::AudioOutputIPCDelegate> delegates_;

  
  
  base::Lock lock_;
  media::AudioHardwareConfig* audio_hardware_config_;

  
  const scoped_refptr<base::MessageLoopProxy> io_message_loop_;

  
  static AudioMessageFilter* g_filter;

  DISALLOW_COPY_AND_ASSIGN(AudioMessageFilter);
};

}  

#endif  
