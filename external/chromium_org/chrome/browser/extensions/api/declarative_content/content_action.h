// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_CONTENT_CONTENT_ACTION_H_
#define CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_CONTENT_CONTENT_ACTION_H_

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "chrome/browser/extensions/api/declarative/declarative_rule.h"

class Profile;

namespace base {
class Time;
class Value;
}

namespace content {
class WebContents;
}

namespace extensions {
class Extension;

class ContentAction : public base::RefCounted<ContentAction> {
 public:
  
  enum Type {
    ACTION_SHOW_PAGE_ACTION,
  };

  struct ApplyInfo {
    Profile* profile;
    content::WebContents* tab;
  };

  ContentAction();

  virtual Type GetType() const = 0;

  
  
  
  virtual void Apply(const std::string& extension_id,
                     const base::Time& extension_install_time,
                     ApplyInfo* apply_info) const = 0;
  virtual void Revert(const std::string& extension_id,
                      const base::Time& extension_install_time,
                      ApplyInfo* apply_info) const = 0;

  
  
  
  
  
  
  static scoped_refptr<ContentAction> Create(const Extension* extension,
                                             const base::Value& json_action,
                                             std::string* error,
                                             bool* bad_message);

 protected:
  friend class base::RefCounted<ContentAction>;
  virtual ~ContentAction();
};

typedef DeclarativeActionSet<ContentAction> ContentActionSet;

}  

#endif  
