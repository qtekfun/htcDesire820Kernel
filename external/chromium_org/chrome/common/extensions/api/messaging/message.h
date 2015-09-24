// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_API_MESSAGING_MESSAGE_H_
#define CHROME_COMMON_EXTENSIONS_API_MESSAGING_MESSAGE_H_

namespace extensions {

struct Message {
  std::string data;
  bool user_gesture;

  Message() : data(), user_gesture(false) {}
  Message(const std::string& data, bool user_gesture)
      : data(data), user_gesture(user_gesture) {}
};

}  

#endif  
