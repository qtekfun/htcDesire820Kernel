// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHARACTER_ENCODING_H_
#define CHROME_BROWSER_CHARACTER_ENCODING_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"

class CharacterEncoding {
 public:
  
  
  

  
  struct EncodingInfo {
    explicit EncodingInfo(int id);
    
    
    
    
    const base::string16& GetStringKey() const { return encoding_category_name; }

    
    int encoding_id;
    
    base::string16 encoding_display_name;
    
    base::string16 encoding_category_name;
  };

  
  
  
  static std::string GetCanonicalEncodingNameByCommandId(int id);

  
  
  
  static base::string16 GetCanonicalEncodingDisplayNameByCommandId(int id);

  
  static int GetSupportCanonicalEncodingCount();

  
  
  
  static std::string GetCanonicalEncodingNameByIndex(int index);

  
  
  
  
  static base::string16 GetCanonicalEncodingDisplayNameByIndex(int index);

  
  
  static int GetEncodingCommandIdByIndex(int index);

  
  
  static std::string GetCanonicalEncodingNameByAliasName(
      const std::string& alias_name);

  
  
  
  
  
  
  
  
  
  
  
  
  static const std::vector<EncodingInfo>* GetCurrentDisplayEncodings(
      const std::string& locale,
      const std::string& locale_encodings,
      const std::string& recently_select_encodings);

  
  
  
  
  static bool UpdateRecentlySelectedEncoding(
      const std::string& original_selected_encodings,
      int new_selected_encoding_id,
      std::string* selected_encodings);

  
  
  static int GetCommandIdByCanonicalEncodingName(
      const std::string& encoding_name);

 private:
  
  DISALLOW_IMPLICIT_CONSTRUCTORS(CharacterEncoding);
};

#endif  
