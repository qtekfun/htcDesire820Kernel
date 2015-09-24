// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_PROVIDER_LISTENER_H_
#define CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_PROVIDER_LISTENER_H_

class AutocompleteProviderListener {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void OnProviderUpdate(bool updated_matches) = 0;

 protected:
  virtual ~AutocompleteProviderListener() {}
};

#endif  
