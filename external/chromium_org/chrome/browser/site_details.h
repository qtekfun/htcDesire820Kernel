// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SITE_DETAILS_H_
#define CHROME_BROWSER_SITE_DETAILS_H_

#include "base/containers/hash_tables.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/site_instance.h"
#include "content/public/browser/web_contents.h"

typedef base::hash_map<int32, std::set<GURL> > BrowsingInstanceSiteMap;

struct SiteData {
  SiteData();
  ~SiteData();

  std::set<GURL> sites;
  std::set<GURL> https_sites;
  std::vector<content::SiteInstance*> instances;
  BrowsingInstanceSiteMap instance_site_map;
  BrowsingInstanceSiteMap instance_https_site_map;
};

typedef base::hash_map<content::BrowserContext*, SiteData>
    BrowserContextSiteDataMap;

class SiteDetails {
 public:
  
  
  static void CollectSiteInfo(content::WebContents* contents,
                              SiteData* site_data);

  
  static void UpdateHistograms(const BrowserContextSiteDataMap& site_data_map,
                               int all_renderer_process_count,
                               int non_renderer_process_count);

 private:
  
  SiteDetails();
  ~SiteDetails();

  DISALLOW_COPY_AND_ASSIGN(SiteDetails);
};

#endif  
