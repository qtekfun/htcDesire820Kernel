// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_FILTER_H__
#define NET_BASE_FILTER_H__
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time.h"

class GURL;

namespace net {

class IOBuffer;

class FilterContext {
 public:
  
  
  enum StatisticSelector {
    SDCH_DECODE,
    SDCH_PASSTHROUGH,
    SDCH_EXPERIMENT_DECODE,
    SDCH_EXPERIMENT_HOLDBACK,
  };

  virtual ~FilterContext();

  
  
  
  virtual bool GetMimeType(std::string* mime_type) const = 0;

  
  
  virtual bool GetURL(GURL* gurl) const = 0;

  
  virtual base::Time GetRequestTime() const = 0;

  
  virtual bool IsCachedContent() const = 0;

  
  virtual bool IsDownload() const = 0;

  
  virtual bool IsSdchResponse() const = 0;

  
  
  virtual int64 GetByteReadCount() const = 0;

  
  
  virtual int GetResponseCode() const = 0;

  
  
  virtual void RecordPacketStats(StatisticSelector statistic) const = 0;
};

class Filter {
 public:
  
  enum FilterStatus {
    
    FILTER_OK,
    
    
    
    
    FILTER_NEED_MORE_DATA,
    
    
    FILTER_DONE,
    
    FILTER_ERROR
  };

  
  enum FilterType {
    FILTER_TYPE_DEFLATE,
    FILTER_TYPE_GZIP,
    FILTER_TYPE_GZIP_HELPING_SDCH,  
    FILTER_TYPE_SDCH,
    FILTER_TYPE_SDCH_POSSIBLE,  
    FILTER_TYPE_UNSUPPORTED,
  };

  virtual ~Filter();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static Filter* Factory(const std::vector<FilterType>& filter_types,
                         const FilterContext& filter_context);

  
  
  
  static Filter* GZipFactory();

  
  
  FilterStatus ReadData(char* dest_buffer, int* dest_len);

  
  IOBuffer* stream_buffer() const { return stream_buffer_.get(); }

  
  int stream_buffer_size() const { return stream_buffer_size_; }

  
  
  
  
  
  int stream_data_len() const { return stream_data_len_; }

  
  
  
  
  
  
  
  
  
  
  bool FlushStreamBuffer(int stream_data_len);

  
  
  static FilterType ConvertEncodingToType(const std::string& filter_type);

  
  
  
  
  
  
  
  static void FixupEncodingTypes(const FilterContext& filter_context,
                                 std::vector<FilterType>* encoding_types);

 protected:
  friend class GZipUnitTest;
  friend class SdchFilterChainingTest;

  Filter();

  
  
  
  
  
  // written into the destination buffer.
  
  
  
  
  
  virtual FilterStatus ReadFilteredData(char* dest_buffer, int* dest_len) = 0;

  
  FilterStatus CopyOut(char* dest_buffer, int* dest_len);

  FilterStatus last_status() const { return last_status_; }

  
  scoped_refptr<IOBuffer> stream_buffer_;

  
  int stream_buffer_size_;

  
  char* next_stream_data_;

  
  int stream_data_len_;

 private:
  
  void InitBuffer(int size);

  
  
  
  
  
  static Filter* PrependNewFilter(FilterType type_id,
                                  const FilterContext& filter_context,
                                  int buffer_size,
                                  Filter* filter_list);

  
  
  static Filter* InitGZipFilter(FilterType type_id, int buffer_size);
  static Filter* InitSdchFilter(FilterType type_id,
                                const FilterContext& filter_context,
                                int buffer_size);

  
  void PushDataIntoNextFilter();

  
  
  static Filter* FactoryForTests(const std::vector<FilterType>& filter_types,
                                 const FilterContext& filter_context,
                                 int buffer_size);

  
  scoped_ptr<Filter> next_filter_;
  
  
  FilterStatus last_status_;

  DISALLOW_COPY_AND_ASSIGN(Filter);
};

}  

#endif  
