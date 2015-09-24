// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_COMPILER_PARSER_H__
#define GOOGLE_PROTOBUF_COMPILER_PARSER_H__

#include <map>
#include <string>
#include <utility>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/io/tokenizer.h>

namespace google {
namespace protobuf { class Message; }

namespace protobuf {
namespace compiler {

class Parser;
class SourceLocationTable;

class LIBPROTOBUF_EXPORT Parser {
 public:
  Parser();
  ~Parser();

  
  
  bool Parse(io::Tokenizer* input, FileDescriptorProto* file);

  

  
  
  
  
  
  
  
  void RecordSourceLocationsTo(SourceLocationTable* location_table) {
    source_location_table_ = location_table;
  }

  
  
  void RecordErrorsTo(io::ErrorCollector* error_collector) {
    error_collector_ = error_collector;
  }

  
  
  const string& GetSyntaxIdentifier() { return syntax_identifier_; }

  
  
  
  
  void SetRequireSyntaxIdentifier(bool value) {
    require_syntax_identifier_ = value;
  }

  
  
  
  
  
  
  
  
  void SetStopAfterSyntaxIdentifier(bool value) {
    stop_after_syntax_identifier_ = value;
  }

 private:
  class LocationRecorder;

  
  

  
  
  
  
  
  
  
  
  
  void SkipStatement();

  
  
  void SkipRestOfBlock();

  
  
  
  

  
  inline bool AtEnd();

  
  inline bool LookingAt(const char* text);
  
  inline bool LookingAtType(io::Tokenizer::TokenType token_type);

  
  
  bool TryConsume(const char* text);

  
  
  

  
  bool Consume(const char* text, const char* error);
  
  
  bool Consume(const char* text);
  
  bool ConsumeIdentifier(string* output, const char* error);
  
  bool ConsumeInteger(int* output, const char* error);
  
  bool ConsumeSignedInteger(int* output, const char* error);
  
  
  bool ConsumeInteger64(uint64 max_value, uint64* output, const char* error);
  
  
  bool ConsumeNumber(double* output, const char* error);
  
  bool ConsumeString(string* output, const char* error);

  
  
  
  
  
  
  
  
  
  bool TryConsumeEndOfDeclaration(const char* text,
                                  const LocationRecorder* location);
  bool ConsumeEndOfDeclaration(const char* text,
                               const LocationRecorder* location);

  
  

  
  void AddError(int line, int column, const string& error);

  
  
  void AddError(const string& error);

  
  
  
  
  
  class LIBPROTOBUF_EXPORT LocationRecorder {
   public:
    
    LocationRecorder(Parser* parser);

    
    
    
    
    
    LocationRecorder(const LocationRecorder& parent);

    
    LocationRecorder(const LocationRecorder& parent, int path1);
    LocationRecorder(const LocationRecorder& parent, int path1, int path2);

    ~LocationRecorder();

    
    
    void AddPath(int path_component);

    
    
    
    void StartAt(const io::Tokenizer::Token& token);

    
    
    
    void EndAt(const io::Tokenizer::Token& token);

    
    
    
    
    void RecordLegacyLocation(const Message* descriptor,
        DescriptorPool::ErrorCollector::ErrorLocation location);

    
    
    
    
    
    
    void AttachComments(string* leading, string* trailing) const;

   private:
    Parser* parser_;
    SourceCodeInfo::Location* location_;

    void Init(const LocationRecorder& parent);
  };

  
  

  
  
  
  bool ParseSyntaxIdentifier();

  
  
  
  
  
  
  
  
  

  
  bool ParseTopLevelStatement(FileDescriptorProto* file,
                              const LocationRecorder& root_location);

  
  bool ParseMessageDefinition(DescriptorProto* message,
                              const LocationRecorder& message_location);
  bool ParseEnumDefinition(EnumDescriptorProto* enum_type,
                           const LocationRecorder& enum_location);
  bool ParseServiceDefinition(ServiceDescriptorProto* service,
                              const LocationRecorder& service_location);
  bool ParsePackage(FileDescriptorProto* file,
                    const LocationRecorder& root_location);
  bool ParseImport(RepeatedPtrField<string>* dependency,
                   RepeatedField<int32>* public_dependency,
                   RepeatedField<int32>* weak_dependency,
                   const LocationRecorder& root_location);
  bool ParseOption(Message* options,
                   const LocationRecorder& options_location);

  
  
  
  bool ParseMessageBlock(DescriptorProto* message,
                         const LocationRecorder& message_location);
  bool ParseEnumBlock(EnumDescriptorProto* enum_type,
                      const LocationRecorder& enum_location);
  bool ParseServiceBlock(ServiceDescriptorProto* service,
                         const LocationRecorder& service_location);

  
  
