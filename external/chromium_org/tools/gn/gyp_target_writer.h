// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_GYP_TARGET_WRITER_H_
#define TOOLS_GN_GYP_TARGET_WRITER_H_

#include <iosfwd>
#include <vector>

#include "base/basictypes.h"
#include "tools/gn/gyp_helper.h"
#include "tools/gn/path_output.h"

class BuilderRecord;
class Err;
class Settings;
class SourceFile;
class Target;
class Toolchain;

class GypTargetWriter {
 public:
  struct TargetGroup {
    TargetGroup()
        : debug(NULL),
          release(NULL),
          host_debug(NULL),
          host_release(NULL),
          debug64(NULL),
          release64(NULL) {
    }
    const BuilderRecord* debug;
    const BuilderRecord* release;

    
    
    
    const BuilderRecord* host_debug;
    const BuilderRecord* host_release;

    
    const BuilderRecord* debug64;
    const BuilderRecord* release64;
  };

  GypTargetWriter(const Target* target,
                  const Toolchain* toolchain,
                  const SourceDir& gyp_dir,
                  std::ostream& out);
  virtual ~GypTargetWriter();

  static void WriteFile(const SourceFile& gyp_file,
                        const std::vector<TargetGroup>& targets,
                        const Toolchain* debug_toolchain,
                        Err* err);

  virtual void Run() = 0;

 protected:
  
  std::ostream& Indent(int spaces);
  static std::ostream& Indent(std::ostream& out, int spaces);

  static const int kExtraIndent = 2;

  const Settings* settings_;  
  const Target* target_;  
  const Toolchain* toolchain_;  
  SourceDir gyp_dir_;  
  std::ostream& out_;

  GypHelper helper_;
  PathOutput path_output_;

 private:
  DISALLOW_COPY_AND_ASSIGN(GypTargetWriter);
};

#endif  
