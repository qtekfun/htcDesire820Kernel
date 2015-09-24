// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_INPUT_FILE_H_
#define TOOLS_GN_INPUT_FILE_H_

#include <string>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "tools/gn/source_dir.h"
#include "tools/gn/source_file.h"

class InputFile {
 public:
  InputFile(const SourceFile& name);

  ~InputFile();

  
  
  
  const SourceFile& name() const { return name_; }

  
  
  const SourceDir& dir() const { return dir_; }

  
  const base::FilePath& physical_name() const { return physical_name_; }

  
  
  
  const std::string& friendly_name() const { return friendly_name_; }
  void set_friendly_name(const std::string& f) { friendly_name_ = f; }

  const std::string& contents() const {
    DCHECK(contents_loaded_);
    return contents_;
  }

  
  
  void SetContents(const std::string& c);

  
  bool Load(const base::FilePath& system_path);

 private:
  SourceFile name_;
  SourceDir dir_;

  base::FilePath physical_name_;
  std::string friendly_name_;

  bool contents_loaded_;
  std::string contents_;

  DISALLOW_COPY_AND_ASSIGN(InputFile);
};

#endif  
