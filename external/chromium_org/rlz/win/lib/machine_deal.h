// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RLZ_WIN_LIB_MACHINE_DEAL_H_
#define RLZ_WIN_LIB_MACHINE_DEAL_H_

#include <string>
#include "rlz/lib/rlz_lib.h"

namespace rlz_lib {

class MachineDealCode {
 public:
  
  
  
  static bool Set(const char* dcc);

  
  
  static bool Get(AccessPoint point,
                  char* dcc,
                  int dcc_size,
                  const wchar_t* sid = NULL);

  
  
  
  
  
  static bool SetFromPingResponse(const char* response);

  
  
  static bool GetNewCodeFromPingResponse(const char* response,
                                         bool* has_new_dcc,
                                         char* new_dcc,
                                         int new_dcc_size);

  
  static bool GetAsCgi(char* cgi, int cgi_size);

  
  static bool Get(char* dcc, int dcc_size);

 protected:
  
  
  
  static bool Clear();

  MachineDealCode() {}
  ~MachineDealCode() {}
};

}  

#endif  
