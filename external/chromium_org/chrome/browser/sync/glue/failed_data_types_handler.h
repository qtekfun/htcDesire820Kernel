// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_FAILED_DATA_TYPES_HANDLER_H_
#define CHROME_BROWSER_SYNC_GLUE_FAILED_DATA_TYPES_HANDLER_H_

#include <string>

#include "chrome/browser/sync/glue/data_type_manager.h"

namespace browser_sync {

class FailedDataTypesHandler {
 public:
  typedef std::map<syncer::ModelType, syncer::SyncError> TypeErrorMap;

  explicit FailedDataTypesHandler();
  ~FailedDataTypesHandler();

  
  
  bool UpdateFailedDataTypes(const TypeErrorMap& errors);

  
  void Reset();

  
  void ResetCryptoErrors();

  
  void ResetPersistenceErrorsFrom(syncer::ModelTypeSet purged_types);

  
  TypeErrorMap GetAllErrors() const;

  
  syncer::ModelTypeSet GetFailedTypes() const;

  
  syncer::ModelTypeSet GetFatalErrorTypes() const;

  
  syncer::ModelTypeSet GetCryptoErrorTypes() const;

  
  syncer::ModelTypeSet GetPersistenceErrorTypes() const;

 private:
  
  bool AnyFailedDataType() const;

  
  
  TypeErrorMap fatal_errors_;

  
  TypeErrorMap crypto_errors_;

  
  
  TypeErrorMap persistence_errors_;

  DISALLOW_COPY_AND_ASSIGN(FailedDataTypesHandler);
};

}  

#endif  
