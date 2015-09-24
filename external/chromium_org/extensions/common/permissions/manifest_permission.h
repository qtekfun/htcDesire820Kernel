// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_PERMISSIONS_MANIFEST_PERMISSION_H_
#define EXTENSIONS_COMMON_PERMISSIONS_MANIFEST_PERMISSION_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/pickle.h"
#include "extensions/common/permissions/permission_message.h"

class PickleIterator;

namespace base {
class Value;
class ListValue;
}

namespace IPC {
class Message;
}

namespace extensions {

class ManifestPermission {
 public:
  ManifestPermission();
  virtual ~ManifestPermission();

  
  virtual std::string name() const = 0;

  
  virtual std::string id() const = 0;

  
  virtual bool HasMessages() const = 0;

  
  virtual PermissionMessages GetMessages() const = 0;

  
  virtual bool FromValue(const base::Value* value) = 0;

  
  virtual scoped_ptr<base::Value> ToValue() const = 0;

  
  virtual ManifestPermission* Clone() const = 0;

  
  virtual ManifestPermission* Diff(const ManifestPermission* rhs) const = 0;

  
  virtual ManifestPermission* Union(const ManifestPermission* rhs) const = 0;

  
  
  virtual ManifestPermission* Intersect(const ManifestPermission* rhs)
      const = 0;

  
  virtual bool Contains(const ManifestPermission* rhs) const = 0;

  
  virtual bool Equal(const ManifestPermission* rhs) const = 0;

  
  
  virtual void Write(IPC::Message* m) const = 0;

  
  virtual bool Read(const IPC::Message* m, PickleIterator* iter) = 0;

  
  virtual void Log(std::string* log) const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(ManifestPermission);
};

}  

#endif  
