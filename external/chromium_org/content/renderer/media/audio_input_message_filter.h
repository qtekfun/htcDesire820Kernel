// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_AUDIO_INPUT_MESSAGE_FILTER_H_
#define CONTENT_RENDERER_MEDIA_AUDIO_INPUT_MESSAGE_FILTER_H_

#include "base/id_map.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/sync_socket.h"
#include "content/common/content_export.h"
#include "ipc/ipc_channel_proxy.h"
#include "media/audio/audio_input_ipc.h"

namespace base {
class MessageLoopProxy;
}

namespace content {

class CONTENT_EXPORT AudioInputMessageFilter
    : public IPC::ChannelProxy::MessageFilter {
 public:
  explicit AudioInputMessageFilter(
      const scoped_refptr<base::MessageLoopProxy>& io_message_loop);

  
  static AudioInputMessageFilter* Get();

  
  
  
  
  
  scoped_ptr<media::AudioInputIPC> CreateAudioInputIPC(int render_view_id);

  scoped_refptr<base::MessageLoopProxy> io_message_loop() const {
    return io_message_loop_;
  }

 private:
  
  
  class AudioInputIPCImpl;

  virtual ~AudioInputMessageFilter();

  
  void Send(IPC::Message* message);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnFilterAdded(IPC::Channel* channel) OVERRIDE;
  virtual void OnFilterRemoved() OVERRIDE;
  virtual void OnChannelClosing() OVERRIDE;

  
  void OnStreamCreated(int stream_id,
                       base::SharedMemoryHandle handle,
#if defined(OS_WIN)
                       base::SyncSocket::Handle socket_handle,
#else
                       base::FileDescriptor socket_descriptor,
#endif
                       uint32 length,
                       uint32 total_segments);

  
  void OnStreamVolume(int stream_id, double volume);

  
  
  void OnStreamStateChanged(int stream_id,
                            media::AudioInputIPCDelegate::State state);

  
  IDMap<media::AudioInputIPCDelegate> delegates_;

  
  IPC::Channel* channel_;

  
  const scoped_refptr<base::MessageLoopProxy> io_message_loop_;

  
  static AudioInputMessageFilter* g_filter;

  DISALLOW_COPY_AND_ASSIGN(AudioInputMessageFilter);
};

}  

#endif  
