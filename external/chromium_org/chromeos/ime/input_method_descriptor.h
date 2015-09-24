// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_IME_INPUT_METHOD_DESCRIPTOR_H_
#define CHROMEOS_IME_INPUT_METHOD_DESCRIPTOR_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "chromeos/chromeos_export.h"
#include "url/gurl.h"

namespace chromeos {
namespace input_method {

class CHROMEOS_EXPORT InputMethodDescriptor {
 public:
  InputMethodDescriptor();
  InputMethodDescriptor(const std::string& id,
                        const std::string& name,
                        const std::vector<std::string>& keyboard_layouts,
                        const std::vector<std::string>& language_codes,
                        bool is_login_keyboard,
                        const GURL& options_page_url,
                        const GURL& input_view_url);
  ~InputMethodDescriptor();

  
  const std::string& id() const { return id_; }
  const std::string& name() const { return name_; }
  const std::vector<std::string>& language_codes() const {
    return language_codes_;
  }
  const GURL& options_page_url() const { return options_page_url_; }
  const GURL& input_view_url() const { return input_view_url_; }
  const std::vector<std::string>& keyboard_layouts() const {
    return keyboard_layouts_;
  }

  bool is_login_keyboard() const { return is_login_keyboard_; }

  
  std::string GetPreferredKeyboardLayout() const;

 private:
  
  
  std::string id_;

  
  
  std::string name_;

  
  
  std::vector<std::string> keyboard_layouts_;

  
  std::vector<std::string> language_codes_;

  
  bool is_login_keyboard_;

  
  
  
  GURL options_page_url_;

  
  
  
  GURL input_view_url_;
};

typedef std::vector<InputMethodDescriptor> InputMethodDescriptors;

}  
}  

#endif  
