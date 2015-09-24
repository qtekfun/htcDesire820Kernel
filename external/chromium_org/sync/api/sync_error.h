// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_API_SYNC_ERROR_H_
#define SYNC_API_SYNC_ERROR_H_

#include <iosfwd>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"

namespace tracked_objects {
class Location;
}  

namespace syncer {

class SYNC_EXPORT SyncError {
 public:
  
  
  
  enum ErrorType {
    UNSET,                
    UNRECOVERABLE_ERROR,  
                          
    DATATYPE_ERROR,       
                          
    PERSISTENCE_ERROR,    
                          
    CRYPTO_ERROR,         
                          
  };

  
  SyncError();

  
  
  
  SyncError(const tracked_objects::Location& location,
            ErrorType error_type,
            const std::string& message,
            ModelType model_type);

  
  SyncError(const SyncError& other);
  SyncError& operator=(const SyncError& other);

  ~SyncError();

  
  
  
  
  void Reset(const tracked_objects::Location& location,
             const std::string& message,
             ModelType type);

  
  bool IsSet() const;

  
  const tracked_objects::Location& location() const;
  const std::string& message() const;
  ModelType model_type() const;
  ErrorType error_type() const;

  
  std::string ToString() const;
 private:
  
  void PrintLogError() const;

  
  
  void Copy(const SyncError& other);

  
  
  void Init(const tracked_objects::Location& location,
            const std::string& message,
            ModelType model_type,
            ErrorType error_type);

  
  void Clear();

  
  scoped_ptr<tracked_objects::Location> location_;
  std::string message_;
  ModelType model_type_;
  ErrorType error_type_;
};

SYNC_EXPORT void PrintTo(const SyncError& sync_error, std::ostream* os);

}  

#endif  
