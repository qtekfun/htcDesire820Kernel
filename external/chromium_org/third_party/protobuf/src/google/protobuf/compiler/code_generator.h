// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_COMPILER_CODE_GENERATOR_H__
#define GOOGLE_PROTOBUF_COMPILER_CODE_GENERATOR_H__

#include <google/protobuf/stubs/common.h>
#include <string>
#include <vector>
#include <utility>

namespace google {
namespace protobuf {

namespace io { class ZeroCopyOutputStream; }
class FileDescriptor;

namespace compiler {

class CodeGenerator;
class GeneratorContext;

class LIBPROTOC_EXPORT CodeGenerator {
 public:
  inline CodeGenerator() {}
  virtual ~CodeGenerator();

  
  
  
  
  
  
  
  
  
  
  
  virtual bool Generate(const FileDescriptor* file,
                        const string& parameter,
                        GeneratorContext* generator_context,
                        string* error) const = 0;

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(CodeGenerator);
};

class LIBPROTOC_EXPORT GeneratorContext {
 public:
  inline GeneratorContext() {}
  virtual ~GeneratorContext();

  
  
  
  
  
  
  
  
  
  
  virtual io::ZeroCopyOutputStream* Open(const string& filename) = 0;

  
  
  
  
  
  
  virtual io::ZeroCopyOutputStream* OpenForInsert(
      const string& filename, const string& insertion_point);

  
  
  
  virtual void ListParsedFiles(vector<const FileDescriptor*>* output);

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(GeneratorContext);
};

typedef GeneratorContext OutputDirectory;

extern void ParseGeneratorParameter(const string&,
            vector<pair<string, string> >*);

}  
}  

}  
#endif  
