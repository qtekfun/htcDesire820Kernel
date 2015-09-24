// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_FILE_DESCRIPTOR_SET_POSIX_H_
#define IPC_FILE_DESCRIPTOR_SET_POSIX_H_

#include <vector>

#include "base/basictypes.h"
#include "base/file_descriptor_posix.h"
#include "base/memory/ref_counted.h"
#include "ipc/ipc_export.h"

class IPC_EXPORT FileDescriptorSet
    : public base::RefCountedThreadSafe<FileDescriptorSet> {
 public:
  FileDescriptorSet();

  
  
  
  
  
  
  
  
  static const size_t kMaxDescriptorsPerMessage = 7;

  
  

  
  bool Add(int fd);
  
  
  bool AddAndAutoClose(int fd);

  


  
  

  
  unsigned size() const { return descriptors_.size(); }
  
  bool empty() const { return descriptors_.empty(); }
  
  
  
  
  
  
  
  int GetDescriptorAt(unsigned n) const;

  


  
  

  
  
  
  void GetDescriptors(int* buffer) const;
  
  
  
  void CommitAll();
  
  
  bool ContainsDirectoryDescriptor() const;
  
  
  void ReleaseFDsToClose(std::vector<int>* fds);

  


  
  

  
  
  
  void SetDescriptors(const int* buffer, unsigned count);

  

 private:
  friend class base::RefCountedThreadSafe<FileDescriptorSet>;

  ~FileDescriptorSet();

  
  
  
  
  std::vector<base::FileDescriptor> descriptors_;

  
  
  
  
  mutable unsigned consumed_descriptor_highwater_;

  DISALLOW_COPY_AND_ASSIGN(FileDescriptorSet);
};

#endif  
