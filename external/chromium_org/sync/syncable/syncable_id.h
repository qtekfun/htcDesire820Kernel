// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_SYNCABLE_SYNCABLE_ID_H_
#define SYNC_SYNCABLE_SYNCABLE_ID_H_

#include <iosfwd>
#include <limits>
#include <sstream>
#include <string>

#include "base/containers/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "sync/base/sync_export.h"

class MockConnectionManager;

namespace base {
class StringValue;
}

namespace sql {
class Statement;
}

namespace syncer {
namespace syncable {
struct EntryKernel;
class Id;

SYNC_EXPORT_PRIVATE std::ostream& operator<<(std::ostream& out, const Id& id);

class SYNC_EXPORT Id {
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

  const std::string& value() const {
    return s_;
  }

  
  
  
  Id GetLexicographicSuccessor() const;

  
  
  base::StringValue* ToValue() const;

  
  std::string GetServerId() const;
  static Id CreateFromServerId(const std::string& server_id);
  
  
  static Id CreateFromClientString(const std::string& local_id);

  
  
  
  static Id GetLeastIdForLexicographicComparison();

 private:
  friend scoped_ptr<EntryKernel> UnpackEntry(sql::Statement* statement);
  friend void BindFields(const EntryKernel& entry,
                         sql::Statement* statement);
  SYNC_EXPORT_PRIVATE friend std::ostream& operator<<(std::ostream& out,
                                                      const Id& id);
  friend class MockConnectionManager;
  friend class SyncableIdTest;

  std::string s_;
};

SYNC_EXPORT_PRIVATE Id GetNullId();

}  
}  

#endif  
