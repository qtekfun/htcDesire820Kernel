// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_TOOLCHAIN_H_
#define TOOLS_GN_TOOLCHAIN_H_

#include "base/compiler_specific.h"
#include "base/strings/string_piece.h"
#include "tools/gn/item.h"
#include "tools/gn/scope.h"
#include "tools/gn/value.h"

class Toolchain : public Item {
 public:
  enum ToolType {
    TYPE_NONE = 0,
    TYPE_CC,
    TYPE_CXX,
    TYPE_OBJC,
    TYPE_OBJCXX,
    TYPE_RC,
    TYPE_ASM,
    TYPE_ALINK,
    TYPE_SOLINK,
    TYPE_LINK,
    TYPE_STAMP,
    TYPE_COPY,

    TYPE_NUMTYPES  
  };

  static const char* kToolCc;
  static const char* kToolCxx;
  static const char* kToolObjC;
  static const char* kToolObjCxx;
  static const char* kToolRc;
  static const char* kToolAsm;
  static const char* kToolAlink;
  static const char* kToolSolink;
  static const char* kToolLink;
  static const char* kToolStamp;
  static const char* kToolCopy;

  struct Tool {
    Tool();
    ~Tool();

    std::string command;
    std::string depfile;
    std::string deps;
    std::string description;
    std::string lib_dir_prefix;
    std::string lib_prefix;
    std::string pool;
    std::string restat;
    std::string rspfile;
    std::string rspfile_content;
  };

  Toolchain(const Settings* settings, const Label& label);
  virtual ~Toolchain();

  
  virtual Toolchain* AsToolchain() OVERRIDE;
  virtual const Toolchain* AsToolchain() const OVERRIDE;

  
  static ToolType ToolNameToType(const base::StringPiece& str);
  static std::string ToolTypeToName(ToolType type);

  const Tool& GetTool(ToolType type) const;
  void SetTool(ToolType type, const Tool& t);

  
  
  std::string gyp_header() const { return gyp_header_; }
  void set_gyp_header(const std::string& gh) { gyp_header_ = gh; }

  
  
  
  
  Scope::KeyValueMap& args() { return args_; }
  const Scope::KeyValueMap& args() const { return args_; }

 private:
  Tool tools_[TYPE_NUMTYPES];

  Scope::KeyValueMap args_;

  std::string gyp_header_;
};

#endif  
