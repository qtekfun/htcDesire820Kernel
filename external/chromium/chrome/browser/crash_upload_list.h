// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CRASH_UPLOAD_LIST_H_
#define CHROME_BROWSER_CRASH_UPLOAD_LIST_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base/time.h"

#include <string>
#include <vector>

class CrashUploadList : public base::RefCountedThreadSafe<CrashUploadList> {
 public:
  struct CrashInfo {
    CrashInfo(const std::string& c, const base::Time& t);
    ~CrashInfo();

    std::string crash_id;
    base::Time crash_time;
  };

  class Delegate {
   public:
    
    
    virtual void OnCrashListAvailable() = 0;

   protected:
    virtual ~Delegate() {}
  };

  
  
  static CrashUploadList* Create(Delegate* delegate);

  
  explicit CrashUploadList(Delegate* delegate);

  
  
  void LoadCrashListAsynchronously();

  
  
  void ClearDelegate();

  
  
  
  void GetUploadedCrashes(unsigned int max_count,
                          std::vector<CrashInfo>* crashes);

 protected:
  virtual ~CrashUploadList();

  
  virtual void LoadCrashList();

  
  std::vector<CrashInfo>& crashes();

 private:
  friend class base::RefCountedThreadSafe<CrashUploadList>;

  
  void LoadCrashListAndInformDelegateOfCompletion();

  
  void InformDelegateOfCompletion();

  
  void ParseLogEntries(const std::vector<std::string>& log_entries);

  std::vector<CrashInfo> crashes_;
  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(CrashUploadList);
};

#endif  
