// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_REQUEST_SENDER_H_
#define CHROME_RENDERER_EXTENSIONS_REQUEST_SENDER_H_

#include <string>
#include <map>

#include "base/memory/linked_ptr.h"
#include "v8/include/v8.h"

namespace base {
class ListValue;
}

namespace extensions {
class ChromeV8Context;
class Dispatcher;

struct PendingRequest;

class RequestSender {
 public:
  
  
  
  
  
  class Source {
   public:
    virtual ~Source() {}

    virtual ChromeV8Context* GetContext() = 0;
    virtual void OnResponseReceived(const std::string& name,
                                    int request_id,
                                    bool success,
                                    const base::ListValue& response,
                                    const std::string& error) = 0;
  };

  explicit RequestSender(Dispatcher* dispatcher);
  ~RequestSender();

  
  
  int GetNextRequestId() const;

  
  
  
  
  
  
  void StartRequest(Source* source,
                    const std::string& name,
                    int request_id,
                    bool has_callback,
                    bool for_io_thread,
                    base::ListValue* value_args);

  
  void HandleResponse(int request_id,
                      bool success,
                      const base::ListValue& response,
                      const std::string& error);

  
  
  void InvalidateSource(Source* source);

 private:
  typedef std::map<int, linked_ptr<PendingRequest> > PendingRequestMap;

  void InsertRequest(int request_id, PendingRequest* pending_request);
  linked_ptr<PendingRequest> RemoveRequest(int request_id);

  Dispatcher* dispatcher_;
  PendingRequestMap pending_requests_;

  DISALLOW_COPY_AND_ASSIGN(RequestSender);
};

}  

#endif  
