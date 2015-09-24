// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_GYP_HELPER_H_
#define TOOLS_GN_GYP_HELPER_H_

#include <string>

#include "base/basictypes.h"

class Err;
class SourceDir;
class SourceFile;
class Target;

class GypHelper {
 public:
  GypHelper();
  ~GypHelper();

  SourceFile GetGypFileForTarget(const Target* target, Err* err) const;

  
  
  std::string GetNameForTarget(const Target* target) const;

  
  
  std::string GetFullRefForTarget(const Target* target) const;

  std::string GetFileReference(const SourceFile& file) const;
  std::string GetDirReference(const SourceDir& dir, bool include_slash) const;

 private:
  DISALLOW_COPY_AND_ASSIGN(GypHelper);
};

#endif  
