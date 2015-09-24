// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_FTP_FTP_CTRL_RESPONSE_BUFFER_H_
#define NET_FTP_FTP_CTRL_RESPONSE_BUFFER_H_

#include <queue>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"

namespace net {

struct NET_EXPORT_PRIVATE FtpCtrlResponse {
  static const int kInvalidStatusCode;

  FtpCtrlResponse();
  ~FtpCtrlResponse();

  int status_code;                 
  std::vector<std::string> lines;  
};

class NET_EXPORT_PRIVATE FtpCtrlResponseBuffer {
 public:
  FtpCtrlResponseBuffer(const BoundNetLog& net_log);
  ~FtpCtrlResponseBuffer();

  
  int ConsumeData(const char* data, int data_length);

  bool ResponseAvailable() const {
    return !responses_.empty();
  }

  
  
  FtpCtrlResponse PopResponse();

 private:
  struct ParsedLine {
    ParsedLine();

    
    bool has_status_code;

    
    
    bool is_multiline;

    
    
    
    bool is_complete;

    
    int status_code;

    
    std::string status_text;

    
    std::string raw_text;
  };

  static ParsedLine ParseLine(const std::string& line);

  void ExtractFullLinesFromBuffer();

  
  std::string buffer_;

  std::queue<ParsedLine> lines_;

  
  bool multiline_;

  
  
  
  std::string line_buf_;

  
  FtpCtrlResponse response_buf_;

  
  std::queue<FtpCtrlResponse> responses_;

  BoundNetLog net_log_;

  DISALLOW_COPY_AND_ASSIGN(FtpCtrlResponseBuffer);
};

}  

#endif  
