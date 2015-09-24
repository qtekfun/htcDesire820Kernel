// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#include <vector>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/io/zero_copy_stream.h>

namespace google {
namespace protobuf {
namespace compiler {

class ZipWriter {
 public:
  ZipWriter(io::ZeroCopyOutputStream* raw_output);
  ~ZipWriter();

  bool Write(const string& filename, const string& contents);
  bool WriteDirectory();

 private:
  struct FileInfo {
    string name;
    uint32 offset;
    uint32 size;
    uint32 crc32;
  };

  io::ZeroCopyOutputStream* raw_output_;
  vector<FileInfo> files_;
};

}  
}  
}  
