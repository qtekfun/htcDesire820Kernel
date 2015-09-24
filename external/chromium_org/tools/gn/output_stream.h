// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_OUTPUT_STREAM_H_
#define TOOLS_GN_OUTPUT_STREAM_H_

class OutputStream {
 public:



  OutputStream& WriteBuffer(const char* buf, size_t len);
  OutputStream& WriteInt(int i);

  
  
  template<size_t size> OutputStream& Write(const char (&buf)[size]) {
    return WriteBuffer(buf, size);
  }

  
  OutputStream& Write(const std::string& str) {
    return WriteBuffer(str.c_str(), str.size());
  }

  
  
  
  OutputStream& WritePath(const std::string& s);
  OutputStream& WritePath(const std::string& s0,
                          const std::string& s1);
  OutputStream& WritePath(const std::string& s0,
                          const std::string& s1,
                          const std::string& s2);

  OutputStream& EndLine() {
    return WriteBuffer("\n", 1);
  }
};

#endif  
