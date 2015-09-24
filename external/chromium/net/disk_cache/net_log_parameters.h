// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_NET_LOG_PARAMETERS_H_
#define NET_DISK_CACHE_NET_LOG_PARAMETERS_H_
#pragma once

#include <string>

#include "net/base/net_log.h"

namespace disk_cache {

class EntryCreationParameters : public net::NetLog::EventParameters {
 public:
  EntryCreationParameters(const std::string& key, bool created);
  virtual Value* ToValue() const;

 private:
  const std::string key_;
  const bool created_;

  DISALLOW_COPY_AND_ASSIGN(EntryCreationParameters);
};

class ReadWriteDataParameters : public net::NetLog::EventParameters {
 public:
  
  ReadWriteDataParameters(int index, int offset, int buf_len, bool truncate);
  virtual Value* ToValue() const;

 private:
  const int index_;
  const int offset_;
  const int buf_len_;
  const bool truncate_;

  DISALLOW_COPY_AND_ASSIGN(ReadWriteDataParameters);
};

class ReadWriteCompleteParameters : public net::NetLog::EventParameters {
 public:
  
  
  
  explicit ReadWriteCompleteParameters(int bytes_copied);
  virtual Value* ToValue() const;

 private:
  const int bytes_copied_;

  DISALLOW_COPY_AND_ASSIGN(ReadWriteCompleteParameters);
};

class SparseOperationParameters : public net::NetLog::EventParameters {
 public:
  SparseOperationParameters(int64 offset, int buff_len);
  virtual Value* ToValue() const;

 private:
  const int64 offset_;
  const int buff_len_;
};

class SparseReadWriteParameters : public net::NetLog::EventParameters {
 public:
  SparseReadWriteParameters(const net::NetLog::Source& source, int child_len);
  virtual Value* ToValue() const;

 private:
  const net::NetLog::Source source_;
  const int child_len_;
};

class GetAvailableRangeResultParameters : public net::NetLog::EventParameters {
 public:
  
  GetAvailableRangeResultParameters(int64 start, int result);
  virtual Value* ToValue() const;

 private:
  const int64 start_;
  const int result_;
};

}  

#endif  
