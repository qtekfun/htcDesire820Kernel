// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_IMPORT_MANAGER_H_
#define TOOLS_GN_IMPORT_MANAGER_H_

#include <map>

#include "base/synchronization/lock.h"

class Err;
class ParseNode;
class Scope;
class SourceFile;

class ImportManager {
 public:
  ImportManager();
  ~ImportManager();

  
  
  bool DoImport(const SourceFile& file,
                const ParseNode* node_for_err,
                Scope* scope,
                Err* err);

 private:
  base::Lock lock_;

  
  typedef std::map<SourceFile, const Scope*> ImportMap;
  ImportMap imports_;

  DISALLOW_COPY_AND_ASSIGN(ImportManager);
};

#endif  
