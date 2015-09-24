// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_COMPILER_JAVA_FILE_H__
#define GOOGLE_PROTOBUF_COMPILER_JAVA_FILE_H__

#include <string>
#include <vector>
#include <google/protobuf/stubs/common.h>

namespace google {
namespace protobuf {
  class FileDescriptor;        
  namespace io {
    class Printer;             
  }
  namespace compiler {
    class GeneratorContext;     
  }
}

namespace protobuf {
namespace compiler {
namespace java {

class FileGenerator {
 public:
  explicit FileGenerator(const FileDescriptor* file);
  ~FileGenerator();

  
  
  
  bool Validate(string* error);

  void Generate(io::Printer* printer);

  
  
  
  void GenerateSiblings(const string& package_dir,
                        GeneratorContext* generator_context,
                        vector<string>* file_list);

  const string& java_package() { return java_package_; }
  const string& classname()    { return classname_;    }


 private:
  
  
  
  bool ShouldIncludeDependency(const FileDescriptor* descriptor);

  const FileDescriptor* file_;
  string java_package_;
  string classname_;


  void GenerateEmbeddedDescriptor(io::Printer* printer);

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(FileGenerator);
};

}  
}  
}  

}  
#endif  
