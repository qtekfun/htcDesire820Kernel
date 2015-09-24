// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEB_RESOURCE_RESOURCE_REQUEST_ALLOWED_NOTIFIER_TEST_UTIL_H_
#define CHROME_BROWSER_WEB_RESOURCE_RESOURCE_REQUEST_ALLOWED_NOTIFIER_TEST_UTIL_H_

#include "chrome/browser/web_resource/resource_request_allowed_notifier.h"

class TestRequestAllowedNotifier : public ResourceRequestAllowedNotifier {
 public:
  TestRequestAllowedNotifier();
  virtual ~TestRequestAllowedNotifier();

  
  void InitWithEulaAcceptNotifier(
      Observer* observer,
      scoped_ptr<EulaAcceptedNotifier> eula_notifier);

  
  void SetRequestsAllowedOverride(bool allowed);

  
  
  void NotifyObserver();

  
  virtual State GetResourceRequestsAllowedState() OVERRIDE;
  virtual EulaAcceptedNotifier* CreateEulaNotifier() OVERRIDE;

 private:
  scoped_ptr<EulaAcceptedNotifier> test_eula_notifier_;
  bool override_requests_allowed_;
  bool requests_allowed_;

  DISALLOW_COPY_AND_ASSIGN(TestRequestAllowedNotifier);
};

#endif  
