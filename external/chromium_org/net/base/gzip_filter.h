// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_BASE_GZIP_FILTER_H_
#define NET_BASE_GZIP_FILTER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/filter.h"

typedef struct z_stream_s z_stream;

namespace net {

class GZipHeader;

class GZipFilter : public Filter {
 public:
  virtual ~GZipFilter();

  
  
  
  
  
  bool InitDecoding(Filter::FilterType filter_type);

  
  
  
  
  
  
  // written into the destination buffer.
  
  
  
  
  
  virtual FilterStatus ReadFilteredData(char* dest_buffer,
                                        int* dest_len) OVERRIDE;

 private:
  enum DecodingStatus {
    DECODING_UNINITIALIZED,
    DECODING_IN_PROGRESS,
    DECODING_DONE,
    DECODING_ERROR
  };

  enum DecodingMode {
    DECODE_MODE_GZIP,
    DECODE_MODE_DEFLATE,
    DECODE_MODE_UNKNOWN
  };

  enum GZipCheckHeaderState {
    GZIP_CHECK_HEADER_IN_PROGRESS,
    GZIP_GET_COMPLETE_HEADER,
    GZIP_GET_INVALID_HEADER
  };

  static const int kGZipFooterSize = 8;

  
  GZipFilter();
  friend class Filter;

  
  
  
  
  
  
  
  
  
  
  FilterStatus CheckGZipHeader();

  
  
  
  
  
  FilterStatus DoInflate(char* dest_buffer, int* dest_len);

  
  
  
  
  bool InsertZlibHeader();

  
  void SkipGZipFooter();

  
  
  
  DecodingStatus decoding_status_;

  
  
  DecodingMode decoding_mode_;

  
  
  scoped_ptr<GZipHeader> gzip_header_;

  
  
  GZipCheckHeaderState gzip_header_status_;

  
  
  bool zlib_header_added_;

  
  int gzip_footer_bytes_;

  
  
  
  scoped_ptr<z_stream> zlib_stream_;

  
  
  
  
  
  
  
  bool possible_sdch_pass_through_;

  DISALLOW_COPY_AND_ASSIGN(GZipFilter);
};

}  

#endif  
