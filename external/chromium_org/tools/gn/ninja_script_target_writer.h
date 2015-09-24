// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_NINJA_SCRIPT_TARGET_WRITER_H_
#define TOOLS_GN_NINJA_SCRIPT_TARGET_WRITER_H_

#include <vector>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "tools/gn/ninja_target_writer.h"

class FileTemplate;
class OutputFile;

class NinjaScriptTargetWriter : public NinjaTargetWriter {
 public:
  NinjaScriptTargetWriter(const Target* target,
                          const Toolchain* toolchain,
                          std::ostream& out);
  virtual ~NinjaScriptTargetWriter();

  virtual void Run() OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(NinjaScriptTargetWriter,
                           WriteOutputFilesForBuildLine);
  FRIEND_TEST_ALL_PREFIXES(NinjaScriptTargetWriter,
                           WriteOutputFilesForBuildLineWithDepfile);
  FRIEND_TEST_ALL_PREFIXES(NinjaScriptTargetWriter,
                           WriteArgsSubstitutions);

  bool has_sources() const { return !target_->sources().empty(); }

  
  
  
  
  
  std::string WriteRuleDefinition(const FileTemplate& args_template);

  
  
  
  
  
  void WriteSourceRules(const std::string& custom_rule_name,
                        const std::string& implicit_deps,
                        const FileTemplate& args_template,
                        std::vector<OutputFile>* output_files);

  
  
  void WriteArgsSubstitutions(const SourceFile& source,
                              const FileTemplate& args_template);

  
  
  void WriteStamp(const std::vector<OutputFile>& output_files);

  
  
  
  void WriteOutputFilesForBuildLine(const FileTemplate& output_template,
                                    const SourceFile& source,
                                    std::vector<OutputFile>* output_files);

  void WriteDepfile(const SourceFile& source);

  
  FileTemplate GetDepfileTemplate() const;

  
  
  
  PathOutput path_output_no_escaping_;

  DISALLOW_COPY_AND_ASSIGN(NinjaScriptTargetWriter);
};

#endif  
