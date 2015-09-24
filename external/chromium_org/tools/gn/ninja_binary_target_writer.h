// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_NINJA_BINARY_TARGET_WRITER_H_
#define TOOLS_GN_NINJA_BINARY_TARGET_WRITER_H_

#include "base/compiler_specific.h"
#include "tools/gn/ninja_target_writer.h"
#include "tools/gn/toolchain.h"

class NinjaBinaryTargetWriter : public NinjaTargetWriter {
 public:
  NinjaBinaryTargetWriter(const Target* target,
                          const Toolchain* toolchain,
                          std::ostream& out);
  virtual ~NinjaBinaryTargetWriter();

  virtual void Run() OVERRIDE;

 private:
  typedef std::set<OutputFile> OutputFileSet;

  void WriteCompilerVars();
  void WriteSources(std::vector<OutputFile>* object_files);
  void WriteLinkerStuff(const std::vector<OutputFile>& object_files);
  void WriteLinkerFlags(const Toolchain::Tool& tool,
                        const OutputFile& windows_manifest);
  void WriteLibs(const Toolchain::Tool& tool);

  
  void WriteLinkCommand(const OutputFile& external_output_file,
                        const OutputFile& internal_output_file,
                        const std::vector<OutputFile>& object_files);

  
  void WriteSourceSetStamp(const std::vector<OutputFile>& object_files);

  
  
  void GetDeps(std::set<OutputFile>* extra_object_files,
               std::vector<const Target*>* linkable_deps,
               std::vector<const Target*>* non_linkable_deps) const;

  
  
  
  
  void ClassifyDependency(const Target* dep,
                          std::set<OutputFile>* extra_object_files,
                          std::vector<const Target*>* linkable_deps,
                          std::vector<const Target*>* non_linkable_deps) const;

  
  
  
  
  
  void WriteImplicitDependencies(
      const std::vector<const Target*>& non_linkable_deps);

  Toolchain::ToolType tool_type_;

  DISALLOW_COPY_AND_ASSIGN(NinjaBinaryTargetWriter);
};

#endif  

