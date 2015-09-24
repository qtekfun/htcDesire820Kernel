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

#ifndef TALK_BASE_OPENSSLSTREAMADAPTER_H__
#define TALK_BASE_OPENSSLSTREAMADAPTER_H__

#include <string>
#include "talk/base/sslstreamadapter.h"
#include "talk/base/opensslidentity.h"

typedef struct ssl_st SSL;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct x509_store_ctx_st X509_STORE_CTX;

namespace talk_base {

// This class was written with OpenSSLAdapter (a socket adapter) as a


class OpenSSLIdentity;


class OpenSSLStreamAdapter : public SSLStreamAdapter {
 public:
  explicit OpenSSLStreamAdapter(StreamInterface* stream);
  virtual ~OpenSSLStreamAdapter();

  virtual void SetIdentity(SSLIdentity* identity);
  virtual void SetServerRole();
  virtual void SetPeerCertificate(SSLCertificate* cert);

  virtual int StartSSLWithServer(const char* server_name);
  virtual int StartSSLWithPeer();

  virtual StreamResult Read(void* data, size_t data_len,
                            size_t* read, int* error);
  virtual StreamResult Write(const void* data, size_t data_len,
                             size_t* written, int* error);
  virtual void Close();
  virtual StreamState GetState() const;

 protected:
  virtual void OnEvent(StreamInterface* stream, int events, int err);

 private:
  enum SSLState {
    
    
    SSL_NONE,
    SSL_WAIT,  
    SSL_CONNECTING,  
    SSL_CONNECTED,  
    SSL_ERROR,  
    SSL_CLOSED  
  };
  enum SSLRole {
    SSL_CLIENT, SSL_SERVER
  };

  
  
  
  

  
  
  
  int StartSSL();
  
  int BeginSSL();
  
  int ContinueSSL();

  
  
  
  
  
  
  void Error(const char* context, int err, bool signal);
  void Cleanup();

  
  SSL_CTX* SetupSSLContext();
  
  bool SSLPostConnectionCheck(SSL* ssl, const char* server_name,
                              const X509* peer_cert);
  
  
  
  
  static int SSLVerifyCallback(int ok, X509_STORE_CTX* store);


  SSLState state_;
  SSLRole role_;
  int ssl_error_code_;  
  
  
  bool ssl_read_needs_write_;
  bool ssl_write_needs_read_;

  SSL* ssl_;
  SSL_CTX* ssl_ctx_;
  
  
  
  scoped_ptr<OpenSSLIdentity> identity_;
  std::string ssl_server_name_;
  
  
  scoped_ptr<OpenSSLCertificate> peer_certificate_;

  
  bool custom_verification_succeeded_;
};


}  

#endif  
