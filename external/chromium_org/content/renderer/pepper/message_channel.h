// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_MESSAGE_CHANNEL_H_
#define CONTENT_RENDERER_PEPPER_MESSAGE_CHANNEL_H_

#include <deque>
#include <list>
#include <map>

#include "base/memory/weak_ptr.h"
#include "ppapi/shared_impl/resource.h"
#include "third_party/WebKit/public/web/WebSerializedScriptValue.h"
#include "third_party/npapi/bindings/npruntime.h"

struct PP_Var;

namespace ppapi {
class ScopedPPVar;
}

namespace content {

class PepperPluginInstanceImpl;

class MessageChannel {
 public:
  
  
  
  
  struct MessageChannelNPObject : public NPObject {
    MessageChannelNPObject();
    ~MessageChannelNPObject();

    base::WeakPtr<MessageChannel> message_channel;
  };

  explicit MessageChannel(PepperPluginInstanceImpl* instance);
  ~MessageChannel();

  
  
  void NPVariantToPPVar(const NPVariant* variant);

  
  
  void PostMessageToJavaScript(PP_Var message_data);
  
  
  void PostMessageToNative(PP_Var message_data);

  
  
  
  
  NPObject* passthrough_object() {
    return passthrough_object_;
  }
  void SetPassthroughObject(NPObject* passthrough);

  NPObject* np_object() { return np_object_; }

  PepperPluginInstanceImpl* instance() {
    return instance_;
  }

  
  
  
  void QueueJavaScriptMessages();
  void StopQueueingJavaScriptMessages();

  bool GetReadOnlyProperty(NPIdentifier key, NPVariant* value) const;
  void SetReadOnlyProperty(PP_Var key, PP_Var value);

 private:
  
  struct VarConversionResult;

  
  
  
  void NPVariantToPPVarComplete(
      const std::list<VarConversionResult>::iterator& result_iterator,
      const ppapi::ScopedPPVar& result,
      bool success);

  PepperPluginInstanceImpl* instance_;

  
  
  
  
  
  NPObject* passthrough_object_;

  
  MessageChannelNPObject* np_object_;

  
  
  void PostMessageToJavaScriptImpl(
      const blink::WebSerializedScriptValue& message_data);
  
  
  void PostMessageToNativeImpl(PP_Var message_data);

  void DrainEarlyMessageQueue();

  
  
  std::deque<blink::WebSerializedScriptValue> early_message_queue_;
  enum EarlyMessageQueueState {
    QUEUE_MESSAGES,       
    SEND_DIRECTLY,        
    DRAIN_PENDING,        
    DRAIN_CANCELLED       
  };
  EarlyMessageQueueState early_message_queue_state_;

  
  
  
  
  std::list<VarConversionResult> converted_var_queue_;

  std::map<NPIdentifier, ppapi::ScopedPPVar> internal_properties_;

  
  
  base::WeakPtrFactory<MessageChannel> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(MessageChannel);
};

}  

#endif  
