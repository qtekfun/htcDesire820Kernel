// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

// custom compilers written to support other languages.

#ifndef GOOGLE_PROTOBUF_COMPILER_COMMAND_LINE_INTERFACE_H__
#define GOOGLE_PROTOBUF_COMPILER_COMMAND_LINE_INTERFACE_H__

#include <google/protobuf/stubs/common.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <utility>

namespace google {
namespace protobuf {

class FileDescriptor;        
class DescriptorPool;        
class FileDescriptorProto;   
template<typename T> class RepeatedPtrField;  

namespace compiler {

class CodeGenerator;        
class GeneratorContext;      
class DiskSourceTree;       

class LIBPROTOC_EXPORT CommandLineInterface {
 public:
  CommandLineInterface();
  ~CommandLineInterface();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void RegisterGenerator(const string& flag_name,
                         CodeGenerator* generator,
                         const string& help_text);

  
  
  
  
  
  
  
  
  void RegisterGenerator(const string& flag_name,
                         const string& option_flag_name,
                         CodeGenerator* generator,
                         const string& help_text);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  // messages should be written to stderr.  If an error is fatal, the plugin
  
  void AllowPlugins(const string& exe_name_prefix);

  
  
  
  
  
  int Run(int argc, const char* const argv[]);

  
  
  
  
  
  
  
  
  void SetInputsAreProtoPathRelative(bool enable) {
    inputs_are_proto_path_relative_ = enable;
  }

  
  
  
  void SetVersionInfo(const string& text) {
    version_info_ = text;
  }


 private:
  

  class ErrorPrinter;
  class GeneratorContextImpl;
  class MemoryOutputStream;

  
  void Clear();

  
  
  
  bool MakeInputsBeProtoPathRelative(
    DiskSourceTree* source_tree);

  
  enum ParseArgumentStatus {
    PARSE_ARGUMENT_DONE_AND_CONTINUE,
    PARSE_ARGUMENT_DONE_AND_EXIT,
    PARSE_ARGUMENT_FAIL
  };

  
  ParseArgumentStatus ParseArguments(int argc, const char* const argv[]);

  
  
  
  
  
  
  
  
  
  
  
  bool ParseArgument(const char* arg, string* name, string* value);

  
  ParseArgumentStatus InterpretArgument(const string& name,
                                        const string& value);

  
  void PrintHelpText();

  
  struct OutputDirective;  
  bool GenerateOutput(const vector<const FileDescriptor*>& parsed_files,
                      const OutputDirective& output_directive,
                      GeneratorContext* generator_context);
  bool GeneratePluginOutput(const vector<const FileDescriptor*>& parsed_files,
                            const string& plugin_name,
                            const string& parameter,
                            GeneratorContext* generator_context,
                            string* error);

  
  bool EncodeOrDecode(const DescriptorPool* pool);

  
  bool WriteDescriptorSet(const vector<const FileDescriptor*> parsed_files);

  
  
  
  
  
  
  
  static void GetTransitiveDependencies(
      const FileDescriptor* file,
      bool include_source_code_info,
      set<const FileDescriptor*>* already_seen,
      RepeatedPtrField<FileDescriptorProto>* output);

  

  
  string executable_name_;

  
  string version_info_;

  
  struct GeneratorInfo {
    string flag_name;
    string option_flag_name;
    CodeGenerator* generator;
    string help_text;
  };
  typedef map<string, GeneratorInfo> GeneratorMap;
  GeneratorMap generators_by_flag_name_;
  GeneratorMap generators_by_option_name_;
  
  
  
  
  map<string, string> generator_parameters_;

  
  string plugin_prefix_;

  
  
  
  map<string, string> plugins_;

  
  enum Mode {
    MODE_COMPILE,  
    MODE_ENCODE,   
    MODE_DECODE    
  };

  Mode mode_;

  enum ErrorFormat {
    ERROR_FORMAT_GCC,   
    ERROR_FORMAT_MSVS   
  };

  ErrorFormat error_format_;

  vector<pair<string, string> > proto_path_;  
  vector<string> input_files_;                

  
  
  struct OutputDirective {
    string name;                
    CodeGenerator* generator;   
    string parameter;
    string output_location;
  };
  vector<OutputDirective> output_directives_;

  
  
  string codec_type_;

  
  // FileDescriptorSet should be written.  Otherwise, empty.
  string descriptor_set_name_;

  
  
  
  bool imports_in_descriptor_set_;

  
  
  bool source_info_in_descriptor_set_;

  
  bool disallow_services_;

  
  bool inputs_are_proto_path_relative_;

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(CommandLineInterface);
};

}  
}  

}  
#endif  
