// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DEBUGGER_DEVTOOLS_REMOTE_MESSAGE_H_
#define CHROME_BROWSER_DEBUGGER_DEVTOOLS_REMOTE_MESSAGE_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/hash_tables.h"

struct DevToolsRemoteMessageHeaders {
  
  static const char kContentLength[];
  
  static const char kTool[];
  
  static const char kDestination[];
};

class DevToolsRemoteMessage {
 public:
  typedef base::hash_map<std::string, std::string> HeaderMap;

  
  
  static const char kEmptyValue[];

  
  DevToolsRemoteMessage();
  DevToolsRemoteMessage(const HeaderMap& headers, const std::string& content);
  virtual ~DevToolsRemoteMessage();

  const HeaderMap& headers() const {
    return header_map_;
  }

  const std::string& content() const {
    return content_;
  }

  int content_length() const {
    return content_.size();
  }

  const std::string tool() const {
    return GetHeaderWithEmptyDefault(DevToolsRemoteMessageHeaders::kTool);
  }

  const std::string destination() const {
    return GetHeaderWithEmptyDefault(
        DevToolsRemoteMessageHeaders::kDestination);
  }

  
  const std::string GetHeader(const std::string& header_name,
                              const std::string& default_value) const;

  
  const std::string GetHeaderWithEmptyDefault(
      const std::string& header_name) const;

  
  
  const std::string ToString() const;

 private:
  HeaderMap header_map_;
  std::string content_;
  
  
};

class DevToolsRemoteMessageBuilder {
 public:
  
  static DevToolsRemoteMessageBuilder& instance();
  
  DevToolsRemoteMessage* Create(const std::string& tool,
                                const std::string& destination,
                                const std::string& payload);

 private:
  DevToolsRemoteMessageBuilder() {}
  virtual ~DevToolsRemoteMessageBuilder() {}
  DISALLOW_COPY_AND_ASSIGN(DevToolsRemoteMessageBuilder);
};

#endif  
