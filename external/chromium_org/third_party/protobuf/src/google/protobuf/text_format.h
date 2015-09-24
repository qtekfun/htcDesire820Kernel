// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_TEXT_FORMAT_H__
#define GOOGLE_PROTOBUF_TEXT_FORMAT_H__

#include <map>
#include <string>
#include <vector>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>

namespace google {
namespace protobuf {

namespace io {
  class ErrorCollector;      
}

class LIBPROTOBUF_EXPORT TextFormat {
 public:
  
  
  static bool Print(const Message& message, io::ZeroCopyOutputStream* output);

  
  
  
  static bool PrintUnknownFields(const UnknownFieldSet& unknown_fields,
                                 io::ZeroCopyOutputStream* output);

  
  static bool PrintToString(const Message& message, string* output);

  
  static bool PrintUnknownFieldsToString(const UnknownFieldSet& unknown_fields,
                                         string* output);

  
  
  
  
  static void PrintFieldValueToString(const Message& message,
                                      const FieldDescriptor* field,
                                      int index,
                                      string* output);

  
  
  class LIBPROTOBUF_EXPORT Printer {
   public:
    Printer();
    ~Printer();

    
    bool Print(const Message& message, io::ZeroCopyOutputStream* output) const;
    
    bool PrintUnknownFields(const UnknownFieldSet& unknown_fields,
                            io::ZeroCopyOutputStream* output) const;
    
    bool PrintToString(const Message& message, string* output) const;
    
    bool PrintUnknownFieldsToString(const UnknownFieldSet& unknown_fields,
                                    string* output) const;
    
    void PrintFieldValueToString(const Message& message,
                                 const FieldDescriptor* field,
                                 int index,
                                 string* output) const;

    
    
    void SetInitialIndentLevel(int indent_level) {
      initial_indent_level_ = indent_level;
    }

    
    
    void SetSingleLineMode(bool single_line_mode) {
      single_line_mode_ = single_line_mode;
    }

    
    
    
    
    
    void SetUseShortRepeatedPrimitives(bool use_short_repeated_primitives) {
      use_short_repeated_primitives_ = use_short_repeated_primitives;
    }

    
    
    
    
    void SetUseUtf8StringEscaping(bool as_utf8) {
      utf8_string_escaping_ = as_utf8;
    }

   private:
    
    
    class TextGenerator;

    
    
    void Print(const Message& message,
               TextGenerator& generator) const;

    
    void PrintField(const Message& message,
                    const Reflection* reflection,
                    const FieldDescriptor* field,
                    TextGenerator& generator) const;

    
    void PrintShortRepeatedField(const Message& message,
                                 const Reflection* reflection,
                                 const FieldDescriptor* field,
                                 TextGenerator& generator) const;

    
    
    void PrintFieldName(const Message& message,
                        const Reflection* reflection,
                        const FieldDescriptor* field,
                        TextGenerator& generator) const;

    
    
    void PrintFieldValue(const Message& message,
                         const Reflection* reflection,
                         const FieldDescriptor* field,
                         int index,
                         TextGenerator& generator) const;

    
    
    
    void PrintUnknownFields(const UnknownFieldSet& unknown_fields,
                            TextGenerator& generator) const;

    int initial_indent_level_;

    bool single_line_mode_;

    bool use_short_repeated_primitives_;

    bool utf8_string_escaping_;
  };

  
  // the given message object.  This function parses the format written
  
  static bool Parse(io::ZeroCopyInputStream* input, Message* output);
  
  static bool ParseFromString(const string& input, Message* output);

  
  
  static bool Merge(io::ZeroCopyInputStream* input, Message* output);
  
  static bool MergeFromString(const string& input, Message* output);

  
  
  
  static bool ParseFieldValueFromString(const string& input,
                                        const FieldDescriptor* field,
                                        Message* message);

  
  
  
  class LIBPROTOBUF_EXPORT Finder {
   public:
    virtual ~Finder();

    
    
    virtual const FieldDescriptor* FindExtension(
        Message* message,
        const string& name) const = 0;
  };

  
  struct ParseLocation {
    int line;
    int column;

    ParseLocation() : line(-1), column(-1) {}
    ParseLocation(int line_param, int column_param)
        : line(line_param), column(column_param) {}
  };

  
  
  class LIBPROTOBUF_EXPORT ParseInfoTree {
   public:
    ParseInfoTree();
    ~ParseInfoTree();

    
    
    
    ParseLocation GetLocation(const FieldDescriptor* field, int index) const;

    
    
    
    ParseInfoTree* GetTreeForNested(const FieldDescriptor* field,
                                    int index) const;

   private:
    
    friend class TextFormat;

    
    void RecordLocation(const FieldDescriptor* field, ParseLocation location);

    
    ParseInfoTree* CreateNested(const FieldDescriptor* field);

    
    typedef map<const FieldDescriptor*, vector<ParseLocation> > LocationMap;

    
    
    typedef map<const FieldDescriptor*, vector<ParseInfoTree*> > NestedMap;

    LocationMap locations_;
    NestedMap nested_;

    GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(ParseInfoTree);
  };

  
  class LIBPROTOBUF_EXPORT Parser {
   public:
    Parser();
    ~Parser();

    
    bool Parse(io::ZeroCopyInputStream* input, Message* output);
    
    bool ParseFromString(const string& input, Message* output);
    
    bool Merge(io::ZeroCopyInputStream* input, Message* output);
    
    bool MergeFromString(const string& input, Message* output);

    
    
    void RecordErrorsTo(io::ErrorCollector* error_collector) {
      error_collector_ = error_collector;
    }

    
    
    
    void SetFinder(Finder* finder) {
      finder_ = finder;
    }

    // Sets where location information about the parse will be written. If NULL
    // (the default), then no location will be written.
    void WriteLocationsTo(ParseInfoTree* tree) {
      parse_info_tree_ = tree;
    }

    
    
    void AllowPartialMessage(bool allow) {
      allow_partial_ = allow;
    }

    
    bool ParseFieldValueFromString(const string& input,
                                   const FieldDescriptor* field,
                                   Message* output);


   private:
    
    
    class ParserImpl;

    
    
    bool MergeUsingImpl(io::ZeroCopyInputStream* input,
                        Message* output,
                        ParserImpl* parser_impl);

    io::ErrorCollector* error_collector_;
    Finder* finder_;
    ParseInfoTree* parse_info_tree_;
    bool allow_partial_;
    bool allow_unknown_field_;
  };

 private:
  
  
  
  
  static inline void RecordLocation(ParseInfoTree* info_tree,
                                    const FieldDescriptor* field,
                                    ParseLocation location);
  static inline ParseInfoTree* CreateNested(ParseInfoTree* info_tree,
                                            const FieldDescriptor* field);

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(TextFormat);
};

inline void TextFormat::RecordLocation(ParseInfoTree* info_tree,
                                       const FieldDescriptor* field,
                                       ParseLocation location) {
  info_tree->RecordLocation(field, location);
}

inline TextFormat::ParseInfoTree* TextFormat::CreateNested(
    ParseInfoTree* info_tree, const FieldDescriptor* field) {
  return info_tree->CreateNested(field);
}

}  

}  
#endif  
