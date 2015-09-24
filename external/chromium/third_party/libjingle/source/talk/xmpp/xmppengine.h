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

#ifndef _xmppengine_h_
#define _xmppengine_h_

#include "talk/xmpp/jid.h"
#include "talk/xmllite/qname.h"
#include "talk/xmllite/xmlelement.h"


namespace buzz {

class XmppEngine;
class SaslHandler;
typedef void * XmppIqCookie;

enum XmppStanzaError {
  XSE_BAD_REQUEST,
  XSE_CONFLICT,
  XSE_FEATURE_NOT_IMPLEMENTED,
  XSE_FORBIDDEN,
  XSE_GONE,
  XSE_INTERNAL_SERVER_ERROR,
  XSE_ITEM_NOT_FOUND,
  XSE_JID_MALFORMED,
  XSE_NOT_ACCEPTABLE,
  XSE_NOT_ALLOWED,
  XSE_PAYMENT_REQUIRED,
  XSE_RECIPIENT_UNAVAILABLE,
  XSE_REDIRECT,
  XSE_REGISTRATION_REQUIRED,
  XSE_SERVER_NOT_FOUND,
  XSE_SERVER_TIMEOUT,
  XSE_RESOURCE_CONSTRAINT,
  XSE_SERVICE_UNAVAILABLE,
  XSE_SUBSCRIPTION_REQUIRED,
  XSE_UNDEFINED_CONDITION,
  XSE_UNEXPECTED_REQUEST,
};

enum XmppReturnStatus {
  XMPP_RETURN_OK,
  XMPP_RETURN_BADARGUMENT,
  XMPP_RETURN_BADSTATE,
  XMPP_RETURN_PENDING,
  XMPP_RETURN_UNEXPECTED,
  XMPP_RETURN_NOTYETIMPLEMENTED,
};

class XmppOutputHandler {
public:
  virtual ~XmppOutputHandler() {}

  
  virtual void WriteOutput(const char * bytes, size_t len) = 0;

  
  
  
  virtual void StartTls(const std::string & domainname) = 0;

  
  virtual void CloseConnection() = 0;
};

class XmppSessionHandler {
public:
  virtual ~XmppSessionHandler() {}
  
  virtual void OnStateChange(int state) = 0;
};

class XmppStanzaHandler {
public:
  virtual ~XmppStanzaHandler() {}
  
  
  
  
  virtual bool HandleStanza(const XmlElement * stanza) = 0;
};

class XmppIqHandler {
public:
  virtual ~XmppIqHandler() {}
  
  
  
  
  
  virtual void IqResponse(XmppIqCookie cookie, const XmlElement * pelStanza) = 0;
};

class XmppEngine {
public:
  static XmppEngine * Create();
  virtual ~XmppEngine() {}

  
  enum Error {
    ERROR_NONE = 0,         
    ERROR_XML,              
    ERROR_STREAM,           
    ERROR_VERSION,          
    ERROR_UNAUTHORIZED,     
    ERROR_TLS,              
    ERROR_AUTH,             
    ERROR_BIND,             
    ERROR_CONNECTION_CLOSED,
    ERROR_DOCUMENT_CLOSED,  
    ERROR_SOCKET,           
    ERROR_NETWORK_TIMEOUT,  
    ERROR_MISSING_USERNAME  
  };

  
  enum State {
    STATE_NONE = 0,        
    STATE_START,           
    STATE_OPENING,         
    STATE_OPEN,            
    STATE_CLOSED,          
  };

  

  
  virtual XmppReturnStatus SetOutputHandler(XmppOutputHandler *pxoh) = 0;

  
  virtual XmppReturnStatus HandleInput(const char * bytes, size_t len) = 0;

  
  virtual XmppReturnStatus ConnectionClosed(int subcode) = 0;

  

  
  virtual XmppReturnStatus SetUser(const Jid & jid)= 0;

  
  virtual const Jid & GetUser() = 0;

  
  
  virtual XmppReturnStatus SetSaslHandler(SaslHandler * h) = 0;

  
  virtual XmppReturnStatus SetUseTls(bool useTls) = 0;

  
  
  
  
  virtual XmppReturnStatus SetTlsServer(const std::string & proxy_hostname, 
                                        const std::string & proxy_domain) = 0;

  
  virtual bool GetUseTls() = 0;

  
  
  
  virtual XmppReturnStatus SetRequestedResource(const std::string& resource) = 0;

  
  virtual const std::string & GetRequestedResource() = 0;

  
  virtual void SetLanguage(const std::string & lang) = 0;

  

  
  virtual XmppReturnStatus SetSessionHandler(XmppSessionHandler* handler) = 0;

  
  
  
  virtual XmppReturnStatus Connect() = 0;

  
  virtual State GetState() = 0;

  
  virtual bool IsEncrypted() = 0;

  
  
  virtual Error GetError(int *subcode) = 0;

  
  
  
  virtual const XmlElement * GetStreamError() = 0;

  
  
  
  
  virtual XmppReturnStatus Disconnect() = 0;

  

  enum HandlerLevel {
    HL_NONE = 0,
    HL_PEEK,   
    HL_SINGLE, 
    HL_SENDER, 
    HL_TYPE,   
    HL_ALL,    
    HL_COUNT,  
  };

  
  
  
  virtual XmppReturnStatus AddStanzaHandler(XmppStanzaHandler* handler, HandlerLevel level = HL_PEEK) = 0;

  
  virtual XmppReturnStatus RemoveStanzaHandler(XmppStanzaHandler* handler) = 0;

  
  virtual XmppReturnStatus SendStanza(const XmlElement * pelStanza) = 0;

  
  virtual XmppReturnStatus SendRaw(const std::string & text) = 0;

  
  
  virtual XmppReturnStatus SendIq(const XmlElement* pelStanza,
                                  XmppIqHandler* iq_handler,
                                  XmppIqCookie* cookie) = 0;

  
  
  virtual XmppReturnStatus RemoveIqHandler(XmppIqCookie cookie,
                                      XmppIqHandler** iq_handler) = 0;


  
  
  
  virtual XmppReturnStatus SendStanzaError(const XmlElement * pelOriginal,
                                           XmppStanzaError code,
                                           const std::string & text) = 0;

  
  
  
  virtual const Jid & FullJid() = 0;

  
  
  
  virtual std::string NextId() = 0;

};

}



#define XMPP_FAILED(x)                      \
  ( (x) == buzz::XMPP_RETURN_OK ? false : true)   \


#define XMPP_SUCCEEDED(x)                   \
  ( (x) == buzz::XMPP_RETURN_OK ? true : false)   \

#define IFR(x)                        \
  do {                                \
    xmpp_status = (x);                \
    if (XMPP_FAILED(xmpp_status)) {   \
      return xmpp_status;             \
    }                                 \
  } while (false)                     \


#define IFC(x)                        \
  do {                                \
    xmpp_status = (x);                \
    if (XMPP_FAILED(xmpp_status)) {   \
      goto Cleanup;                   \
    }                                 \
  } while (false)                     \


#endif
