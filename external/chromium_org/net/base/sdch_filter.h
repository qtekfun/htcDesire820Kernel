// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_BASE_SDCH_FILTER_H_
#define NET_BASE_SDCH_FILTER_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "net/base/filter.h"
#include "net/base/net_export.h"
#include "net/base/sdch_manager.h"

namespace open_vcdiff {
class VCDiffStreamingDecoder;
}

namespace net {

class NET_EXPORT_PRIVATE SdchFilter : public Filter {
 public:
  virtual ~SdchFilter();

  
  bool InitDecoding(Filter::FilterType filter_type);

  
  
  
  
  
  // written into the destination buffer.
  virtual FilterStatus ReadFilteredData(char* dest_buffer,
                                        int* dest_len) OVERRIDE;

 private:
  
  enum DecodingStatus {
    DECODING_UNINITIALIZED,
    WAITING_FOR_DICTIONARY_SELECTION,
    DECODING_IN_PROGRESS,
    DECODING_ERROR,
    META_REFRESH_RECOVERY,  
    PASS_THROUGH,  
  };

  
  explicit SdchFilter(const FilterContext& filter_context);
  friend class Filter;

  
  Filter::FilterStatus InitializeDictionary();

  
  
  int OutputBufferExcess(char* const dest_buffer, size_t available_space);

  
  const FilterContext& filter_context_;

  
  
  
  DecodingStatus decoding_status_;

  
  
  
  scoped_ptr<open_vcdiff::VCDiffStreamingDecoder> vcdiff_streaming_decoder_;

  
  
  std::string dictionary_hash_;

  
  
  
  
  
  
  bool dictionary_hash_is_plausible_;

  
  
  
  scoped_refptr<SdchManager::Dictionary> dictionary_;

  
  
  std::string dest_buffer_excess_;
  
  
  size_t dest_buffer_excess_index_;

  
  
  size_t source_bytes_;
  size_t output_bytes_;

  
  
  
  bool possible_pass_through_;

  
  
  GURL url_;

  
  
  std::string mime_type_;

  DISALLOW_COPY_AND_ASSIGN(SdchFilter);
};

}  

#endif  
