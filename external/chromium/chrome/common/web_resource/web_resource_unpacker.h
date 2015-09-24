// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_COMMON_WEB_RESOURCE_WEB_RESOURCE_UNPACKER_H_
#define CHROME_COMMON_WEB_RESOURCE_WEB_RESOURCE_UNPACKER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

class DictionaryValue;

class WebResourceUnpacker {
 public:
  static const char* kInvalidDataTypeError;
  static const char* kUnexpectedJSONFormatError;

  explicit WebResourceUnpacker(const std::string &resource_data);
  ~WebResourceUnpacker();

  
  
  bool Run();

  
  const std::string& error_message() { return error_message_; }

  
  DictionaryValue* parsed_json() {
    return parsed_json_.get();
  }

 private:
  
  std::string resource_data_;

  
  scoped_ptr<DictionaryValue> parsed_json_;

  
  std::string error_message_;

  DISALLOW_COPY_AND_ASSIGN(WebResourceUnpacker);
};

#endif  
