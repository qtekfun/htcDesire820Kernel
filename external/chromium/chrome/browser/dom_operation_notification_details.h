// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOM_OPERATION_NOTIFICATION_DETAILS_H__
#define CHROME_BROWSER_DOM_OPERATION_NOTIFICATION_DETAILS_H__
#pragma once

class DomOperationNotificationDetails {
 public:
  DomOperationNotificationDetails(const std::string& json, int automation_id)
    : json_(json), automation_id_(automation_id) { }

  ~DomOperationNotificationDetails() { }

  std::string json() const { return json_; }
  int automation_id() const { return automation_id_; }

 private:
  std::string json_;
  int automation_id_;
};

#endif  
