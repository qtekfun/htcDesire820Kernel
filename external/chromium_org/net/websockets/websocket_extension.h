// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_EXTENSION_H_
#define NET_WEBSOCKETS_WEBSOCKET_EXTENSION_H_

#include <string>
#include <vector>

#include "net/base/net_export.h"

namespace net {

class NET_EXPORT_PRIVATE WebSocketExtension {
 public:
  
  class NET_EXPORT_PRIVATE Parameter {
   public:
    
    explicit Parameter(const std::string& name);
    
    Parameter(const std::string& name, const std::string& value);

    bool HasValue() const { return !value_.empty(); }
    const std::string& name() const { return name_; }
    const std::string& value() const { return value_; }
    bool Equals(const Parameter& other) const;

    
    
   private:
    std::string name_;
    std::string value_;
  };

  WebSocketExtension();
  explicit WebSocketExtension(const std::string& name);
  ~WebSocketExtension();

  void Add(const Parameter& parameter) { parameters_.push_back(parameter); }
  const std::string& name() const { return name_; }
  const std::vector<Parameter>& parameters() const { return parameters_; }
  bool Equals(const WebSocketExtension& other) const;

  
  
 private:
  std::string name_;
  std::vector<Parameter> parameters_;
};

}  

#endif  
