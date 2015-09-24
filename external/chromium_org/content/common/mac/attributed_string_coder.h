// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_MAC_ATTRIBUTED_STRING_CODER_H_
#define CONTENT_COMMON_MAC_ATTRIBUTED_STRING_CODER_H_

#include <set>

#include "base/strings/string16.h"
#include "content/common/content_export.h"
#include "content/common/mac/font_descriptor.h"
#include "ipc/ipc_message_utils.h"
#include "ui/gfx/range/range.h"

#if __OBJC__
@class NSAttributedString;
@class NSDictionary;
#else
class NSAttributedString;
class NSDictionary;
#endif

namespace mac {

class CONTENT_EXPORT AttributedStringCoder {
 public:
  
  
  class FontAttribute {
   public:
    FontAttribute(NSDictionary* ns_attributes, gfx::Range effective_range);
    FontAttribute(FontDescriptor font, gfx::Range range);
    FontAttribute();
    ~FontAttribute();

    
    
    NSDictionary* ToAttributesDictionary() const;

    
    
    
    bool ShouldEncode() const;

    
    FontDescriptor font_descriptor() const { return font_descriptor_; }
    gfx::Range effective_range() const { return effective_range_; }

   private:
    FontDescriptor font_descriptor_;
    gfx::Range effective_range_;
  };

  
  
  class EncodedString {
   public:
    explicit EncodedString(base::string16 string);
    EncodedString();
    ~EncodedString();

    
    base::string16 string() const { return string_; }
    const std::vector<FontAttribute>& attributes() const {
      return attributes_;
    }
    std::vector<FontAttribute>* attributes() { return &attributes_; }

   private:
    
    base::string16 string_;
    
    std::vector<FontAttribute> attributes_;
  };

  
  
  static const EncodedString* Encode(NSAttributedString* str);

  
  static NSAttributedString* Decode(const EncodedString* str);

 private:
  AttributedStringCoder();
};

}  


namespace IPC {

template <>
struct ParamTraits<mac::AttributedStringCoder::EncodedString> {
  typedef mac::AttributedStringCoder::EncodedString param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<mac::AttributedStringCoder::FontAttribute> {
  typedef mac::AttributedStringCoder::FontAttribute param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

}  

#endif  
