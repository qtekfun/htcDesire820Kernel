// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DEVTOOLS_DEVTOOLS_PROTOCOL_H_
#define CHROME_BROWSER_DEVTOOLS_DEVTOOLS_PROTOCOL_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/values.h"

class DevToolsProtocol {
 public:
  class Message {
   public:
    virtual ~Message();

    std::string method() { return method_; }
    base::DictionaryValue* params() { return params_.get(); }

   protected:
    
    Message(const std::string& method, base::DictionaryValue* params);

    std::string method_;
    scoped_ptr<base::DictionaryValue> params_;

   private:
    DISALLOW_COPY_AND_ASSIGN(Message);
  };

  class Command : public Message {
   public:
    
    Command(int id, const std::string& method, base::DictionaryValue* params);
    virtual ~Command();

    int id() { return id_; }
    std::string Serialize();

   private:
    int id_;

    DISALLOW_COPY_AND_ASSIGN(Command);
  };

  class Response {
   public:
    virtual ~Response();

    int id() { return id_; }
    int error_code() { return error_code_; }

   private:
    friend class DevToolsProtocol;

    Response(int id, int error_code);
    int id_;
    int error_code_;

    DISALLOW_COPY_AND_ASSIGN(Response);
  };

  class Notification : public Message {
   public:
    virtual ~Notification();

   private:
    friend class DevToolsProtocol;

    
    Notification(const std::string& method,
                 base::DictionaryValue* params);

    DISALLOW_COPY_AND_ASSIGN(Notification);
  };

  
  static Notification* ParseNotification(const std::string& json);

  
  static Response* ParseResponse(const std::string& json);

 private:

  DevToolsProtocol() {}
  ~DevToolsProtocol() {}
};

#endif  
