// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_GYP_BINARY_TARGET_WRITER_H_
#define TOOLS_GN_GYP_BINARY_TARGET_WRITER_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "tools/gn/gyp_target_writer.h"
#include "tools/gn/target.h"
#include "tools/gn/toolchain.h"

class GypBinaryTargetWriter : public GypTargetWriter {
 public:
  GypBinaryTargetWriter(const TargetGroup& group,
                        const Toolchain* debug_toolchain,
                        const SourceDir& gyp_dir,
                        std::ostream& out);
  virtual ~GypBinaryTargetWriter();

  virtual void Run() OVERRIDE;

 private:
  struct Flags {
    Flags();
    ~Flags();

    std::vector<std::string> defines;
    std::vector<SourceDir> include_dirs;

    std::vector<std::string> cflags;
    std::vector<std::string> cflags_c;
    std::vector<std::string> cflags_cc;
    std::vector<std::string> cflags_objc;
    std::vector<std::string> cflags_objcc;
    std::vector<std::string> ldflags;
    std::vector<SourceDir> lib_dirs;
    std::vector<std::string> libs;
  };

  void WriteName(int indent);
  void WriteType(int indent);

  
  void WriteVCConfiguration(int indent);
  void WriteLinuxConfiguration(int indent);
  void WriteMacConfiguration(int indent);

  
  
  
  void WriteVCFlags(Flags& flags, int indent);
  void WriteMacFlags(Flags& flags, int indent);

  
  
  
  void WriteLinuxFlagsForTarget(const Target* target, int indent);
  void WriteLinuxFlags(const Flags& flags, int indent);

  
  void WriteSources(const Target* target, int indent);
  void WriteDeps(const Target* target, int indent);
  void WriteIncludeDirs(const Flags& flags, int indent);
  void WriteDirectDependentSettings(int indent);
  void WriteAllDependentSettings(int indent);

  
  void WriteSettingsFromConfigList(const std::vector<const Config*>& configs,
                                   int indent);

  
  Flags FlagsFromTarget(const Target* target) const;
  Flags FlagsFromConfigList(const LabelConfigVector& configs) const;

  
  
  
  void WriteNamedArray(const char* name,
                       const std::vector<std::string>& values,
                       int indent);

  
  TargetGroup group_;

  DISALLOW_COPY_AND_ASSIGN(GypBinaryTargetWriter);
};

#endif  

