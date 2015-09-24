// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef JINGLE_NOTIFIER_BASE_WEAK_XMPP_CLIENT_H_
#define JINGLE_NOTIFIER_BASE_WEAK_XMPP_CLIENT_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "talk/xmpp/xmppclient.h"

namespace talk_base {
class TaskParent;
}  

namespace notifier {

class WeakXmppClient : public buzz::XmppClient, public base::NonThreadSafe {
 public:
  explicit WeakXmppClient(talk_base::TaskParent* parent);

  virtual ~WeakXmppClient();

  
  
  base::WeakPtr<WeakXmppClient> AsWeakPtr();

  
  
  
  void Invalidate();

 protected:
  virtual void Stop() OVERRIDE;

 private:
  
  
  
  base::WeakPtrFactory<WeakXmppClient> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(WeakXmppClient);
};

}  

#endif  
