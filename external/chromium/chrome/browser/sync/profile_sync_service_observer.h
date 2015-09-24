// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_PROFILE_SYNC_SERVICE_OBSERVER_H_
#define CHROME_BROWSER_SYNC_PROFILE_SYNC_SERVICE_OBSERVER_H_
#pragma once

class ProfileSyncServiceObserver {
 public:
  
  
  
  
  
  
  virtual void OnStateChanged() = 0;
 protected:
  virtual ~ProfileSyncServiceObserver() { }
};

#endif  
