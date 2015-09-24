// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_JAVA_JAVA_BRIDGE_DISPATCHER_HOST_MANAGER_H_
#define CONTENT_BROWSER_RENDERER_HOST_JAVA_JAVA_BRIDGE_DISPATCHER_HOST_MANAGER_H_

#include <map>

#include "base/android/jni_helper.h"
#include "base/android/scoped_java_ref.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "content/public/browser/web_contents_observer.h"

struct NPObject;

namespace content {
class JavaBridgeDispatcherHost;
class RenderViewHost;

class JavaBridgeDispatcherHostManager
    : public WebContentsObserver,
      public base::SupportsWeakPtr<JavaBridgeDispatcherHostManager> {
 public:
  explicit JavaBridgeDispatcherHostManager(WebContents* web_contents);
  virtual ~JavaBridgeDispatcherHostManager();

  
  
  
  void AddNamedObject(const base::string16& name, NPObject* object);
  void RemoveNamedObject(const base::string16& name);

  void OnGetChannelHandle(RenderViewHost* render_view_host,
                          IPC::Message* reply_msg);

  
  
  
  
  
  
  void SetRetainedObjectSet(const JavaObjectWeakGlobalRef& retained_object_set);

  
  virtual void RenderViewCreated(RenderViewHost* render_view_host) OVERRIDE;
  virtual void RenderViewDeleted(RenderViewHost* render_view_host) OVERRIDE;
  virtual void DocumentAvailableInMainFrame() OVERRIDE;

  void JavaBoundObjectCreated(const base::android::JavaRef<jobject>& object);
  void JavaBoundObjectDestroyed(const base::android::JavaRef<jobject>& object);

  void AddMessageToConsole(int32 level, const char* message);

 private:
  typedef std::map<RenderViewHost*, scoped_refptr<JavaBridgeDispatcherHost> >
      InstanceMap;
  InstanceMap instances_;
  typedef std::map<base::string16, NPObject*> ObjectMap;
  ObjectMap objects_;
  JavaObjectWeakGlobalRef retained_object_set_;

  DISALLOW_COPY_AND_ASSIGN(JavaBridgeDispatcherHostManager);
};

}  

#endif  
