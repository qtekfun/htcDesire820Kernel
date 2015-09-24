// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_NET_ERRORS_H__
#define NET_BASE_NET_ERRORS_H__
#pragma once

#include "base/basictypes.h"

namespace net {

extern const char kErrorDomain[];

enum Error {
  
  OK = 0,

#define NET_ERROR(label, value) ERR_ ## label = value,
#include "net/base/net_error_list.h"
#undef NET_ERROR

  
  ERR_CERT_BEGIN = ERR_CERT_COMMON_NAME_INVALID,
};

#pragma GCC visibility push(default)
const char* ErrorToString(int error);
#pragma GCC visibility pop

inline bool IsCertificateError(int error) {
  
  
  return error <= ERR_CERT_BEGIN && error > ERR_CERT_END;
}

Error MapSystemError(int os_error);

}  

#endif  
