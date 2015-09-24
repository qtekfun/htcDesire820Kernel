// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_NPAPI_NPOBJECT_STUB_H_
#define CONTENT_CHILD_NPAPI_NPOBJECT_STUB_H_

#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "content/child/npapi/npobject_base.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_sender.h"
#include "url/gurl.h"

struct NPObject;

namespace content {
class NPChannelBase;
struct NPIdentifier_Param;
struct NPVariant_Param;

class NPObjectStub : public IPC::Listener,
                     public IPC::Sender,
                     public base::SupportsWeakPtr<NPObjectStub>,
                     public NPObjectBase {
 public:
  NPObjectStub(NPObject* npobject,
               NPChannelBase* channel,
               int route_id,
               int render_view_id,
               const GURL& page_url);
  virtual ~NPObjectStub();

  
  
  
  
  
  void DeleteSoon();

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;

  
  virtual NPObject* GetUnderlyingNPObject() OVERRIDE;
  virtual IPC::Listener* GetChannelListener() OVERRIDE;

 private:
  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

  
  void OnRelease(IPC::Message* reply_msg);
  void OnHasMethod(const NPIdentifier_Param& name,
                   bool* result);
  void OnInvoke(bool is_default,
                const NPIdentifier_Param& method,
                const std::vector<NPVariant_Param>& args,
                IPC::Message* reply_msg);
  void OnHasProperty(const NPIdentifier_Param& name,
                     bool* result);
  void OnGetProperty(const NPIdentifier_Param& name,
                     NPVariant_Param* property,
                     bool* result);
  void OnSetProperty(const NPIdentifier_Param& name,
                     const NPVariant_Param& property,
                     IPC::Message* reply_msg);
  void OnRemoveProperty(const NPIdentifier_Param& name,
                        bool* result);
  void OnInvalidate();
  void OnEnumeration(std::vector<NPIdentifier_Param>* value,
                     bool* result);
  void OnConstruct(const std::vector<NPVariant_Param>& args,
                   IPC::Message* reply_msg);
  void OnEvaluate(const std::string& script, bool popups_allowed,
                  IPC::Message* reply_msg);

 private:
  NPObject* npobject_;
  scoped_refptr<NPChannelBase> channel_;
  int route_id_;
  int render_view_id_;

  
  GURL page_url_;
};

}  

#endif  
