// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_PROCESSES_API_H__
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_PROCESSES_API_H__
#pragma once

#include <set>
#include <string>

#include "chrome/browser/extensions/extension_function.h"
#include "chrome/browser/task_manager/task_manager.h"
#include "content/common/notification_registrar.h"

class ExtensionProcessesEventRouter : public TaskManagerModelObserver {
 public:
  
  static ExtensionProcessesEventRouter* GetInstance();

  
  void ObserveProfile(Profile* profile);

  
  void ListenerAdded();

  
  void ListenerRemoved();

 private:
  friend struct DefaultSingletonTraits<ExtensionProcessesEventRouter>;

  ExtensionProcessesEventRouter();
  virtual ~ExtensionProcessesEventRouter();

  
  virtual void OnModelChanged() {}
  virtual void OnItemsChanged(int start, int length);
  virtual void OnItemsAdded(int start, int length) {}
  virtual void OnItemsRemoved(int start, int length) {}

  void DispatchEvent(Profile* profile,
                     const char* event_name,
                     const std::string& json_args);

  
  NotificationRegistrar registrar_;

  
  typedef std::set<Profile*> ProfileSet;
  ProfileSet profiles_;

  
  TaskManagerModel* model_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionProcessesEventRouter);
};


class GetProcessIdForTabFunction : public SyncExtensionFunction {
  virtual ~GetProcessIdForTabFunction() {}
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("experimental.processes.getProcessIdForTab")
};

#endif  
