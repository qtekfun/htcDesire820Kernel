// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_BASE_GZIP_HEADER_H_
#define NET_BASE_GZIP_HEADER_H_

#include "base/basictypes.h"

namespace net {

class GZipHeader {
 public:
  enum Status {
    INCOMPLETE_HEADER,    
    COMPLETE_HEADER,      
    INVALID_HEADER,       
  };

  GZipHeader();
  ~GZipHeader();

  
  void Reset();

  
  
  
  
  
  
  
  Status ReadMore(const char* inbuf,
                  int inbuf_len,
                  const char** header_end);
 private:
  enum {                       
    FLAG_FTEXT        = 0x01,  
    FLAG_FHCRC        = 0x02,  
    FLAG_FEXTRA       = 0x04,  
    FLAG_FNAME        = 0x08,  
    FLAG_FCOMMENT     = 0x10,  
    FLAG_RESERVED     = 0xE0,  
  };

  enum State {
    
    IN_HEADER_ID1,
    IN_HEADER_ID2,
    IN_HEADER_CM,
    IN_HEADER_FLG,
    IN_HEADER_MTIME_BYTE_0,
    IN_HEADER_MTIME_BYTE_1,
    IN_HEADER_MTIME_BYTE_2,
    IN_HEADER_MTIME_BYTE_3,
    IN_HEADER_XFL,
    IN_HEADER_OS,

    IN_XLEN_BYTE_0,
    IN_XLEN_BYTE_1,
    IN_FEXTRA,

    IN_FNAME,

    IN_FCOMMENT,

    IN_FHCRC_BYTE_0,
    IN_FHCRC_BYTE_1,

    IN_DONE,
  };

  static const uint8 magic[];  

  int    state_;  
  uint8  flags_;         
  uint16 extra_length_;  

  DISALLOW_COPY_AND_ASSIGN(GZipHeader);
};

}  

#endif  
