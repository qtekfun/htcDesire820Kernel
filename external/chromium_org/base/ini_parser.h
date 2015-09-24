// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_INI_PARSER_H_
#define BASE_INI_PARSER_H_

#include <string>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/values.h"

namespace base {

class BASE_EXPORT INIParser {
 public:
  INIParser();
  virtual ~INIParser();

  
  void Parse(const std::string& content);

 private:
  virtual void HandleTriplet(const std::string& section,
                             const std::string& key,
                             const std::string& value) = 0;

  bool used_;
};

class BASE_EXPORT DictionaryValueINIParser : public INIParser {
 public:
  DictionaryValueINIParser();
  virtual ~DictionaryValueINIParser();

  const DictionaryValue& root() const { return root_; }

 private:
  
  virtual void HandleTriplet(const std::string& section,
                             const std::string& key,
                             const std::string& value) OVERRIDE;

  DictionaryValue root_;

  DISALLOW_COPY_AND_ASSIGN(DictionaryValueINIParser);
};

}  

#endif  
