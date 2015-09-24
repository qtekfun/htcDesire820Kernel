// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SQL_TEST_ERROR_CALLBACK_SUPPORT_H_
#define SQL_TEST_ERROR_CALLBACK_SUPPORT_H_

#include "sql/connection.h"

namespace sql {

void CaptureErrorCallback(int* error_pointer, int error, sql::Statement* stmt);

class ScopedErrorCallback {
 public:
  ScopedErrorCallback(sql::Connection* db,
                      const sql::Connection::ErrorCallback& cb);
  ~ScopedErrorCallback();

 private:
  sql::Connection* db_;

  DISALLOW_COPY_AND_ASSIGN(ScopedErrorCallback);
};

}  

#endif  
