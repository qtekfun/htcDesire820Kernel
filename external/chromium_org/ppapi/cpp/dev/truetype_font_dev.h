// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_TRUETYPE_FONT_H_
#define PPAPI_CPP_TRUETYPE_FONT_H_

#include "ppapi/c/dev/ppb_truetype_font_dev.h"
#include "ppapi/c/pp_time.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/pass_ref.h"
#include "ppapi/cpp/resource.h"


namespace pp {

class InstanceHandle;


class TrueTypeFontDesc_Dev {
 public:
  
  
  TrueTypeFontDesc_Dev();

  
  
  
  TrueTypeFontDesc_Dev(PassRef, const PP_TrueTypeFontDesc_Dev& pp_desc);

  
  
  
  TrueTypeFontDesc_Dev(const TrueTypeFontDesc_Dev& other);

  ~TrueTypeFontDesc_Dev();

  TrueTypeFontDesc_Dev& operator=(const TrueTypeFontDesc_Dev& other);

  const PP_TrueTypeFontDesc_Dev& pp_desc() const {
    return desc_;
  }

  Var family() const {
    return family_;
  }
  void set_family(const Var& family) {
    family_ = family;
    
    
    desc_.family = family_.pp_var();
  }

  PP_TrueTypeFontFamily_Dev generic_family() const {
    return desc_.generic_family;
  }
  void set_generic_family(PP_TrueTypeFontFamily_Dev family) {
    desc_.generic_family = family;
  }

  PP_TrueTypeFontStyle_Dev style() const { return desc_.style; }
  void set_style(PP_TrueTypeFontStyle_Dev style) {
    desc_.style = style;
  }

  PP_TrueTypeFontWeight_Dev weight() const { return desc_.weight; }
  void set_weight(PP_TrueTypeFontWeight_Dev weight) {
    desc_.weight = weight;
  }

  PP_TrueTypeFontWidth_Dev width() const { return desc_.width; }
  void set_width(PP_TrueTypeFontWidth_Dev width) {
    desc_.width = width;
  }

  PP_TrueTypeFontCharset_Dev charset() const { return desc_.charset; }
  void set_charset(PP_TrueTypeFontCharset_Dev charset) {
    desc_.charset = charset;
  }

 private:
  pp::Var family_;  
  PP_TrueTypeFontDesc_Dev desc_;
};


class TrueTypeFont_Dev : public Resource {
 public:
  
  TrueTypeFont_Dev();

  
  
  
  
  TrueTypeFont_Dev(const InstanceHandle& instance,
                   const TrueTypeFontDesc_Dev& desc);

  
  
  
  TrueTypeFont_Dev(const TrueTypeFont_Dev& other);

  
  
  
  
  TrueTypeFont_Dev(PassRef, PP_Resource resource);

  
  
  
  
  
  
  
  
  
  
  static int32_t GetFontFamilies(
      const InstanceHandle& instance,
      const CompletionCallbackWithOutput<std::vector<Var> >& callback);

  
  
  
  
  
  
  
  
  
  
  
  
  
  static int32_t GetFontsInFamily(
      const InstanceHandle& instance,
      const Var& family,
      const CompletionCallbackWithOutput<std::vector<TrueTypeFontDesc_Dev> >&
          callback);

  
  
  
  
  
  
  
  
  
  int32_t Describe(
      const CompletionCallbackWithOutput<TrueTypeFontDesc_Dev>& callback);

  
  
  
  
  
  
  
  
  int32_t GetTableTags(
      const CompletionCallbackWithOutput<std::vector<uint32_t> >& callback);

  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t GetTable(
      uint32_t table,
      int32_t offset,
      int32_t max_data_length,
      const CompletionCallbackWithOutput<std::vector<char> >& callback);
};

namespace internal {

 
template<>
struct CallbackOutputTraits<TrueTypeFontDesc_Dev> {
  typedef PP_TrueTypeFontDesc_Dev* APIArgType;
  typedef PP_TrueTypeFontDesc_Dev StorageType;

  static inline APIArgType StorageToAPIArg(StorageType& t) {
    return &t;
  }

  static inline TrueTypeFontDesc_Dev StorageToPluginArg(StorageType& t) {
    return TrueTypeFontDesc_Dev(PASS_REF, t);
  }

  static inline void Initialize(StorageType* t) {
    
    TrueTypeFontDesc_Dev dummy;
    *t = dummy.pp_desc();
  }
};

class TrueTypeFontDescArrayOutputAdapterWithStorage
    : public ArrayOutputAdapter<PP_TrueTypeFontDesc_Dev> {
 public:
  TrueTypeFontDescArrayOutputAdapterWithStorage() {
    set_output(&temp_storage_);
  };

  virtual ~TrueTypeFontDescArrayOutputAdapterWithStorage() {
    if (!temp_storage_.empty()) {
      
      output();
    }
  };

  std::vector<TrueTypeFontDesc_Dev>& output() {
    PP_DCHECK(output_storage_.empty());
    typedef std::vector<PP_TrueTypeFontDesc_Dev> Entries;
    for (Entries::iterator it = temp_storage_.begin();
         it != temp_storage_.end(); ++it)
      output_storage_.push_back(TrueTypeFontDesc_Dev(PASS_REF, *it));
    temp_storage_.clear();
    return output_storage_;
  }

 private:
  std::vector<PP_TrueTypeFontDesc_Dev> temp_storage_;
  std::vector<TrueTypeFontDesc_Dev> output_storage_;
};

template<>
struct CallbackOutputTraits< std::vector<TrueTypeFontDesc_Dev> > {
  typedef PP_ArrayOutput APIArgType;
  typedef TrueTypeFontDescArrayOutputAdapterWithStorage StorageType;

  static inline APIArgType StorageToAPIArg(StorageType& t) {
    return t.pp_array_output();
  }

  static inline std::vector<TrueTypeFontDesc_Dev>& StorageToPluginArg(
      StorageType& t) {
    return t.output();
  }

  static inline void Initialize(StorageType* ) {}
};

}  

}  

#endif  
