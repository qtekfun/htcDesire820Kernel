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

#ifndef TALK_P2P_BASE_SESSIONMANAGER_H_
#define TALK_P2P_BASE_SESSIONMANAGER_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "talk/base/sigslot.h"
#include "talk/base/thread.h"
#include "talk/p2p/base/portallocator.h"

namespace buzz {
class QName;
class XmlElement;
}

namespace cricket {

class Session;
class BaseSession;
class SessionClient;


class SessionManager : public sigslot::has_slots<> {
 public:
  SessionManager(PortAllocator *allocator,
                 talk_base::Thread *worker_thread = NULL);
  virtual ~SessionManager();

  PortAllocator *port_allocator() const { return allocator_; }
  talk_base::Thread *worker_thread() const { return worker_thread_; }
  talk_base::Thread *signaling_thread() const { return signaling_thread_; }

  int session_timeout() const { return timeout_; }
  void set_session_timeout(int timeout) { timeout_ = timeout; }

  
  
  
  
  void AddClient(const std::string& content_type, SessionClient* client);
  void RemoveClient(const std::string& content_type);
  SessionClient* GetClient(const std::string& content_type);

  
  
  Session *CreateSession(const std::string& local_name,
                         const std::string& content_type);

  
  void DestroySession(Session *session);

  
  Session *GetSession(const std::string& sid);

  
  void TerminateAll();

  
  sigslot::signal2<Session *, bool> SignalSessionCreate;
  sigslot::signal1<Session *> SignalSessionDestroy;

  
  bool IsSessionMessage(const buzz::XmlElement* stanza);

  
  Session* FindSession(const std::string& sid,
                       const std::string& remote_name);

  
  void OnIncomingMessage(const buzz::XmlElement* stanza);

  
  void OnIncomingResponse(const buzz::XmlElement* orig_stanza,
                          const buzz::XmlElement* response_stanza);

  
  
  void OnFailedSend(const buzz::XmlElement* orig_stanza,
                    const buzz::XmlElement* error_stanza);

  
  
  sigslot::signal2<SessionManager*,
                   const buzz::XmlElement*> SignalOutgoingMessage;

  
  
  
  
  
  
  
  
  sigslot::signal0<> SignalRequestSignaling;
  void OnSignalingReady();

 private:
  typedef std::map<std::string, Session*> SessionMap;
  typedef std::map<std::string, SessionClient*> ClientMap;

  PortAllocator *allocator_;
  talk_base::Thread *signaling_thread_;
  talk_base::Thread *worker_thread_;
  int timeout_;
  SessionMap session_map_;
  ClientMap client_map_;

  
  
  Session *CreateSession(const std::string& local_name,
                         const std::string& initiator,
                         const std::string& sid,
                         const std::string& content_type,
                         bool received_initiate);

  
  
  
  std::string FindClient(const buzz::XmlElement* session);

  
  
  
  
  void SendErrorMessage(const buzz::XmlElement* stanza,
                        const buzz::QName& name,
                        const std::string& type,
                        const std::string& text,
                        const buzz::XmlElement* extra_info);

  
  
  buzz::XmlElement* CreateErrorMessage(
      const buzz::XmlElement* stanza,
      const buzz::QName& name,
      const std::string& type,
      const std::string& text,
      const buzz::XmlElement* extra_info);

  
  void OnRequestSignaling(Session* session);

  
  void OnOutgoingMessage(Session* session, const buzz::XmlElement* stanza);

  
  void OnErrorMessage(BaseSession* session,
                      const buzz::XmlElement* stanza,
                      const buzz::QName& name,
                      const std::string& type,
                      const std::string& text,
                      const buzz::XmlElement* extra_info);
};

}  

#endif  
