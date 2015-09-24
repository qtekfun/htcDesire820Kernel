/*
 * libjingle
 * Copyright 2004--2005, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TALK_P2P_BASE_TRANSPORTCHANNEL_H_
#define TALK_P2P_BASE_TRANSPORTCHANNEL_H_

#include <string>
#include "talk/base/basictypes.h"
#include "talk/base/sigslot.h"
#include "talk/base/socket.h"

namespace cricket {

class P2PTransportChannel;

class TransportChannel: public sigslot::has_slots<> {
 public:
  TransportChannel(const std::string& name, const std::string &content_type)
      : name_(name), content_type_(content_type),
        readable_(false), writable_(false) {}
  virtual ~TransportChannel() {}

  
  const std::string& name() const { return name_; }
  const std::string& content_type() const { return content_type_; }

  
  
  
  bool readable() const { return readable_; }
  bool writable() const { return writable_; }
  sigslot::signal1<TransportChannel*> SignalReadableState;
  sigslot::signal1<TransportChannel*> SignalWritableState;

  
  virtual int SendPacket(const char *data, size_t len) = 0;

  
  
  virtual int SetOption(talk_base::Socket::Option opt, int value) = 0;

  
  virtual int GetError() = 0;

  
  
  
  virtual P2PTransportChannel* GetP2PChannel() { return NULL; }

  
  sigslot::signal3<TransportChannel*, const char*, size_t> SignalReadPacket;

  
  
  
  
  sigslot::signal2<TransportChannel*, const talk_base::SocketAddress&>
      SignalRouteChange;

  
  sigslot::signal1<TransportChannel*> SignalDestroyed;

  
  std::string ToString() const;

 protected:
  
  void set_readable(bool readable);

  
  void set_writable(bool writable);

 private:
  std::string name_;
  std::string content_type_;
  bool readable_;
  bool writable_;

  DISALLOW_EVIL_CONSTRUCTORS(TransportChannel);
};

}  

#endif  
