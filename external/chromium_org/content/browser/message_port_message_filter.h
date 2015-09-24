// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_MESSAGE_PORT_MESSAGE_FILTER_H_
#define CONTENT_BROWSER_MESSAGE_PORT_MESSAGE_FILTER_H_

#include "base/callback.h"
#include "content/public/browser/browser_message_filter.h"

namespace content {

class MessagePortMessageFilter : public BrowserMessageFilter {
 public:
  typedef base::Callback<int(void)> NextRoutingIDCallback;

  
  
  explicit MessagePortMessageFilter(const NextRoutingIDCallback& callback);

  
  virtual void OnChannelClosing() OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) OVERRIDE;
  virtual void OnDestruct() const OVERRIDE;

  int GetNextRoutingID();

 private:
  friend class BrowserThread;
  friend class base::DeleteHelper<MessagePortMessageFilter>;

  virtual ~MessagePortMessageFilter();

  
  void OnCreateMessagePort(int* route_id, int* message_port_id);

  
  
  NextRoutingIDCallback next_routing_id_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(MessagePortMessageFilter);
};

}  

#endif  
