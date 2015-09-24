// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_HISTORY_PROVIDER_H_
#define CHROME_BROWSER_AUTOCOMPLETE_HISTORY_PROVIDER_H_
#pragma once

#include "base/compiler_specific.h"
#include "chrome/browser/autocomplete/autocomplete.h"

namespace history {

class HistoryBackend;
class URLDatabase;
class URLRow;

}  

class HistoryProvider : public AutocompleteProvider {
 public:
  virtual void DeleteMatch(const AutocompleteMatch& match) OVERRIDE;

 protected:
  enum MatchType {
    NORMAL,
    WHAT_YOU_TYPED,
    INLINE_AUTOCOMPLETE
  };

  HistoryProvider(ACProviderListener* listener,
                  Profile* profile,
                  const char* name);

  
  
  
  
  
  
  
  
  
  
  static string16 FixupUserInput(const AutocompleteInput& input);

  
  
  
  
  static size_t TrimHttpPrefix(string16* url);
};

#endif  
