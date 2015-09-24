// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UPLOAD_LIST_H_
#define CHROME_BROWSER_UPLOAD_LIST_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/time/time.h"

class UploadList : public base::RefCountedThreadSafe<UploadList> {
 public:
  struct UploadInfo {
    UploadInfo(const std::string& c, const base::Time& t);
    ~UploadInfo();

    std::string id;
    base::Time time;
  };

  class Delegate {
   public:
    
    
    virtual void OnUploadListAvailable() = 0;

   protected:
    virtual ~Delegate() {}
  };

  
  UploadList(Delegate* delegate, const base::FilePath& upload_log_path);

  
  
  void LoadUploadListAsynchronously();

  
  
  void ClearDelegate();

  
  
  
  void GetUploads(unsigned int max_count, std::vector<UploadInfo>* uploads);

 protected:
  virtual ~UploadList();

  
  virtual void LoadUploadList();

  
  void AppendUploadInfo(const UploadInfo& info);

  
  void ClearUploads();

 private:
  friend class base::RefCountedThreadSafe<UploadList>;
  FRIEND_TEST_ALL_PREFIXES(UploadListTest, ParseLogEntries);

  
  void LoadUploadListAndInformDelegateOfCompletion();

  
  void InformDelegateOfCompletion();

  
  void ParseLogEntries(const std::vector<std::string>& log_entries);

  std::vector<UploadInfo> uploads_;
  Delegate* delegate_;
  const base::FilePath upload_log_path_;

  DISALLOW_COPY_AND_ASSIGN(UploadList);
};

#endif  
