// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_DISK_CACHE_STORAGE_BLOCK_H_
#define NET_DISK_CACHE_STORAGE_BLOCK_H_

#include "net/disk_cache/addr.h"
#include "net/disk_cache/mapped_file.h"

namespace disk_cache {

template<typename T>
class StorageBlock : public FileBlock {
 public:
  StorageBlock(MappedFile* file, Addr address);
  virtual ~StorageBlock();

  
  virtual void* buffer() const;
  virtual size_t size() const;
  virtual int offset() const;

  
  bool LazyInit(MappedFile* file, Addr address);

  
  void SetData(T* other);

  
  
  void Discard();

  
  void StopSharingData();

  
  void set_modified();

  
  void clear_modified();

  
  T* Data();

  
  bool HasData() const;

  
  bool VerifyHash() const;

  
  bool own_data() const;

  const Addr address() const;

  
  bool Load();
  bool Store();
  bool Load(FileIOCallback* callback, bool* completed);
  bool Store(FileIOCallback* callback, bool* completed);

 private:
  void AllocateData();
  void DeleteData();
  uint32 CalculateHash() const;

  T* data_;
  MappedFile* file_;
  Addr address_;
  bool modified_;
  bool own_data_;  
  bool extended_;  

  DISALLOW_COPY_AND_ASSIGN(StorageBlock);
};

}  

#endif  
