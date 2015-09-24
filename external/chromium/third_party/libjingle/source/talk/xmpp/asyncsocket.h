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

#ifndef _ASYNCSOCKET_H_
#define _ASYNCSOCKET_H_

#include "talk/base/sigslot.h"

namespace talk_base {
  class SocketAddress;
}

namespace buzz {

class AsyncSocket {
public:
  enum State {
    STATE_CLOSED = 0,      
    STATE_CLOSING,         
    STATE_CONNECTING,      
    STATE_OPEN,            
#if defined(FEATURE_ENABLE_SSL)
    STATE_TLS_CONNECTING,  
    STATE_TLS_OPEN,        
#endif
  };

  enum Error {
    ERROR_NONE = 0,         
    ERROR_WINSOCK,          
    ERROR_DNS,              
    ERROR_WRONGSTATE,       
#if defined(FEATURE_ENABLE_SSL)
    ERROR_SSL,              
#endif
  };

  virtual ~AsyncSocket() {}
  virtual State state() = 0;
  virtual Error error() = 0;
  virtual int GetError() = 0;    

  virtual bool Connect(const talk_base::SocketAddress& addr) = 0;
  virtual bool Read(char * data, size_t len, size_t* len_read) = 0;
  virtual bool Write(const char * data, size_t len) = 0;
  virtual bool Close() = 0;
#if defined(FEATURE_ENABLE_SSL)
  
  
  virtual bool StartTls(const std::string & domainname) = 0;
#endif

  sigslot::signal0<> SignalConnected;
  sigslot::signal0<> SignalSSLConnected;
  sigslot::signal0<> SignalClosed;
  sigslot::signal0<> SignalRead;
  sigslot::signal0<> SignalError;
};

}

#endif
