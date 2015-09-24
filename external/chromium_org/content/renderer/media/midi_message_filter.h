// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_MIDI_MESSAGE_FILTER_H_
#define CONTENT_RENDERER_MEDIA_MIDI_MESSAGE_FILTER_H_

#include <map>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"
#include "ipc/ipc_channel_proxy.h"
#include "media/midi/midi_port_info.h"
#include "third_party/WebKit/public/platform/WebMIDIAccessorClient.h"

namespace base {
class MessageLoopProxy;
}

namespace content {

class CONTENT_EXPORT MIDIMessageFilter
    : public IPC::ChannelProxy::MessageFilter {
 public:
  explicit MIDIMessageFilter(
      const scoped_refptr<base::MessageLoopProxy>& io_message_loop);

  
  
  
  
  void StartSession(blink::WebMIDIAccessorClient* client);
  void RemoveClient(blink::WebMIDIAccessorClient* client);

  
  
  void SendMIDIData(uint32 port,
                    const uint8* data,
                    size_t length,
                    double timestamp);

  
  scoped_refptr<base::MessageLoopProxy> io_message_loop() const {
    return io_message_loop_;
  }

 protected:
  virtual ~MIDIMessageFilter();

 private:
  
  void Send(IPC::Message* message);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnFilterAdded(IPC::Channel* channel) OVERRIDE;
  virtual void OnFilterRemoved() OVERRIDE;
  virtual void OnChannelClosing() OVERRIDE;

  
  
  void OnSessionStarted(int client_id,
                        bool success,
                        media::MIDIPortInfoList inputs,
                        media::MIDIPortInfoList outputs);

  
  
  void OnDataReceived(uint32 port,
                      const std::vector<uint8>& data,
                      double timestamp);

  
  
  
  void OnAcknowledgeSentData(size_t bytes_sent);

  void HandleSessionStarted(int client_id,
                            bool success,
                            media::MIDIPortInfoList inputs,
                            media::MIDIPortInfoList outputs);

  void HandleDataReceived(uint32 port,
                          const std::vector<uint8>& data,
                          double timestamp);

  void StartSessionOnIOThread(int client_id);

  void SendMIDIDataOnIOThread(uint32 port,
                              const std::vector<uint8>& data,
                              double timestamp);

  blink::WebMIDIAccessorClient* GetClientFromId(int client_id);

  
  IPC::Channel* channel_;

  
  const scoped_refptr<base::MessageLoopProxy> io_message_loop_;

  
  scoped_refptr<base::MessageLoopProxy> main_message_loop_;

  
  
  
  
  typedef std::map<blink::WebMIDIAccessorClient*, int> ClientsMap;
  ClientsMap clients_;

  
  int next_available_id_;

  size_t unacknowledged_bytes_sent_;

  DISALLOW_COPY_AND_ASSIGN(MIDIMessageFilter);
};

}  

#endif  
