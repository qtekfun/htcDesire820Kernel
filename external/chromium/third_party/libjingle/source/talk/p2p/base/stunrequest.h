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

#ifndef TALK_P2P_BASE_STUNREQUEST_H_
#define TALK_P2P_BASE_STUNREQUEST_H_

#include "talk/base/sigslot.h"
#include "talk/base/thread.h"
#include "talk/p2p/base/stun.h"
#include <map>
#include <string>

namespace cricket {

class StunRequest;

class StunRequestManager {
public:
  StunRequestManager(talk_base::Thread* thread);
  ~StunRequestManager();

  
  void Send(StunRequest* request);
  void SendDelayed(StunRequest* request, int delay);

  
  
  void Remove(StunRequest* request);

  
  void Clear();

  
  
  bool CheckResponse(StunMessage* msg);
  bool CheckResponse(const char* data, size_t size);

  
  sigslot::signal3<const void*, size_t, StunRequest*> SignalSendPacket;

private:
  typedef std::map<std::string, StunRequest*> RequestMap;

  talk_base::Thread* thread_;
  RequestMap requests_;

  friend class StunRequest;
};

class StunRequest : public talk_base::MessageHandler {
public:
  StunRequest();
  StunRequest(StunMessage* request);
  virtual ~StunRequest();

  
  void Construct();

  
  StunRequestManager* manager() { return manager_; }

  
  const std::string& id() { return id_; }

  
  StunMessageType type();

  
  void OnMessage(talk_base::Message* pmsg);

  
  uint32 Elapsed() const;

protected:
  int count_;
  bool timeout_;

  
  
  virtual void Prepare(StunMessage* request) {}

  
  virtual void OnResponse(StunMessage* response) {}
  virtual void OnErrorResponse(StunMessage* response) {}
  virtual void OnTimeout() {}
  virtual int GetNextDelay();

private:
  StunRequestManager* manager_;
  std::string id_;
  StunMessage* msg_;
  uint32 tstamp_;

  void set_manager(StunRequestManager* manager);

  friend class StunRequestManager;
};

}  

#endif  
