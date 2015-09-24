// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef APP_SQL_TRANSACTION_H_
#define APP_SQL_TRANSACTION_H_
#pragma once

#include "base/basictypes.h"

namespace sql {

class Connection;

class Transaction {
 public:
  
  
  
  
  explicit Transaction(Connection* connection);
  ~Transaction();

  
  bool is_open() const { return is_open_; }

  
  
  
  
  
  
  bool Begin();

  
  
  void Rollback();

  
  
  
  
  bool Commit();

 private:
  Connection* connection_;

  
  
  bool is_open_;

  DISALLOW_COPY_AND_ASSIGN(Transaction);
};

}  

#endif  
