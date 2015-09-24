// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef APP_SQL_DIAGNOSTIC_ERROR_DELEGATE_H_
#define APP_SQL_DIAGNOSTIC_ERROR_DELEGATE_H_
#pragma once

#include "app/sql/connection.h"
#include "base/logging.h"
#include "base/metrics/histogram.h"

namespace sql {

template <class UniqueT>
class DiagnosticErrorDelegate : public ErrorDelegate {
 public:

  virtual int OnError(int error, Connection* connection,
                      Statement* stmt) {
    NOTREACHED() << "sqlite error " << error
                 << ", errno " << connection->GetLastErrno()
                 << ": " << connection->GetErrorMessage();
    RecordErrorInHistogram(error);
    return error;
  }

 private:
  static void RecordErrorInHistogram(int error) {
    
    error &= 0xff;

    
    
    UMA_HISTOGRAM_ENUMERATION(UniqueT::name(), error, 50);
  }
};

}  

#endif  
