// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_BROWSER_PROCESS_MAP_H_
#define EXTENSIONS_BROWSER_PROCESS_MAP_H_

#include <set>
#include <string>

#include "base/basictypes.h"

namespace extensions {

class ProcessMap {
 public:
  ProcessMap();
  ~ProcessMap();

  size_t size() const { return items_.size(); }

  bool Insert(const std::string& extension_id, int process_id,
              int site_instance_id);

  bool Remove(const std::string& extension_id, int process_id,
              int site_instance_id);
  int RemoveAllFromProcess(int process_id);

  bool Contains(const std::string& extension_id, int process_id) const;
  bool Contains(int process_id) const;

  std::set<std::string> GetExtensionsInProcess(int process_id) const;

 private:
  struct Item;

  typedef std::set<Item> ItemSet;
  ItemSet items_;

  DISALLOW_COPY_AND_ASSIGN(ProcessMap);
};

}  

#endif  
