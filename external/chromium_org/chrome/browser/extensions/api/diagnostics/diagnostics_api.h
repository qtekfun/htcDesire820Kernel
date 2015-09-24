// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DIAGNOSTICS_DIAGNOSTICS_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_DIAGNOSTICS_DIAGNOSTICS_API_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/api/api_function.h"
#include "chrome/common/extensions/api/diagnostics.h"

namespace extensions {

class DiagnosticsSendPacketFunction : public AsyncApiFunction {
 public:
  
  
  enum SendPacketResultCode {
    
    SEND_PACKET_OK,

    
    SEND_PACKET_NOT_IMPLEMENTED,

    
    SEND_PACKET_FAILED,
  };

  DECLARE_EXTENSION_FUNCTION("diagnostics.sendPacket",
                             DIAGNOSTICS_SENDPACKET);

  DiagnosticsSendPacketFunction();

 protected:
  virtual ~DiagnosticsSendPacketFunction();

  
  virtual bool Prepare() OVERRIDE;
  
  virtual void AsyncWorkStart() OVERRIDE;
  virtual bool Respond() OVERRIDE;

 private:
  void SendPingPacket();
  void OnCompleted(SendPacketResultCode result_code,
                   const std::string& ip,
                   double latency);

  scoped_ptr<extensions::api::diagnostics::SendPacket::Params>
      parameters_;
};

}  

#endif  
