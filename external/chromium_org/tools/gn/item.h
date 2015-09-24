// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_ITEM_H_
#define TOOLS_GN_ITEM_H_

#include <string>

#include "tools/gn/label.h"

class Config;
class ParseNode;
class Settings;
class Target;
class Toolchain;

class Item {
 public:
  Item(const Settings* settings, const Label& label);
  virtual ~Item();

  const Settings* settings() const { return settings_; }

  
  
  const Label& label() const { return label_; }

  const ParseNode* defined_from() const { return defined_from_; }
  void set_defined_from(const ParseNode* df) { defined_from_ = df; }

  
  virtual Config* AsConfig();
  virtual const Config* AsConfig() const;
  virtual Target* AsTarget();
  virtual const Target* AsTarget() const;
  virtual Toolchain* AsToolchain();
  virtual const Toolchain* AsToolchain() const;

  
  
  std::string GetItemTypeName() const;

  
  
  virtual void OnResolved() {}

 private:
  const Settings* settings_;
  Label label_;
  const ParseNode* defined_from_;
};

#endif  
