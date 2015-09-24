// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_MANAGEMENT_API_H__
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_MANAGEMENT_API_H__
#pragma once

#include "base/memory/singleton.h"
#include "chrome/browser/extensions/extension_function.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class ExtensionService;

class ExtensionManagementFunction : public SyncExtensionFunction {
 protected:
  ExtensionService* service();
};

class GetAllExtensionsFunction : public ExtensionManagementFunction {
  ~GetAllExtensionsFunction() {}
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("management.getAll");
};

class GetExtensionByIdFunction : public ExtensionManagementFunction {
  ~GetExtensionByIdFunction() {}
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("management.get");
};

class LaunchAppFunction : public ExtensionManagementFunction {
  ~LaunchAppFunction() {}
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("management.launchApp");
};

class SetEnabledFunction : public ExtensionManagementFunction {
  ~SetEnabledFunction() {}
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("management.setEnabled");
};

class UninstallFunction : public ExtensionManagementFunction {
  ~UninstallFunction() {}
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("management.uninstall");
};

class ExtensionManagementEventRouter : public NotificationObserver {
 public:
  
  static ExtensionManagementEventRouter* GetInstance();

  
  void Init();

 private:
  friend struct DefaultSingletonTraits<ExtensionManagementEventRouter>;

  ExtensionManagementEventRouter();
  virtual ~ExtensionManagementEventRouter();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionManagementEventRouter);
};

#endif  
