// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_IME_INPUT_METHOD_PROPERTY_H_
#define CHROMEOS_IME_INPUT_METHOD_PROPERTY_H_

#include <string>
#include <vector>
#include "chromeos/chromeos_export.h"

namespace chromeos {
namespace input_method {

struct CHROMEOS_EXPORT InputMethodProperty {
  InputMethodProperty(const std::string& in_key,
                      const std::string& in_label,
                      bool in_is_selection_item,
                      bool in_is_selection_item_checked);

  InputMethodProperty();
  ~InputMethodProperty();

  bool operator==(const InputMethodProperty& other) const;
  bool operator!=(const InputMethodProperty& other) const;

  
  std::string ToString() const;

  std::string key;  
                    
  std::string label;  
                      
  bool is_selection_item;  
  bool is_selection_item_checked;  
                                   
};
typedef std::vector<InputMethodProperty> InputMethodPropertyList;

}  
}  

#endif  
