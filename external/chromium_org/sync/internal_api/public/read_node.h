// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_READ_NODE_H_
#define SYNC_INTERNAL_API_PUBLIC_READ_NODE_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/base_node.h"

namespace syncer {

class SYNC_EXPORT ReadNode : public BaseNode {
 public:
  
  
  explicit ReadNode(const BaseTransaction* transaction);
  virtual ~ReadNode();

  
  

  
  virtual InitByLookupResult InitByIdLookup(int64 id) OVERRIDE;
  virtual InitByLookupResult InitByClientTagLookup(
      ModelType model_type,
      const std::string& tag) OVERRIDE;

  
  
  void InitByRootLookup();

  
  
  
  InitByLookupResult InitByTagLookup(const std::string& tag);

  
  virtual const syncable::Entry* GetEntry() const OVERRIDE;
  virtual const BaseTransaction* GetTransaction() const OVERRIDE;

 protected:
  ReadNode();

 private:
  void* operator new(size_t size);  

  
  syncable::Entry* entry_;

  
  const BaseTransaction* transaction_;

  DISALLOW_COPY_AND_ASSIGN(ReadNode);
};

}  

#endif  
