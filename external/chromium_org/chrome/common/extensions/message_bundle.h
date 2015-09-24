// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_MESSAGE_BUNDLE_H_
#define CHROME_COMMON_EXTENSIONS_MESSAGE_BUNDLE_H_

#include <map>
#include <string>
#include <vector>

#include "base/memory/linked_ptr.h"

namespace base {
class DictionaryValue;
class Value;
}

namespace extensions {

class MessageBundle {
 public:
  typedef std::map<std::string, std::string> SubstitutionMap;
  typedef std::vector<linked_ptr<base::DictionaryValue> > CatalogVector;

  
  static const char* kContentKey;
  static const char* kMessageKey;
  static const char* kPlaceholdersKey;

  
  static const char* kPlaceholderBegin;
  static const char* kPlaceholderEnd;
  static const char* kMessageBegin;
  static const char* kMessageEnd;

  
  
  static const char* kUILocaleKey;
  
  
  
  static const char* kBidiDirectionKey;
  static const char* kBidiReversedDirectionKey;
  static const char* kBidiStartEdgeKey;
  static const char* kBidiEndEdgeKey;
  
  
  
  static const char* kExtensionIdKey;

  
  static const char* kBidiLeftEdgeValue;
  static const char* kBidiRightEdgeValue;

  
  
  
  static MessageBundle* Create(const CatalogVector& locale_catalogs,
                               std::string* error);

  
  
  
  
  
  
  std::string GetL10nMessage(const std::string& name) const;

  
  static std::string GetL10nMessage(const std::string& name,
                                    const SubstitutionMap& dictionary);

  
  
  size_t size() const { return dictionary_.size(); }

  
  
  
  bool ReplaceMessages(std::string* text, std::string* error) const;
  
  static bool ReplaceMessagesWithExternalDictionary(
      const SubstitutionMap& dictionary, std::string* text, std::string* error);

  
  
  
  
  
  static bool ReplaceVariables(const SubstitutionMap& variables,
                               const std::string& var_begin,
                               const std::string& var_end,
                               std::string* message,
                               std::string* error);

  
  
  static bool IsValidName(const std::string& name);

  
  const SubstitutionMap* dictionary() const { return &dictionary_; }

  ~MessageBundle();

 private:
  
  friend class MessageBundleTest;

  
  MessageBundle();

  
  
  
  
  bool Init(const CatalogVector& locale_catalogs, std::string* error);

  
  
  bool AppendReservedMessagesForLocale(const std::string& application_locale,
                                       std::string* error);

  
  
  
  bool GetMessageValue(const std::string& key,
                       const base::Value& name_value,
                       std::string* value,
                       std::string* error) const;

  
  bool GetPlaceholders(const base::DictionaryValue& name_tree,
                       const std::string& name_key,
                       SubstitutionMap* placeholders,
                       std::string* error) const;

  
  
  bool ReplacePlaceholders(const SubstitutionMap& placeholders,
                           std::string* message,
                           std::string* error) const;

  
  SubstitutionMap dictionary_;
};


typedef std::map<std::string, std::string> L10nMessagesMap;

typedef std::map<std::string, L10nMessagesMap > ExtensionToL10nMessagesMap;

ExtensionToL10nMessagesMap* GetExtensionToL10nMessagesMap();

L10nMessagesMap* GetL10nMessagesMap(const std::string& extension_id);

void EraseL10nMessagesMap(const std::string& extension_id);

}  

#endif  
