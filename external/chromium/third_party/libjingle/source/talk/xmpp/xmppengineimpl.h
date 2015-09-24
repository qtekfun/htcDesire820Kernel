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

#ifndef _xmppengineimpl_h_
#define _xmppengineimpl_h_

#include <sstream>
#include <vector>
#include "talk/xmpp/xmppengine.h"
#include "talk/xmpp/xmppstanzaparser.h"

namespace buzz {

class XmppLoginTask;
class XmppEngine;
class XmppIqEntry;
class SaslHandler;
class SaslMechanism;


class XmppEngineImpl : public XmppEngine {
public:
  XmppEngineImpl();
  virtual ~XmppEngineImpl();

  

  
  virtual XmppReturnStatus SetOutputHandler(XmppOutputHandler *pxoh);

  
  virtual XmppReturnStatus HandleInput(const char * bytes, size_t len);

  
  virtual XmppReturnStatus ConnectionClosed(int subcode);

  

  
  virtual XmppReturnStatus SetUser(const Jid & jid);

  
  virtual const Jid & GetUser();

  
  virtual XmppReturnStatus SetSaslHandler(SaslHandler * sasl_handler);

  
  virtual XmppReturnStatus SetUseTls(bool useTls);

  
  
  
  
  virtual XmppReturnStatus SetTlsServer(const std::string & proxy_hostname,
                                        const std::string & proxy_domain);

  
  virtual bool GetUseTls();

  
  
  
  virtual XmppReturnStatus SetRequestedResource(const std::string& resource);

  
  virtual const std::string & GetRequestedResource();

  
  virtual void SetLanguage(const std::string & lang) {
    lang_ = lang;
  }

  

  
  virtual XmppReturnStatus SetSessionHandler(XmppSessionHandler* handler);

  
  
  
  virtual XmppReturnStatus Connect();

  
  virtual State GetState() { return state_; }

  
  virtual bool IsEncrypted() { return encrypted_; }

  
  
  virtual Error GetError(int *subcode) {
     if (subcode) {
       *subcode = subcode_;
     }
     return error_code_;
  }

  
  
  
  virtual const XmlElement * GetStreamError() { return stream_error_.get(); }

  
  
  
  
  virtual XmppReturnStatus Disconnect();

  

  
  
  
  virtual XmppReturnStatus AddStanzaHandler(XmppStanzaHandler* handler,
                                            XmppEngine::HandlerLevel level);

  
  virtual XmppReturnStatus RemoveStanzaHandler(XmppStanzaHandler* handler);

  
  virtual XmppReturnStatus SendStanza(const XmlElement * pelStanza);

  
  virtual XmppReturnStatus SendRaw(const std::string & text);

  
  
  virtual XmppReturnStatus SendIq(const XmlElement* pelStanza,
                                  XmppIqHandler* iq_handler,
                                  XmppIqCookie* cookie);

  
  
  virtual XmppReturnStatus RemoveIqHandler(XmppIqCookie cookie,
                                      XmppIqHandler** iq_handler);

  
  
  
  virtual XmppReturnStatus SendStanzaError(const XmlElement * pelOriginal,
                                           XmppStanzaError code,
                                           const std::string & text);

  
  
  
  virtual const Jid & FullJid() { return bound_jid_; }

  
  
  
  virtual std::string NextId();

private:
  friend class XmppLoginTask;
  friend class XmppIqEntry;

  void IncomingStanza(const XmlElement *pelStanza);
  void IncomingStart(const XmlElement *pelStanza);
  void IncomingEnd(bool isError);

  void InternalSendStart(const std::string & domainName);
  void InternalSendStanza(const XmlElement * pelStanza);
  std::string ChooseBestSaslMechanism(const std::vector<std::string> & mechanisms, bool encrypted);
  SaslMechanism * GetSaslMechanism(const std::string & name);
  void SignalBound(const Jid & fullJid);
  void SignalStreamError(const XmlElement * pelStreamError);
  void SignalError(Error errorCode, int subCode);
  bool HasError();
  void DeleteIqCookies();
  bool HandleIqResponse(const XmlElement * element);
  void StartTls(const std::string & domain);
  void RaiseReset() { raised_reset_ = true; }

  class StanzaParseHandler : public XmppStanzaParseHandler {
  public:
    StanzaParseHandler(XmppEngineImpl * outer) : outer_(outer) {}
    virtual ~StanzaParseHandler() {}
    virtual void StartStream(const XmlElement * pelStream)
      { outer_->IncomingStart(pelStream); }
    virtual void Stanza(const XmlElement * pelStanza)
      { outer_->IncomingStanza(pelStanza); }
    virtual void EndStream()
      { outer_->IncomingEnd(false); }
    virtual void XmlError()
      { outer_->IncomingEnd(true); }
  private:
    XmppEngineImpl * const outer_;
  };

  class EnterExit {
   public:
    EnterExit(XmppEngineImpl* engine);
    ~EnterExit();
   private:
    XmppEngineImpl* engine_;
    State state_;
    Error error_;

  };

  friend class StanzaParseHandler;
  friend class EnterExit;

  StanzaParseHandler stanzaParseHandler_;
  XmppStanzaParser stanzaParser_;


  
  int engine_entered_;
  Jid user_jid_;
  std::string password_;
  std::string requested_resource_;
  bool tls_needed_;
  std::string tls_server_hostname_;
  std::string tls_server_domain_;
  talk_base::scoped_ptr<XmppLoginTask> login_task_;
  std::string lang_;

  int next_id_;
  Jid bound_jid_;
  State state_;
  bool encrypted_;
  Error error_code_;
  int subcode_;
  talk_base::scoped_ptr<XmlElement> stream_error_;
  bool raised_reset_;
  XmppOutputHandler* output_handler_;
  XmppSessionHandler* session_handler_;

  typedef std::vector<XmppStanzaHandler*> StanzaHandlerVector;
  talk_base::scoped_ptr<StanzaHandlerVector> stanza_handlers_[HL_COUNT];

  typedef std::vector<XmppIqEntry*> IqEntryVector;
  talk_base::scoped_ptr<IqEntryVector> iq_entries_;

  talk_base::scoped_ptr<SaslHandler> sasl_handler_;

  talk_base::scoped_ptr<std::stringstream> output_;
};

}


#endif
