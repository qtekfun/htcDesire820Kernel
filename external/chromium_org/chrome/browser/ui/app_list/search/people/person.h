// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_SEARCH_PEOPLE_PERSON_H_
#define CHROME_BROWSER_UI_APP_LIST_SEARCH_PEOPLE_PERSON_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "url/gurl.h"

namespace base {
class DictionaryValue;
}

namespace app_list {

struct Person {
  
  
  static scoped_ptr<Person> Create(const base::DictionaryValue& dict);

  Person();
  ~Person();

  scoped_ptr<Person> Duplicate();

  
  
  
  
  std::string id;

  
  
  std::string owner_id;

  
  
  double interaction_rank;

  std::string display_name;
  std::string email;
  GURL image_url;

};

}  

#endif  
