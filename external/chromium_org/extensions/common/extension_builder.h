// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_EXTENSION_BUILDER_H_
#define EXTENSIONS_COMMON_EXTENSION_BUILDER_H_

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "extensions/common/manifest.h"
#include "extensions/common/value_builder.h"

namespace extensions {
class Extension;

class ExtensionBuilder {
 public:
  ExtensionBuilder();
  ~ExtensionBuilder();

  
  
  scoped_refptr<Extension> Build();

  
  
  ExtensionBuilder& Pass() { return *this; }

  
  ExtensionBuilder& SetPath(const base::FilePath& path);

  
  ExtensionBuilder& SetLocation(Manifest::Location location);

  ExtensionBuilder& SetManifest(scoped_ptr<base::DictionaryValue> manifest);
  ExtensionBuilder& SetManifest(DictionaryBuilder& manifest_builder) {
    return SetManifest(manifest_builder.Build());
  }

  
  
  ExtensionBuilder& MergeManifest(DictionaryBuilder& builder);

  ExtensionBuilder& AddFlags(int init_from_value_flags);

  
  ExtensionBuilder& SetID(const std::string& id);

 private:
  base::FilePath path_;
  Manifest::Location location_;
  scoped_ptr<base::DictionaryValue> manifest_;
  int flags_;
  std::string id_;
};

} 

#endif  
