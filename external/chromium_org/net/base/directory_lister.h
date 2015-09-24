// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_DIRECTORY_LISTER_H_
#define NET_BASE_DIRECTORY_LISTER_H_

#include <vector>

#include "base/files/file_enumerator.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/message_loop/message_loop_proxy.h"
#include "net/base/net_export.h"

namespace net {

class NET_EXPORT DirectoryLister  {
 public:
  
  struct DirectoryListerData {
    base::FileEnumerator::FileInfo info;
    base::FilePath path;
  };

  
  class DirectoryListerDelegate {
   public:
    
    virtual void OnListFile(const DirectoryListerData& data) = 0;

    
    virtual void OnListDone(int error) = 0;

   protected:
    virtual ~DirectoryListerDelegate() {}
  };

  
  
  
  
  
  enum SortType {
    NO_SORT,
    DATE,
    ALPHA_DIRS_FIRST,
    FULL_PATH
  };

  DirectoryLister(const base::FilePath& dir,
                  DirectoryListerDelegate* delegate);

  DirectoryLister(const base::FilePath& dir,
                  bool recursive,
                  SortType sort,
                  DirectoryListerDelegate* delegate);

  
  ~DirectoryLister();

  
  bool Start();

  
  
  void Cancel();

 private:
  class Core : public base::RefCountedThreadSafe<Core> {
   public:
    Core(const base::FilePath& dir,
         bool recursive,
         SortType sort,
         DirectoryLister* lister);

    bool Start();

    void Cancel();

   private:
    friend class base::RefCountedThreadSafe<Core>;
    class DataEvent;

    ~Core();

    
    void StartInternal();

    void SendData(const std::vector<DirectoryListerData>& data);

    void OnDone(int error);

    base::FilePath dir_;
    bool recursive_;
    SortType sort_;
    scoped_refptr<base::MessageLoopProxy> origin_loop_;

    
    DirectoryLister* lister_;

    DISALLOW_COPY_AND_ASSIGN(Core);
  };

  void OnReceivedData(const DirectoryListerData& data);
  void OnDone(int error);

  const scoped_refptr<Core> core_;
  DirectoryListerDelegate* const delegate_;

  DISALLOW_COPY_AND_ASSIGN(DirectoryLister);
};

}  

#endif  
