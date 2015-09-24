// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_BALSA_BALSA_ENUMS_H_
#define NET_TOOLS_BALSA_BALSA_ENUMS_H_

namespace net {

struct BalsaFrameEnums {
  enum ParseState {
    PARSE_ERROR,
    READING_HEADER_AND_FIRSTLINE,
    READING_CHUNK_LENGTH,
    READING_CHUNK_EXTENSION,
    READING_CHUNK_DATA,
    READING_CHUNK_TERM,
    READING_LAST_CHUNK_TERM,
    READING_TRAILER,
    READING_UNTIL_CLOSE,
    READING_CONTENT,
    MESSAGE_FULLY_READ,
    NUM_STATES,
  };

  enum ErrorCode {
    NO_ERROR = 0,  
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    NO_STATUS_LINE_IN_RESPONSE,                      
    NO_REQUEST_LINE_IN_REQUEST,                      
    FAILED_TO_FIND_WS_AFTER_RESPONSE_VERSION,        
    FAILED_TO_FIND_WS_AFTER_REQUEST_METHOD,          
    FAILED_TO_FIND_WS_AFTER_RESPONSE_STATUSCODE,     
    FAILED_TO_FIND_WS_AFTER_REQUEST_REQUEST_URI,     
    FAILED_TO_FIND_NL_AFTER_RESPONSE_REASON_PHRASE,  
    FAILED_TO_FIND_NL_AFTER_REQUEST_HTTP_VERSION,    

    FAILED_CONVERTING_STATUS_CODE_TO_INT,
    REQUEST_URI_TOO_LONG,  

    HEADERS_TOO_LONG,
    UNPARSABLE_CONTENT_LENGTH,
    
    
    MAYBE_BODY_BUT_NO_CONTENT_LENGTH,

    
    REQUIRED_BODY_BUT_NO_CONTENT_LENGTH,

    HEADER_MISSING_COLON,

    
    INVALID_CHUNK_LENGTH,
    CHUNK_LENGTH_OVERFLOW,

    
    CALLED_BYTES_SPLICED_WHEN_UNSAFE_TO_DO_SO,
    CALLED_BYTES_SPLICED_AND_EXCEEDED_SAFE_SPLICE_AMOUNT,
    MULTIPLE_CONTENT_LENGTH_KEYS,
    MULTIPLE_TRANSFER_ENCODING_KEYS,
    UNKNOWN_TRANSFER_ENCODING,
    INVALID_HEADER_FORMAT,

    
    INTERNAL_LOGIC_ERROR,

    NUM_ERROR_CODES
  };
  static const char* ParseStateToString(ParseState error_code);
  static const char* ErrorCodeToString(ErrorCode error_code);
};

struct BalsaHeadersEnums {
  enum ContentLengthStatus {
    INVALID_CONTENT_LENGTH,
    CONTENT_LENGTH_OVERFLOW,
    NO_CONTENT_LENGTH,
    VALID_CONTENT_LENGTH,
  };
};

}  

#endif  

