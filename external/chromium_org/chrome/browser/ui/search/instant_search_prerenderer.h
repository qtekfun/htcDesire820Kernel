// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SEARCH_INSTANT_SEARCH_PRERENDERER_H_
#define CHROME_BROWSER_UI_SEARCH_INSTANT_SEARCH_PRERENDERER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/common/instant_types.h"
#include "content/public/browser/navigation_controller.h"

class GURL;
class Profile;
struct AutocompleteMatch;

namespace chrome {
struct NavigateParams;
}

namespace content {
class WebContents;
}

namespace gfx {
class Size;
}

namespace prerender {
class PrerenderHandle;
}

class InstantSearchPrerenderer {
 public:
  InstantSearchPrerenderer(Profile* profile, const GURL& url);
  ~InstantSearchPrerenderer();

  
  static InstantSearchPrerenderer* GetForProfile(Profile* profile);

  
  
  
  
  
  
  void Init(
      const content::SessionStorageNamespaceMap& session_storage_namespace_map,
      const gfx::Size& size);

  
  void Cancel();

  
  void Prerender(const InstantSuggestion& suggestion);

  
  void Commit(const string16& query);

  
  
  bool CanCommitQuery(content::WebContents* source,
                      const string16& query) const;

  
  
  bool UsePrerenderedPage(const GURL& url, chrome::NavigateParams* params);

  
  const string16& get_last_query() const {
    return last_instant_suggestion_.text;
  }

  
  
  bool IsAllowed(const AutocompleteMatch& match,
                 content::WebContents* source) const;

 private:
  friend class InstantSearchPrerendererTest;

  content::WebContents* prerender_contents() const;

  Profile* const profile_;

  
  const GURL prerender_url_;

  scoped_ptr<prerender::PrerenderHandle> prerender_handle_;

  InstantSuggestion last_instant_suggestion_;

  DISALLOW_COPY_AND_ASSIGN(InstantSearchPrerenderer);
};

#endif  
