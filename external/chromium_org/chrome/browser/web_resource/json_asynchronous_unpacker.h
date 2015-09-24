// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEB_RESOURCE_JSON_ASYNCHRONOUS_UNPACKER_H_
#define CHROME_BROWSER_WEB_RESOURCE_JSON_ASYNCHRONOUS_UNPACKER_H_

#include <string>

#include "base/values.h"

class JSONAsynchronousUnpackerDelegate {
 public:
  virtual ~JSONAsynchronousUnpackerDelegate() {}

  
  
  virtual void OnUnpackFinished(const DictionaryValue& parsed_json) = 0;

  
  virtual void OnUnpackError(const std::string& error_message) = 0;
};

class JSONAsynchronousUnpacker {
 public:
  
  
  
  static JSONAsynchronousUnpacker*
      Create(JSONAsynchronousUnpackerDelegate* delegate);

  virtual ~JSONAsynchronousUnpacker() {}

  
  
  virtual void Start(const std::string& json_data) = 0;

  
  void ClearDelegate() {
    delegate_ = NULL;
  };

 protected:
  explicit JSONAsynchronousUnpacker(JSONAsynchronousUnpackerDelegate* delegate)
      : delegate_(delegate) {
  }

  JSONAsynchronousUnpackerDelegate* delegate_;
};

#endif  
