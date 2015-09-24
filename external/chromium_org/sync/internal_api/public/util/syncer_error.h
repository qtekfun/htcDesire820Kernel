// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_UTIL_SYNCER_ERROR_H_
#define SYNC_INTERNAL_API_PUBLIC_UTIL_SYNCER_ERROR_H_

#include "sync/base/sync_export.h"

namespace syncer {

enum SYNC_EXPORT_PRIVATE SyncerError {
  UNSET = 0,       
  CANNOT_DO_WORK,  

  NETWORK_CONNECTION_UNAVAILABLE,  
  NETWORK_IO_ERROR,                
  SYNC_SERVER_ERROR,               
  SYNC_AUTH_ERROR,                 

  
  SERVER_RETURN_INVALID_CREDENTIAL,
  SERVER_RETURN_UNKNOWN_ERROR,
  SERVER_RETURN_THROTTLED,
  SERVER_RETURN_TRANSIENT_ERROR,
  SERVER_RETURN_MIGRATION_DONE,
  SERVER_RETURN_CLEAR_PENDING,
  SERVER_RETURN_NOT_MY_BIRTHDAY,
  SERVER_RETURN_CONFLICT,
  SERVER_RESPONSE_VALIDATION_FAILED,
  SERVER_RETURN_DISABLED_BY_ADMIN,

  SERVER_MORE_TO_DOWNLOAD,

  SYNCER_OK
};

SYNC_EXPORT const char* GetSyncerErrorString(SyncerError);

bool SyncerErrorIsError(SyncerError error);

}  

#endif  
