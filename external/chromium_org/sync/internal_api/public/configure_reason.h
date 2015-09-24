// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_CONFIGURE_REASON_H_
#define SYNC_INTERNAL_API_PUBLIC_CONFIGURE_REASON_H_

namespace syncer {

enum ConfigureReason {
  
  
  CONFIGURE_REASON_UNKNOWN,

  
  
  CONFIGURE_REASON_RECONFIGURATION,

  
  CONFIGURE_REASON_MIGRATION,

  
  
  CONFIGURE_REASON_NEW_CLIENT,

  
  CONFIGURE_REASON_NEWLY_ENABLED_DATA_TYPE,

  
  
  CONFIGURE_REASON_CRYPTO,
};

}  

#endif  
