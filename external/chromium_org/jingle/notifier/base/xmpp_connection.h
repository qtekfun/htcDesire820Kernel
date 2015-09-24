// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef JINGLE_NOTIFIER_BASE_XMPP_CONNECTION_H_
#define JINGLE_NOTIFIER_BASE_XMPP_CONNECTION_H_

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "net/url_request/url_request_context_getter.h"
#include "talk/base/sigslot.h"
#include "talk/xmpp/xmppengine.h"

namespace buzz {
class PreXmppAuth;
class XmlElement;
class XmppClientSettings;
class XmppTaskParentInterface;
}  

namespace jingle_glue {
class TaskPump;
}  

namespace notifier {

class WeakXmppClient;

class XmppConnection
    : public sigslot::has_slots<>,
      public base::NonThreadSafe {
 public:
  class Delegate {
   public:
    
    
    
    virtual void OnConnect(
        base::WeakPtr<buzz::XmppTaskParentInterface> base_task) = 0;

    
    
    
    
    
    
    
    
    
    
    
    
    
    virtual void OnError(buzz::XmppEngine::Error error, int subcode,
                         const buzz::XmlElement* stream_error) = 0;

   protected:
    virtual ~Delegate();
  };

  
  
  
  
  XmppConnection(const buzz::XmppClientSettings& xmpp_client_settings,
                 const scoped_refptr<net::URLRequestContextGetter>&
                     request_context_getter,
                 Delegate* delegate,
                 buzz::PreXmppAuth* pre_xmpp_auth);

  
  
  
  virtual ~XmppConnection();

 private:
  void OnStateChange(buzz::XmppEngine::State state);
  void OnInputLog(const char* data, int len);
  void OnOutputLog(const char* data, int len);

  void ClearClient();

  scoped_ptr<jingle_glue::TaskPump> task_pump_;
  base::WeakPtr<WeakXmppClient> weak_xmpp_client_;
  bool on_connect_called_;
  Delegate* delegate_;

  FRIEND_TEST(XmppConnectionTest, RaisedError);
  FRIEND_TEST(XmppConnectionTest, Connect);
  FRIEND_TEST(XmppConnectionTest, MultipleConnect);
  FRIEND_TEST(XmppConnectionTest, ConnectThenError);
  FRIEND_TEST(XmppConnectionTest, TasksDontRunAfterXmppConnectionDestructor);

  DISALLOW_COPY_AND_ASSIGN(XmppConnection);
};

}  

#endif  
