/*
 * libjingle
 * Copyright 2004--2008, Google Inc.
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

#ifndef TALK_BASE_SSLSTREAMADAPTER_H__
#define TALK_BASE_SSLSTREAMADAPTER_H__

#include "talk/base/stream.h"
#include "talk/base/sslidentity.h"

namespace talk_base {

// This class was written with SSLAdapter as a starting point. It

class SSLStreamAdapter : public StreamAdapterInterface {
 public:
  
  
  
  static SSLStreamAdapter* Create(StreamInterface* stream);

  explicit SSLStreamAdapter(StreamInterface* stream)
      : StreamAdapterInterface(stream), ignore_bad_cert_(false) { }

  void set_ignore_bad_cert(bool ignore) { ignore_bad_cert_ = ignore; }
  bool ignore_bad_cert() const { return ignore_bad_cert_; }

  
  
  
  
  
  
  virtual void SetIdentity(SSLIdentity* identity) = 0;

  
  
  virtual void SetServerRole() = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  
  
  virtual int StartSSLWithServer(const char* server_name) = 0;

  
  
  
  
  
  
  
  virtual int StartSSLWithPeer() = 0;

  
  
  
  
  
  
  
  
  
  virtual void SetPeerCertificate(SSLCertificate* cert) = 0;

  
  
  
  bool ignore_bad_cert_;
};

}  

#endif  
