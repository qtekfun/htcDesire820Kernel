// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_JAVA_JAVA_BOUND_OBJECT_H_
#define CONTENT_BROWSER_RENDERER_HOST_JAVA_JAVA_BOUND_OBJECT_H_

#include <jni.h>
#include <map>
#include <string>

#include "base/android/jni_helper.h"
#include "base/android/scoped_java_ref.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/weak_ptr.h"
#include "content/browser/renderer_host/java/java_method.h"
#include "third_party/npapi/bindings/npruntime.h"

namespace content {

class JavaBridgeDispatcherHostManager;

class JavaBoundObject {
 public:
  
  
  
  
  
  
  
  static NPObject* Create(
      const base::android::JavaRef<jobject>& object,
      const base::android::JavaRef<jclass>& safe_annotation_clazz,
      const base::WeakPtr<JavaBridgeDispatcherHostManager>& manager);

  virtual ~JavaBoundObject();

  
  
  
  static base::android::ScopedJavaLocalRef<jobject> GetJavaObject(
      NPObject* object);

  
  bool HasMethod(const std::string& name) const;
  bool Invoke(const std::string& name, const NPVariant* args, size_t arg_count,
              NPVariant* result);

 private:
  explicit JavaBoundObject(
      const base::android::JavaRef<jobject>& object,
      const base::android::JavaRef<jclass>& safe_annotation_clazz,
      const base::WeakPtr<JavaBridgeDispatcherHostManager>& manager_);

  void EnsureMethodsAreSetUp() const;

  
  
  JavaObjectWeakGlobalRef java_object_;

  
  
  
  
  base::WeakPtr<JavaBridgeDispatcherHostManager> manager_;

  
  
  
  typedef std::multimap<std::string, linked_ptr<JavaMethod> > JavaMethodMap;
  mutable JavaMethodMap methods_;
  mutable bool are_methods_set_up_;
  mutable jmethodID object_get_class_method_id_;

  base::android::ScopedJavaGlobalRef<jclass> safe_annotation_clazz_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(JavaBoundObject);
};

}  

#endif  
