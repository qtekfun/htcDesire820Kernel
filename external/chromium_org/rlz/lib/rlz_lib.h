// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef RLZ_LIB_RLZ_LIB_H_
#define RLZ_LIB_RLZ_LIB_H_

#include <stdio.h>
#include <string>

#include "build/build_config.h"

#include "rlz/lib/rlz_enums.h"

#if defined(OS_WIN)
#define RLZ_LIB_API __cdecl
#else
#define RLZ_LIB_API
#endif

#if defined(RLZ_NETWORK_IMPLEMENTATION_WIN_INET) && \
    defined(RLZ_NETWORK_IMPLEMENTATION_CHROME_NET)
#error Exactly one of RLZ_NETWORK_IMPLEMENTATION_WIN_INET and \
    RLZ_NETWORK_IMPLEMENTATION_CHROME_NET should be defined.
#endif
#if !defined(RLZ_NETWORK_IMPLEMENTATION_WIN_INET) && \
    !defined(RLZ_NETWORK_IMPLEMENTATION_CHROME_NET)
#if defined(OS_WIN)
#define RLZ_NETWORK_IMPLEMENTATION_WIN_INET
#else
#define RLZ_NETWORK_IMPLEMENTATION_CHROME_NET
#endif
#endif

#if defined(RLZ_NETWORK_IMPLEMENTATION_CHROME_NET)
namespace net {
class URLRequestContextGetter;
}  
#endif

namespace rlz_lib {


class ScopedRlzValueStoreLock;

const size_t kMaxRlzLength = 64;
const size_t kMaxDccLength = 128;
const size_t kMaxCgiLength = 2048;
const size_t kMaxPingResponseLength = 0x4000;  

#if defined(RLZ_NETWORK_IMPLEMENTATION_CHROME_NET)
bool RLZ_LIB_API SetURLRequestContext(net::URLRequestContextGetter* context);
#endif


bool RLZ_LIB_API GetProductEventsAsCgi(Product product, char* unescaped_cgi,
                                       size_t unescaped_cgi_size);

bool RLZ_LIB_API RecordProductEvent(Product product, AccessPoint point,
                                    Event event_id);

bool RLZ_LIB_API ClearProductEvent(Product product, AccessPoint point,
                                   Event event_id);

bool RLZ_LIB_API ClearAllProductEvents(Product product);

void RLZ_LIB_API ClearProductState(Product product,
                                   const AccessPoint* access_points);

bool RLZ_LIB_API GetAccessPointRlz(AccessPoint point, char* rlz,
                                   size_t rlz_size);

bool RLZ_LIB_API SetAccessPointRlz(AccessPoint point, const char* new_rlz);


bool RLZ_LIB_API FormFinancialPingRequest(Product product,
                                          const AccessPoint* access_points,
                                          const char* product_signature,
                                          const char* product_brand,
                                          const char* product_id,
                                          const char* product_lang,
                                          bool exclude_machine_id,
                                          char* request,
                                          size_t request_buffer_size);

bool RLZ_LIB_API PingFinancialServer(Product product,
                                     const char* request,
                                     char* response,
                                     size_t response_buffer_size);

bool RLZ_LIB_API IsPingResponseValid(const char* response,
                                     int* checksum_idx);



bool RLZ_LIB_API ParseFinancialPingResponse(Product product,
                                            const char* response);

bool RLZ_LIB_API SendFinancialPing(Product product,
                                   const AccessPoint* access_points,
                                   const char* product_signature,
                                   const char* product_brand,
                                   const char* product_id,
                                   const char* product_lang,
                                   bool exclude_machine_id);

bool RLZ_LIB_API SendFinancialPing(Product product,
                                   const AccessPoint* access_points,
                                   const char* product_signature,
                                   const char* product_brand,
                                   const char* product_id,
                                   const char* product_lang,
                                   bool exclude_machine_id,
                                   const bool skip_time_check);

bool RLZ_LIB_API ParsePingResponse(Product product, const char* response);


bool RLZ_LIB_API GetPingParams(Product product,
                               const AccessPoint* access_points,
                               char* unescaped_cgi, size_t unescaped_cgi_size);

#if defined(OS_WIN)

bool RLZ_LIB_API CreateMachineState(void);

bool RLZ_LIB_API SetMachineDealCode(const char* dcc);

bool RLZ_LIB_API GetMachineDealCodeAsCgi(char* cgi, size_t cgi_size);

bool RLZ_LIB_API GetMachineDealCode(char* dcc, size_t dcc_size);

bool RLZ_LIB_API SetMachineDealCodeFromPingResponse(const char* response);

#endif

class SupplementaryBranding {
 public:
  SupplementaryBranding(const char* brand);
  ~SupplementaryBranding();

  static const std::string& GetBrand();

 private:
  ScopedRlzValueStoreLock* lock_;
};

}  

#endif  