  bool ParseMessageStatement(DescriptorProto* message,
                             const LocationRecorder& message_location);
  bool ParseEnumStatement(EnumDescriptorProto* message,
                          const LocationRecorder& enum_location);
  bool ParseServiceStatement(ServiceDescriptorProto* message,
                             const LocationRecorder& service_location);

  
  
  
  
  
  
  
  
  bool ParseMessageField(FieldDescriptorProto* field,
                         RepeatedPtrField<DescriptorProto>* messages,
                         const LocationRecorder& parent_location,
                         int location_field_number_for_nested_type,
                         const LocationRecorder& field_location);

  
  bool ParseExtensions(DescriptorProto* message,
                       const LocationRecorder& extensions_location);

  
  
  bool ParseExtend(RepeatedPtrField<FieldDescriptorProto>* extensions,
                   RepeatedPtrField<DescriptorProto>* messages,
                   const LocationRecorder& parent_location,
                   int location_field_number_for_nested_type,
                   const LocationRecorder& extend_location);

  
  bool ParseEnumConstant(EnumValueDescriptorProto* enum_value,
                         const LocationRecorder& enum_value_location);

  
  
  bool ParseEnumConstantOptions(EnumValueDescriptorProto* value,
                                const LocationRecorder& enum_value_location);

  
  bool ParseServiceMethod(MethodDescriptorProto* method,
                          const LocationRecorder& method_location);


  
  bool ParseOptions(const LocationRecorder& parent_location,
                    const int optionsFieldNumber,
                    Message* mutable_options);

  
  
  bool ParseLabel(FieldDescriptorProto::Label* label);

  
  
  bool ParseType(FieldDescriptorProto::Type* type,
                 string* type_name);
  
  
  bool ParseUserDefinedType(string* type_name);

  
  
  bool ParseFieldOptions(FieldDescriptorProto* field,
                         const LocationRecorder& field_location);

  
  
  bool ParseDefaultAssignment(FieldDescriptorProto* field,
                              const LocationRecorder& field_location);

  enum OptionStyle {
    OPTION_ASSIGNMENT,  
    OPTION_STATEMENT    
  };

  
  
  
  bool ParseOption(Message* options,
                   const LocationRecorder& options_location,
                   OptionStyle style);

  
  
  
  
  bool ParseOptionNamePart(UninterpretedOption* uninterpreted_option,
                           const LocationRecorder& part_location);

  
  
  
  
  
  
  
  
  
  
  bool ParseUninterpretedBlock(string* value);

  

  io::Tokenizer* input_;
  io::ErrorCollector* error_collector_;
  SourceCodeInfo* source_code_info_;
  SourceLocationTable* source_location_table_;  
  bool had_errors_;
  bool require_syntax_identifier_;
  bool stop_after_syntax_identifier_;
  string syntax_identifier_;

  
  
  string upcoming_doc_comments_;

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(Parser);
};

class LIBPROTOBUF_EXPORT SourceLocationTable {
 public:
  SourceLocationTable();
  ~SourceLocationTable();

  
  
  
  
  
  bool Find(const Message* descriptor,
            DescriptorPool::ErrorCollector::ErrorLocation location,
            int* line, int* column) const;

  
  void Add(const Message* descriptor,
           DescriptorPool::ErrorCollector::ErrorLocation location,
           int line, int column);

  
  void Clear();

 private:
  typedef map<
    pair<const Message*, DescriptorPool::ErrorCollector::ErrorLocation>,
    pair<int, int> > LocationMap;
  LocationMap location_map_;
};

}  
}  

}  
#endif  
