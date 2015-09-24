// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_FILE_TEMPLATE_H_
#define TOOLS_GN_FILE_TEMPLATE_H_

#include <iosfwd>

#include "base/basictypes.h"
#include "base/containers/stack_container.h"
#include "tools/gn/err.h"
#include "tools/gn/value.h"

struct EscapeOptions;
class ParseNode;
class Target;

extern const char kSourceExpansion_Help[];

class FileTemplate {
 public:
  struct Subrange {
    enum Type {
      LITERAL = 0,

      
      
      SOURCE,

      
      
      NAME_PART,

      
      
      FILE_PART,

      NUM_TYPES  
    };
    Subrange(Type t, const std::string& l = std::string())
        : type(t),
          literal(l) {
    }

    Type type;

    
    std::string literal;
  };

  
  
  FileTemplate(const Value& t, Err* err);
  FileTemplate(const std::vector<std::string>& t);
  ~FileTemplate();

  
  
  static FileTemplate GetForTargetOutputs(const Target* target);

  
  bool IsTypeUsed(Subrange::Type type) const;

  
  bool has_substitutions() const { return has_substitutions_; }

  
  
  
  void Apply(const Value& sources,
             const ParseNode* origin,
             std::vector<Value>* dest,
             Err* err) const;
  void ApplyString(const std::string& input,
                   std::vector<std::string>* output) const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void WriteWithNinjaExpansions(std::ostream& out) const;

  
  
  
  
  
  
  
  void WriteNinjaVariablesForSubstitution(
      std::ostream& out,
      const std::string& source,
      const EscapeOptions& escape_options) const;

  
  
  static const char* GetNinjaVariableNameForType(Subrange::Type type);

  
  
  static std::string GetSubstitution(const std::string& source,
                                     Subrange::Type type);

  
  static const char kSource[];
  static const char kSourceNamePart[];
  static const char kSourceFilePart[];

 private:
  typedef base::StackVector<Subrange, 8> Template;
  typedef base::StackVector<Template, 8> TemplateVector;

  void ParseInput(const Value& value, Err* err);

  
  void ParseOneTemplateString(const std::string& str);

  TemplateVector templates_;

  
  
  
  bool types_required_[Subrange::NUM_TYPES];

  
  
  bool has_substitutions_;
};

#endif  
