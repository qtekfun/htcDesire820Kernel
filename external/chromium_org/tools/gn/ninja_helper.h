// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_NINJA_HELPER_H_
#define TOOLS_GN_NINJA_HELPER_H_

#include <iosfwd>
#include <string>

#include "tools/gn/filesystem_utils.h"
#include "tools/gn/output_file.h"
#include "tools/gn/target.h"

class BuildSettings;
class SourceDir;
class SourceFile;
class Target;


class NinjaHelper {
 public:
  NinjaHelper(const BuildSettings* build_settings);
  ~NinjaHelper();

  
  std::string GetTopleveOutputDir() const;

  
  std::string GetTargetOutputDir(const Target* target) const;

  
  
  OutputFile GetNinjaFileForTarget(const Target* target) const;

  
  OutputFile GetNinjaFileForToolchain(const Settings* settings) const;

  
  
  OutputFile GetOutputFileForSource(const Target* target,
                                    const SourceFile& source,
                                    SourceFileType type) const;

  
  
  
  
  
  OutputFile GetTargetOutputFile(const Target* target) const;

  
  
  std::string GetRulePrefix(const Settings* settings) const;

  
  
  std::string GetRuleForSourceType(const Settings* settings,
                                   SourceFileType type) const;

  
  
  
  const std::string& build_to_src_no_last_slash() const {
    return build_to_src_no_last_slash_;
  }
  const std::string& build_to_src_system_no_last_slash() const {
    return build_to_src_system_no_last_slash_;
  }

 private:
  const BuildSettings* build_settings_;

  std::string build_to_src_no_last_slash_;
  std::string build_to_src_system_no_last_slash_;

  DISALLOW_COPY_AND_ASSIGN(NinjaHelper);
};

#endif  
