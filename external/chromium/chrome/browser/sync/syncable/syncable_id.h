// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_SYNCABLE_SYNCABLE_ID_H_
#define CHROME_BROWSER_SYNC_SYNCABLE_SYNCABLE_ID_H_
#pragma once

#include <iosfwd>
#include <limits>
#include <sstream>
#include <string>

#include "base/hash_tables.h"

class StringValue;

namespace syncable {
struct EntryKernel;
class Id;
}

class MockConnectionManager;
class SQLStatement;

namespace syncable {

std::ostream& operator<<(std::ostream& out, const Id& id);

class Id {
  friend int UnpackEntry(SQLStatement* statement,
                         syncable::EntryKernel** kernel);
  friend int BindFields(const EntryKernel& entry, SQLStatement* statement);
  friend std::ostream& operator<<(std::ostream& out, const Id& id);
  friend class MockConnectionManager;
  friend class SyncableIdTest;
 public:
  
  
  inline Id() : s_("r") { }
  inline Id(const Id& that) {
    Copy(that);
  }
  inline Id& operator = (const Id& that) {
    Copy(that);
    return *this;
  }
  inline void Copy(const Id& that) {
    this->s_ = that.s_;
  }
  inline bool IsRoot() const {
    return "r" == s_;
  }
  inline bool ServerKnows() const {
    return s_[0] == 's' || s_ == "r";
  }

  
  
  inline bool IsNull() const {
    return IsRoot();
  }
  inline void Clear() {
    s_ = "r";
  }
  inline int compare(const Id& that) const {
    return s_.compare(that.s_);
  }
  inline bool operator == (const Id& that) const {
    return s_ == that.s_;
  }
  inline bool operator != (const Id& that) const {
    return s_ != that.s_;
  }
  inline bool operator < (const Id& that) const {
    return s_ < that.s_;
  }
  inline bool operator > (const Id& that) const {
    return s_ > that.s_;
  }
  
  
  
  Id GetLexicographicSuccessor() const;

  
  
  StringValue* ToValue() const;

  
  std::string GetServerId() const;
  static Id CreateFromServerId(const std::string& server_id);
  
  
  static Id CreateFromClientString(const std::string& local_id);

  
  
  
  static Id GetLeastIdForLexicographicComparison();

 private:
  std::string s_;
};

extern const Id kNullId;

}  

#endif  
