// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_CHANNEL_MULTIPLEXER_H_
#define REMOTING_PROTOCOL_CHANNEL_MULTIPLEXER_H_

#include "base/memory/weak_ptr.h"
#include "remoting/proto/mux.pb.h"
#include "remoting/protocol/buffered_socket_writer.h"
#include "remoting/protocol/channel_factory.h"
#include "remoting/protocol/message_reader.h"

namespace remoting {
namespace protocol {

class ChannelMultiplexer : public ChannelFactory {
 public:
  static const char kMuxChannelName[];

  
  ChannelMultiplexer(ChannelFactory* factory,
                     const std::string& base_channel_name);
  virtual ~ChannelMultiplexer();

  
  virtual void CreateStreamChannel(
      const std::string& name,
      const StreamChannelCallback& callback) OVERRIDE;
  virtual void CreateDatagramChannel(
      const std::string& name,
      const DatagramChannelCallback& callback) OVERRIDE;
  virtual void CancelChannelCreation(const std::string& name) OVERRIDE;

 private:
  struct PendingChannel;
  class MuxChannel;
  class MuxSocket;
  friend class MuxChannel;

  
  void OnBaseChannelReady(scoped_ptr<net::StreamSocket> socket);

  
  void DoCreatePendingChannels();

  
  MuxChannel* GetOrCreateChannel(const std::string& name);

  
  void OnWriteFailed(int error);

  
  void NotifyWriteFailed(const std::string& name);

  
  void OnIncomingPacket(scoped_ptr<MultiplexPacket> packet,
                        const base::Closure& done_task);

  
  bool DoWrite(scoped_ptr<MultiplexPacket> packet,
               const base::Closure& done_task);

  
  
  ChannelFactory* base_channel_factory_;

  
  std::string base_channel_name_;

  
  scoped_ptr<net::StreamSocket> base_channel_;

  
  std::list<PendingChannel> pending_channels_;

  int next_channel_id_;
  std::map<std::string, MuxChannel*> channels_;

  
  
  std::map<int, MuxChannel*> channels_by_receive_id_;

  BufferedSocketWriter writer_;
  ProtobufMessageReader<MultiplexPacket> reader_;

  base::WeakPtrFactory<ChannelMultiplexer> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(ChannelMultiplexer);
};

}  
}  


#endif  
