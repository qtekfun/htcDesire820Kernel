// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_BUILD_SETTINGS_H_
#define TOOLS_GN_BUILD_SETTINGS_H_

#include <map>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "tools/gn/args.h"
#include "tools/gn/scope.h"
#include "tools/gn/source_dir.h"
#include "tools/gn/source_file.h"

class Item;
class OutputFile;

class BuildSettings {
 public:
  typedef base::Callback<void(scoped_ptr<Item>)> ItemDefinedCallback;

  BuildSettings();
  BuildSettings(const BuildSettings& other);
  ~BuildSettings();

  
  
  const base::FilePath& root_path() const { return root_path_; }
  const std::string& root_path_utf8() const { return root_path_utf8_; }
  void SetRootPath(const base::FilePath& r);

  
  
  
  const base::FilePath& secondary_source_path() const {
    return secondary_source_path_;
  }
  void SetSecondarySourcePath(const SourceDir& d);

  
  base::FilePath python_path() const { return python_path_; }
  void set_python_path(const base::FilePath& p) { python_path_ = p; }

  const SourceFile& build_config_file() const { return build_config_file_; }
  void set_build_config_file(const SourceFile& f) { build_config_file_ = f; }

  
  
  
  const SourceDir& build_dir() const { return build_dir_; }
  void SetBuildDir(const SourceDir& dir);

  
  
  const std::string& build_to_source_dir_string() const {
    return build_to_source_dir_string_;
  }

  
  Args& build_args() { return build_args_; }
  const Args& build_args() const { return build_args_; }

  
  
  base::FilePath GetFullPath(const SourceFile& file) const;
  base::FilePath GetFullPath(const SourceDir& dir) const;

  
  
  
  base::FilePath GetFullPathSecondary(const SourceFile& file) const;
  base::FilePath GetFullPathSecondary(const SourceDir& dir) const;

  
  void ItemDefined(scoped_ptr<Item> item) const;
  void set_item_defined_callback(ItemDefinedCallback cb) {
    item_defined_callback_ = cb;
  }

 private:
  base::FilePath root_path_;
  std::string root_path_utf8_;
  base::FilePath secondary_source_path_;
  base::FilePath python_path_;

  SourceFile build_config_file_;
  SourceDir build_dir_;
  std::string build_to_source_dir_string_;
  Args build_args_;

  ItemDefinedCallback item_defined_callback_;

  BuildSettings& operator=(const BuildSettings& other);  
};

#endif  
