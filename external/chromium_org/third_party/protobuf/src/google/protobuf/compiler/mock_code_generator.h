// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_COMPILER_MOCK_CODE_GENERATOR_H__
#define GOOGLE_PROTOBUF_COMPILER_MOCK_CODE_GENERATOR_H__

#include <string>
#include <google/protobuf/compiler/code_generator.h>

namespace google {
namespace protobuf {
namespace compiler {

class MockCodeGenerator : public CodeGenerator {
 public:
  MockCodeGenerator(const string& name);
  virtual ~MockCodeGenerator();

  
  
  
  
  
  
  
  static void ExpectGenerated(const string& name,
                              const string& parameter,
                              const string& insertions,
                              const string& file,
                              const string& first_message_name,
                              const string& parsed_file_list,
                              const string& output_directory);

  // Get the name of the file which would be written by the given generator.
  static string GetOutputFileName(const string& generator_name,
                                  const FileDescriptor* file);
  static string GetOutputFileName(const string& generator_name,
                                  const string& file);

  

  virtual bool Generate(const FileDescriptor* file,
                        const string& parameter,
                        GeneratorContext* context,
                        string* error) const;

 private:
  string name_;

  static string GetOutputFileContent(const string& generator_name,
                                     const string& parameter,
                                     const FileDescriptor* file,
                                     GeneratorContext *context);
  static string GetOutputFileContent(const string& generator_name,
                                     const string& parameter,
                                     const string& file,
                                     const string& parsed_file_list,
                                     const string& first_message_name);
};

}  
}  

}  
#endif  
