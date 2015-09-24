// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DISCOVERY_SUGGESTED_LINKS_REGISTRY_H_
#define CHROME_BROWSER_EXTENSIONS_API_DISCOVERY_SUGGESTED_LINKS_REGISTRY_H_

#include <map>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/api/discovery/suggested_link.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

namespace extensions {

class SuggestedLinksRegistry : public BrowserContextKeyedService {
 public:
  
  typedef std::vector<linked_ptr<extensions::SuggestedLink> > SuggestedLinkList;

  SuggestedLinksRegistry();

  
  
  void Add(const std::string& extension_id,
      scoped_ptr<extensions::SuggestedLink> item);

  
  scoped_ptr<std::vector<std::string> > GetExtensionIds() const;

  
  const SuggestedLinkList* GetAll(const std::string& extension_id) const;

  
  void Remove(const std::string& extension_id, const std::string& link_url);

  
  void ClearAll(const std::string& extension_id);

 private:
  
  typedef std::map<std::string, SuggestedLinkList> SuggestedLinksMap;

  virtual ~SuggestedLinksRegistry();

  
  SuggestedLinkList& GetAllInternal(const std::string& extension_id);

  SuggestedLinksMap suggested_links_;

  
  
  SuggestedLinkList empty_list_;

  DISALLOW_COPY_AND_ASSIGN(SuggestedLinksRegistry);
};

}  

#endif  
