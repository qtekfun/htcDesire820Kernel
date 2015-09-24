/*
 * libjingle
 * Copyright 2009, Google Inc.
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

#ifndef TALK_SESSION_PHONE_SRTPFILTER_H_
#define TALK_SESSION_PHONE_SRTPFILTER_H_

#include <list>
#include <string>
#include <vector>
#include "talk/base/basictypes.h"
#include "talk/session/phone/cryptoparams.h"
#include "talk/p2p/base/sessiondescription.h"

struct srtp_event_data_t;
struct srtp_ctx_t;
typedef srtp_ctx_t* srtp_t;
struct srtp_policy_t;

namespace cricket {

extern const std::string& CS_DEFAULT;
extern const std::string CS_AES_CM_128_HMAC_SHA1_80;
extern const std::string CS_AES_CM_128_HMAC_SHA1_32;
extern const int SRTP_MASTER_KEY_BASE64_LEN;

class SrtpSession {
 public:
  SrtpSession();
  ~SrtpSession();

  
  
  bool SetSend(const std::string& cs, const uint8* key, int len);
  
  
  bool SetRecv(const std::string& cs, const uint8* key, int len);

  
  
  bool ProtectRtp(void* data, int in_len, int max_len, int* out_len);
  bool ProtectRtcp(void* data, int in_len, int max_len, int* out_len);
  
  
  bool UnprotectRtp(void* data, int in_len, int* out_len);
  bool UnprotectRtcp(void* data, int in_len, int* out_len);

 private:
  bool SetKey(int type, const std::string& cs, const uint8* key, int len);
  static bool Init();
  void HandleEvent(const srtp_event_data_t* ev);
  static void HandleEventThunk(srtp_event_data_t* ev);

  srtp_t session_;
  int rtp_auth_tag_len_;
  int rtcp_auth_tag_len_;
  static bool inited_;
  static std::list<SrtpSession*> sessions_;
};

class SrtpFilter {
 public:
  SrtpFilter();
  ~SrtpFilter();

  
  bool IsActive() const;

  
  
  
  bool SetOffer(const std::vector<CryptoParams>& offer_params,
                ContentSource source);
  
  
  
  
  
  bool SetAnswer(const std::vector<CryptoParams>& answer_params,
                 ContentSource source);

  
  
  bool ProtectRtp(void* data, int in_len, int max_len, int* out_len);
  bool ProtectRtcp(void* data, int in_len, int max_len, int* out_len);
  
  
  bool UnprotectRtp(void* data, int in_len, int* out_len);
  bool UnprotectRtcp(void* data, int in_len, int* out_len);

 protected:
  bool StoreParams(const std::vector<CryptoParams>& offer_params,
                   ContentSource source);
  bool NegotiateParams(const std::vector<CryptoParams>& answer_params,
                       CryptoParams* selected_params);
  bool ApplyParams(const CryptoParams& send_params,
                   const CryptoParams& recv_params);
  bool ResetParams();
  static bool ParseKeyParams(const std::string& params, uint8* key, int len);

 private:
  enum State { ST_INIT, ST_SENTOFFER, ST_RECEIVEDOFFER, ST_ACTIVE };
  State state_;
  std::vector<CryptoParams> offer_params_;
  SrtpSession send_session_;
  SrtpSession recv_session_;
};

}  

#endif  
