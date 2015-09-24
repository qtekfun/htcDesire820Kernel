// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOMATION_AUTOMATION_RESOURCE_TRACKER_H__
#define CHROME_BROWSER_AUTOMATION_AUTOMATION_RESOURCE_TRACKER_H__
#pragma once

#include <map>

#include "base/basictypes.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "content/common/notification_source.h"
#include "content/common/notification_type.h"
#include "ipc/ipc_message.h"

template <class T>
struct AutomationResourceTraits {
  typedef T ValueType;
};

template <class T>
struct AutomationResourceTraits<T*> {
  typedef T ValueType;
};

class AutomationResourceTrackerImpl {
 public:
  explicit AutomationResourceTrackerImpl(IPC::Message::Sender* sender);
  virtual ~AutomationResourceTrackerImpl();

 protected:
  
  
  
  virtual void AddObserverTypeProxy(const void* resource) = 0;
  virtual void RemoveObserverTypeProxy(const void* resource) = 0;

  int AddImpl(const void* resource);
  void RemoveImpl(const void* resource);
  int GenerateHandle();
  bool ContainsResourceImpl(const void* resource);
  bool ContainsHandleImpl(int handle);
  const void* GetResourceImpl(int handle);
  int GetHandleImpl(const void* resource);
  void HandleCloseNotification(const void* resource);

 private:
  typedef std::map<const void*, int> ResourceToHandleMap;
  typedef std::map<int, const void*> HandleToResourceMap;

  ResourceToHandleMap resource_to_handle_;
  HandleToResourceMap handle_to_resource_;

  IPC::Message::Sender* sender_;

  DISALLOW_COPY_AND_ASSIGN(AutomationResourceTrackerImpl);
};

template <class T>
class AutomationResourceTracker : public AutomationResourceTrackerImpl,
                                  public NotificationObserver {
 public:
  explicit AutomationResourceTracker(IPC::Message::Sender* automation)
    : AutomationResourceTrackerImpl(automation) {}

  
  
  
  virtual void AddObserver(T resource) = 0;
  virtual void RemoveObserver(T resource) = 0;

  
  
  
  int Add(T resource) {
    return AddImpl(resource);
  }

  
  
  void Remove(T resource) {
    RemoveImpl(resource);
  }

  
  
  bool ContainsResource(T resource) {
    return ContainsResourceImpl(resource);
  }

  
  bool ContainsHandle(int handle) {
    return ContainsHandleImpl(handle);
  }

  
  
  
  T GetResource(int handle) {
    return static_cast<T>(const_cast<void*>(GetResourceImpl(handle)));
  }

  
  
  int GetHandle(T resource) {
    return GetHandleImpl(resource);
  }

  
  
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) {
     T resource =
        Source<typename AutomationResourceTraits<T>::ValueType>(source).ptr();

     CloseResource(resource);
  }

 protected:
  
  
  
  
  void CloseResource(T resource) {
    HandleCloseNotification(resource);
  }

  
  
  virtual void AddObserverTypeProxy(const void* resource) {
    AddObserver(static_cast<T>(const_cast<void*>(resource)));
  }
  virtual void RemoveObserverTypeProxy(const void* resource) {
    RemoveObserver(static_cast<T>(const_cast<void*>(resource)));
  }

  NotificationRegistrar registrar_;

 private:
  DISALLOW_COPY_AND_ASSIGN(AutomationResourceTracker);
};

#endif  
