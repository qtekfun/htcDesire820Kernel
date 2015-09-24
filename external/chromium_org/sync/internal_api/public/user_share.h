// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_USER_SHARE_H_
#define SYNC_INTERNAL_API_PUBLIC_USER_SHARE_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "sync/base/sync_export.h"

namespace syncer {

namespace syncable {
class Directory;
}

struct SYNC_EXPORT_PRIVATE UserShare {
  UserShare();
  ~UserShare();

  
  scoped_ptr<syncable::Directory> directory;
};

}  

#endif  
