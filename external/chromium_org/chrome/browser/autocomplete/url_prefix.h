// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_URL_PREFIX_H_
#define CHROME_BROWSER_AUTOCOMPLETE_URL_PREFIX_H_

#include <vector>

#include "base/strings/string16.h"

struct URLPrefix;
typedef std::vector<URLPrefix> URLPrefixes;

struct URLPrefix {
  URLPrefix(const base::string16& prefix, size_t num_components);

  
  static const URLPrefixes& GetURLPrefixes();

  
  static bool IsURLPrefix(const base::string16& prefix);

  
  
  
  
  static const URLPrefix* BestURLPrefix(const base::string16& text,
                                        const base::string16& prefix_suffix);

  
  
  
  static bool PrefixMatch(const URLPrefix& prefix,
                          const base::string16& text,
                          const base::string16& prefix_suffix);

  base::string16 prefix;

  
  
  
  size_t num_components;
};

#endif  
