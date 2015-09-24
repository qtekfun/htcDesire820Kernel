// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_TEST_ENGINE_TEST_ID_FACTORY_H_
#define SYNC_TEST_ENGINE_TEST_ID_FACTORY_H_

#include <string>

#include "base/strings/string_number_conversions.h"
#include "sync/syncable/syncable_id.h"

namespace syncer {

class TestIdFactory {
 public:
  TestIdFactory() : next_value_(1337000) {}
  ~TestIdFactory() {}

  
  static syncable::Id root() {
    return syncable::Id();
  }

  
  
  
  
  static syncable::Id FromNumber(int64 value) {
    if (value == 0)
      return root();
    else if (value < 0)
      return syncable::Id::CreateFromClientString(base::Int64ToString(value));
    else
      return syncable::Id::CreateFromServerId(base::Int64ToString(value));
  }

  
  static syncable::Id MakeLocal(std::string name) {
    return syncable::Id::CreateFromClientString(std::string("lient ") + name);
  }

  
  static syncable::Id MakeServer(std::string name) {
    return syncable::Id::CreateFromServerId(std::string("erver ") + name);
  }

  
  syncable::Id NewLocalId() {
    return syncable::Id::CreateFromClientString(
        std::string("_auto ") + base::IntToString(-next_value()));
  }

  
  syncable::Id NewServerId() {
    return syncable::Id::CreateFromServerId(
        std::string("_auto ") + base::IntToString(next_value()));
  }

 private:
  int next_value() {
    return next_value_++;
  }
  int next_value_;
};

}  

#endif  

