// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_TEST_INTEGRATION_DICTIONARY_LOAD_OBSERVER_H_
#define CHROME_BROWSER_SYNC_TEST_INTEGRATION_DICTIONARY_LOAD_OBSERVER_H_

#include "base/callback.h"
#include "chrome/browser/spellchecker/spellcheck_custom_dictionary.h"

class DictionaryLoadObserver : public SpellcheckCustomDictionary::Observer {
 public:
  explicit DictionaryLoadObserver(const base::Closure& quit_task);
  virtual ~DictionaryLoadObserver();

  
  virtual void OnCustomDictionaryLoaded() OVERRIDE;
  virtual void OnCustomDictionaryChanged(
      const SpellcheckCustomDictionary::Change& dictionary_change) OVERRIDE;

 private:
  base::Closure quit_task_;
  DISALLOW_COPY_AND_ASSIGN(DictionaryLoadObserver);
};

#endif  
