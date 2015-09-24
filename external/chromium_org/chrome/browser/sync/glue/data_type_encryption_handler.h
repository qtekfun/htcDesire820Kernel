// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_DATA_TYPE_ENCRYPTION_HANDLER_H_
#define CHROME_BROWSER_SYNC_GLUE_DATA_TYPE_ENCRYPTION_HANDLER_H_

#include "base/basictypes.h"
#include "sync/internal_api/public/base/model_type.h"

namespace browser_sync {

class DataTypeEncryptionHandler {
 public:
  DataTypeEncryptionHandler();
  virtual ~DataTypeEncryptionHandler();

  
  
  virtual bool IsPassphraseRequired() const = 0;

  
  virtual syncer::ModelTypeSet GetEncryptedDataTypes() const = 0;
};

} 

#endif  
