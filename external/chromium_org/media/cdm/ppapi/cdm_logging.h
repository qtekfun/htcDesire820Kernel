// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_CDM_PPAPI_CDM_LOGGING_H_
#define MEDIA_CDM_PPAPI_CDM_LOGGING_H_

#include <iostream>
#include <sstream>
#include <string>

namespace media {

namespace {


class LogMessageVoidify {
 public:
  LogMessageVoidify() {}
  
  
  void operator&(std::ostream&) {}
};

}  

class CdmLogMessage {
 public:
  CdmLogMessage(const char* file, int line);
  ~CdmLogMessage();

  std::string message() { return stream_.str(); }

 private:
  std::ostringstream stream_;
};

#define CDM_LAZY_STREAM(stream, condition) \
  !(condition) ? (void) 0 : LogMessageVoidify() & (stream)

#define CDM_DLOG() CDM_LAZY_STREAM(std::cout, CDM_DLOG_IS_ON()) \
  << CdmLogMessage(__FILE__, __LINE__).message()

#if defined(NDEBUG)
#define CDM_DLOG_IS_ON() false
#else
#define CDM_DLOG_IS_ON() true
#endif

}  

#endif  
