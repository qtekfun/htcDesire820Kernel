// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_DIRECTORY_LISTER_H_
#define NET_BASE_DIRECTORY_LISTER_H_
#pragma once

#include <vector>

#include "base/file_path.h"
#include "base/file_util.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/cancellation_flag.h"
#include "base/task.h"
#include "base/threading/platform_thread.h"

class MessageLoop;

namespace net {

class DirectoryLister : public base::RefCountedThreadSafe<DirectoryLister>,
                        public base::PlatformThread::Delegate {
 public:
  
  struct DirectoryListerData {
    file_util::FileEnumerator::FindInfo info;
    FilePath path;
  };

  
  class DirectoryListerDelegate {
   public:
    
    virtual void OnListFile(const DirectoryListerData& data) = 0;

    
    virtual void OnListDone(int error) = 0;

   protected:
    virtual ~DirectoryListerDelegate() {}
  };

  
  
  
  
  
  enum SORT_TYPE {
    NO_SORT,
    DATE,
    ALPHA_DIRS_FIRST,
    FULL_PATH
  };

  DirectoryLister(const FilePath& dir,
                  DirectoryListerDelegate* delegate);

  DirectoryLister(const FilePath& dir,
                  bool recursive,
                  SORT_TYPE sort,
                  DirectoryListerDelegate* delegate);


  
  bool Start();

  
  
  
  void Cancel();

  
  DirectoryListerDelegate* delegate() const { return delegate_; }
  void set_delegate(DirectoryListerDelegate* d) { delegate_ = d; }

  
  virtual void ThreadMain();

 private:
  friend class base::RefCountedThreadSafe<DirectoryLister>;
  friend class DirectoryDataEvent;

  ~DirectoryLister();

  
  static bool CompareAlphaDirsFirst(const DirectoryListerData& a,
                                    const DirectoryListerData& b);
  static bool CompareDate(const DirectoryListerData& a,
                          const DirectoryListerData& b);
  static bool CompareFullPath(const DirectoryListerData& a,
                              const DirectoryListerData& b);

  void OnReceivedData(const DirectoryListerData* data, int count);
  void OnDone(int error);

  FilePath dir_;
  bool recursive_;
  DirectoryListerDelegate* delegate_;
  SORT_TYPE sort_;
  MessageLoop* message_loop_;
  base::PlatformThreadHandle thread_;
  base::CancellationFlag canceled_;
};

}  

#endif  
