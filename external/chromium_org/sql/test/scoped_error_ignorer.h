// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SQL_TEST_SCOPED_ERROR_IGNORER_H_
#define SQL_TEST_SCOPED_ERROR_IGNORER_H_

#include <set>

#include "base/basictypes.h"
#include "sql/connection.h"

namespace sql {

class ScopedErrorIgnorer {
 public:
  ScopedErrorIgnorer();
  ~ScopedErrorIgnorer();

  
  
  
  void IgnoreError(int err);

  
  
  bool CheckIgnoredErrors();

  
  bool ShouldIgnore(int err);

 private:
  
  Connection::ErrorIgnorerCallback callback_;

  
  bool checked_;

  
  std::set<int> ignore_errors_;

  
  std::set<int> errors_ignored_;

  DISALLOW_COPY_AND_ASSIGN(ScopedErrorIgnorer);
};

}  

#endif  
