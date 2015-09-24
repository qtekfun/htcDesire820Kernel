// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_MDNS_CACHE_H_
#define NET_DNS_MDNS_CACHE_H_

#include <map>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "net/base/net_export.h"

namespace net {

class ParsedDnsRecord;
class RecordParsed;

class NET_EXPORT_PRIVATE MDnsCache {
 public:
  
  
  
  
  class Key {
   public:
    Key(unsigned type, const std::string& name, const std::string& optional);
    Key(const Key&);
    Key& operator=(const Key&);
    ~Key();
    bool operator<(const Key& key) const;
    bool operator==(const Key& key) const;

    unsigned type() const { return type_; }
    const std::string& name() const  { return name_; }
    const std::string& optional() const { return optional_; }

    
    static Key CreateFor(const RecordParsed* record);
   private:
    unsigned type_;
    std::string name_;
    std::string optional_;
  };

  typedef base::Callback<void(const RecordParsed*)> RecordRemovedCallback;

  enum UpdateType {
    RecordAdded,
    RecordChanged,
    NoChange
  };

  MDnsCache();
  ~MDnsCache();

  
  
  
  UpdateType UpdateDnsRecord(scoped_ptr<const RecordParsed> record);

  
  
  const RecordParsed* LookupKey(const Key& key);

  
  
  void FindDnsRecords(unsigned type,
                      const std::string& name,
                      std::vector<const RecordParsed*>* records,
                      base::Time now) const;

  
  
  void CleanupRecords(base::Time now,
                      const RecordRemovedCallback& record_removed_callback);

  
  
  
  base::Time next_expiration() const { return next_expiration_; }

  
  
  scoped_ptr<const RecordParsed> RemoveRecord(const RecordParsed* record);

  void Clear();

 private:
  typedef std::map<Key, const RecordParsed*> RecordMap;

  
  
  
  static base::Time GetEffectiveExpiration(const RecordParsed* entry);

  
  
  
  static std::string GetOptionalFieldForRecord(
      const RecordParsed* record);

  RecordMap mdns_cache_;

  base::Time next_expiration_;

  DISALLOW_COPY_AND_ASSIGN(MDnsCache);
};

}  

#endif  
