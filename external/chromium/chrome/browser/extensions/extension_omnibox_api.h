// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_OMNIBOX_API_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_OMNIBOX_API_H_
#pragma once

#include "base/string16.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "chrome/browser/extensions/extension_function.h"

class ExtensionOmniboxEventRouter {
 public:
  
  
  static void OnInputStarted(
      Profile* profile, const std::string& extension_id);

  
  
  
  static bool OnInputChanged(
      Profile* profile, const std::string& extension_id,
      const std::string& input, int suggest_id);

  
  static void OnInputEntered(
      Profile* profile, const std::string& extension_id,
      const std::string& input);

  
  
  static void OnInputCancelled(
      Profile* profile, const std::string& extension_id);

 private:
  DISALLOW_COPY_AND_ASSIGN(ExtensionOmniboxEventRouter);
};

class OmniboxSendSuggestionsFunction : public SyncExtensionFunction {
 public:
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("omnibox.sendSuggestions");
};

class OmniboxSetDefaultSuggestionFunction : public SyncExtensionFunction {
 public:
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("omnibox.setDefaultSuggestion");
};

struct ExtensionOmniboxSuggestion {
  ExtensionOmniboxSuggestion();
  ~ExtensionOmniboxSuggestion();

  
  
  bool ReadStylesFromValue(const ListValue& value);

  
  string16 content;

  
  string16 description;

  
  ACMatchClassifications description_styles;
};

struct ExtensionOmniboxSuggestions {
  ExtensionOmniboxSuggestions();
  ~ExtensionOmniboxSuggestions();

  int request_id;
  std::vector<ExtensionOmniboxSuggestion> suggestions;

 private:
  
  DISALLOW_COPY_AND_ASSIGN(ExtensionOmniboxSuggestions);
};

void ApplyDefaultSuggestionForExtensionKeyword(
    Profile* profile,
    const TemplateURL* keyword,
    const string16& remaining_input,
    AutocompleteMatch* match);

#endif  
