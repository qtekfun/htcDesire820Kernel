// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_CLASSIFIER_H_
#define CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_CLASSIFIER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/string16.h"

class AutocompleteController;
struct AutocompleteMatch;
class GURL;
class Profile;

class AutocompleteClassifier {
 public:
  explicit AutocompleteClassifier(Profile* profile);
  virtual ~AutocompleteClassifier();

  
  
  
  
  
  
  
  
  
  
  
  
  void Classify(const string16& text,
                const string16& desired_tld,
                bool allow_exact_keyword_match,
                AutocompleteMatch* match,
                GURL* alternate_nav_url);

 private:
  scoped_ptr<AutocompleteController> controller_;

  
  
  bool inside_classify_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(AutocompleteClassifier);
};

#endif  
