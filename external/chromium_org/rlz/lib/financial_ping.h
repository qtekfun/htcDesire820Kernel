// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RLZ_LIB_FINANCIAL_PING_H_
#define RLZ_LIB_FINANCIAL_PING_H_

#include <string>
#include "rlz/lib/rlz_enums.h"

#if defined(RLZ_NETWORK_IMPLEMENTATION_CHROME_NET)
namespace net {
class URLRequestContextGetter;
}  
#endif

namespace rlz_lib {

class FinancialPing {
 public:
  
  
  
  
  static bool FormRequest(Product product, const AccessPoint* access_points,
                          const char* product_signature,
                          const char* product_brand, const char* product_id,
                          const char* product_lang, bool exclude_machine_id,
                          std::string* request);

  
  
  
  
  
  
  
  static bool IsPingTime(Product product, bool no_delay);

  
  static bool UpdateLastPingTime(Product product);

  
  
  static bool ClearLastPingTime(Product product);

  
  static bool PingServer(const char* request, std::string* response);

#if defined(RLZ_NETWORK_IMPLEMENTATION_CHROME_NET)
  static bool SetURLRequestContext(net::URLRequestContextGetter* context);
#endif

 private:
  FinancialPing() {}
  ~FinancialPing() {}
};

#if defined(RLZ_NETWORK_IMPLEMENTATION_CHROME_NET)
namespace test {
void ResetSendFinancialPingInterrupted();
bool WasSendFinancialPingInterrupted();
}  
#endif

}  


#endif  
