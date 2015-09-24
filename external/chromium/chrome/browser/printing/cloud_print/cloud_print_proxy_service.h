// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_CLOUD_PRINT_CLOUD_PRINT_PROXY_SERVICE_H_
#define CHROME_BROWSER_PRINTING_CLOUD_PRINT_CLOUD_PRINT_PROXY_SERVICE_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list.h"
#include "chrome/browser/printing/cloud_print/cloud_print_setup_handler.h"

class Profile;

class CloudPrintProxyService
    : public CloudPrintSetupHandlerDelegate,
      public base::RefCountedThreadSafe<CloudPrintProxyService> {
 public:
  explicit CloudPrintProxyService(Profile* profile);
  virtual ~CloudPrintProxyService();

  
  
  void Initialize();

  
  virtual void EnableForUser(const std::string& lsid, const std::string& email);
  virtual void DisableForUser();

  
  
  void RefreshStatusFromService();

  bool ShowTokenExpiredNotification();

  
  virtual void OnCloudPrintSetupClosed();

 private:
  
  class TokenExpiredNotificationDelegate;
  friend class TokenExpiredNotificationDelegate;

  Profile* profile_;
  scoped_refptr<TokenExpiredNotificationDelegate> token_expired_delegate_;
  scoped_ptr<CloudPrintSetupHandler> cloud_print_setup_handler_;

  
  void RefreshCloudPrintProxyStatus();
  void EnableCloudPrintProxy(const std::string& lsid, const std::string& email);
  void DisableCloudPrintProxy();

  
  void StatusCallback(bool enabled, std::string email);
  
  
  
  bool InvokeServiceTask(Task* task);

  void OnTokenExpiredNotificationError();
  void OnTokenExpiredNotificationClosed(bool by_user);
  void OnTokenExpiredNotificationClick();
  void TokenExpiredNotificationDone(bool keep_alive);

  DISALLOW_COPY_AND_ASSIGN(CloudPrintProxyService);
};

#endif  
