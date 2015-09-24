// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ACCESSIBILITY_ACCESSIBILITY_TREE_FORMATTER_H_
#define CONTENT_BROWSER_ACCESSIBILITY_ACCESSIBILITY_TREE_FORMATTER_H_

#include <vector>

#include "base/files/file_path.h"
#include "base/strings/string16.h"
#include "base/strings/utf_string_conversions.h"
#include "base/values.h"
#include "content/browser/accessibility/browser_accessibility.h"
#include "content/common/content_export.h"

namespace content {

class RenderViewHost;

class CONTENT_EXPORT AccessibilityTreeFormatter {
 public:
  explicit AccessibilityTreeFormatter(BrowserAccessibility* root);
  virtual ~AccessibilityTreeFormatter();

  static AccessibilityTreeFormatter* Create(RenderViewHost* rvh);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  scoped_ptr<base::DictionaryValue> BuildAccessibilityTree();

  
  void FormatAccessibilityTree(base::string16* contents);

  
  
  struct Filter {
    enum Type {
      ALLOW,
      ALLOW_EMPTY,
      DENY
    };
    base::string16 match_str;
    Type type;

    Filter(base::string16 match_str, Type type)
        : match_str(match_str), type(type) {}
  };

  
  
  void SetFilters(const std::vector<Filter>& filters);

  
  
  
  
  
  static const base::FilePath::StringType GetActualFileSuffix();
  static const base::FilePath::StringType GetExpectedFileSuffix();

  
  
  
  
  
  
  
  
  
  
  
  
  
  static const std::string GetAllowEmptyString();
  static const std::string GetAllowString();
  static const std::string GetDenyString();

 protected:
  void RecursiveFormatAccessibilityTree(const BrowserAccessibility& node,
                                        base::string16* contents,
                                        int indent);
  void RecursiveBuildAccessibilityTree(const BrowserAccessibility& node,
                                       base::DictionaryValue* tree_node);
  void RecursiveFormatAccessibilityTree(const base::DictionaryValue& tree_node,
                                        base::string16* contents,
                                        int depth = 0);

  
  
  void AddProperties(const BrowserAccessibility& node,
                     base::DictionaryValue* dict);

  base::string16 FormatCoordinates(const char* name,
                                   const char* x_name,
                                   const char* y_name,
                                   const base::DictionaryValue& value);

  
  
  
  base::string16 ToString(const base::DictionaryValue& node,
                          const base::string16& indent);

  void Initialize();

  bool MatchesFilters(const base::string16& text, bool default_result) const;

  
  void WriteAttribute(bool include_by_default,
                      const base::string16& attr,
                      base::string16* line);
  void WriteAttribute(bool include_by_default,
                      const std::string& attr,
                      base::string16* line);

  BrowserAccessibility* root_;

  
  std::vector<Filter> filters_;

  DISALLOW_COPY_AND_ASSIGN(AccessibilityTreeFormatter);
};

}  

#endif  
