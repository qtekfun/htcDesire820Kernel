// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_NAVIGATION_CONTROLLER_H_
#define CONTENT_PUBLIC_BROWSER_NAVIGATION_CONTROLLER_H_

#include <map>
#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "content/common/content_export.h"
#include "content/public/browser/global_request_id.h"
#include "content/public/common/page_transition_types.h"
#include "content/public/common/referrer.h"
#include "url/gurl.h"

namespace base {

class RefCountedMemory;

}  

namespace content {

class BrowserContext;
class NavigationEntry;
class SessionStorageNamespace;
class WebContents;

typedef std::map<std::string, scoped_refptr<SessionStorageNamespace> >
    SessionStorageNamespaceMap;

class NavigationController {
 public:
  enum ReloadType {
    NO_RELOAD,                   
    RELOAD,                      
    RELOAD_IGNORING_CACHE,       
    RELOAD_ORIGINAL_REQUEST_URL  
  };

  
  enum LoadURLType {
    
    LOAD_TYPE_DEFAULT,

    
    
    LOAD_TYPE_BROWSER_INITIATED_HTTP_POST,

    
    
    
    
    LOAD_TYPE_DATA

    
  };

  
  enum UserAgentOverrideOption {
    
    
    UA_OVERRIDE_INHERIT,

    
    UA_OVERRIDE_FALSE,

    
    UA_OVERRIDE_TRUE

    
    
  };

  enum RestoreType {
    
    
    RESTORE_CURRENT_SESSION,

    
    RESTORE_LAST_SESSION_EXITED_CLEANLY,
    RESTORE_LAST_SESSION_CRASHED,
  };

  
  
  
  CONTENT_EXPORT static NavigationEntry* CreateNavigationEntry(
      const GURL& url,
      const Referrer& referrer,
      PageTransition transition,
      bool is_renderer_initiated,
      const std::string& extra_headers,
      BrowserContext* browser_context);

  
  struct CONTENT_EXPORT LoadURLParams {
    
    GURL url;

    
    LoadURLType load_type;

    
    
    PageTransition transition_type;

    
    int64 frame_tree_node_id;

    
    Referrer referrer;

    
    
    std::vector<GURL> redirect_chain;

    
    std::string extra_headers;

    
    
    bool is_renderer_initiated;

    
    
    UserAgentOverrideOption override_user_agent;

    
    
    
    GlobalRequestID transferred_global_request_id;

    
    
    GURL base_url_for_data_url;

    
    
    GURL virtual_url_for_data_url;

    
    
    
    scoped_refptr<base::RefCountedMemory> browser_initiated_post_data;

    
    bool can_load_local_resources;

    
    
    bool should_replace_current_entry;

    
    
    std::string frame_name;

    
    
    
    
    
    
    bool should_clear_history_list;

    explicit LoadURLParams(const GURL& url);
    ~LoadURLParams();

    
    LoadURLParams(const LoadURLParams& other);
    LoadURLParams& operator=(const LoadURLParams& other);
  };

  
  
  CONTENT_EXPORT static void DisablePromptOnRepost();

  virtual ~NavigationController() {}

  
  
  virtual WebContents* GetWebContents() const = 0;

  
  virtual BrowserContext* GetBrowserContext() const = 0;
  virtual void SetBrowserContext(BrowserContext* browser_context) = 0;

  
  
  
  
  
  
  virtual void Restore(int selected_navigation,
                       RestoreType type,
                       std::vector<NavigationEntry*>* entries) = 0;

  

  
  
  
  

  
  
  

  

  
  
  
  
  
  
  
  
  
  
  virtual NavigationEntry* GetActiveEntry() const = 0;

  
  
  
  
  
  virtual NavigationEntry* GetVisibleEntry() const = 0;

  
  
  
  virtual int GetCurrentEntryIndex() const = 0;

  
  
  virtual NavigationEntry* GetLastCommittedEntry() const = 0;

  
  virtual int GetLastCommittedEntryIndex() const = 0;

  
  virtual bool CanViewSource() const = 0;

  

  
  
  
  virtual int GetEntryCount() const = 0;

  virtual NavigationEntry* GetEntryAtIndex(int index) const = 0;

  
  
  virtual NavigationEntry* GetEntryAtOffset(int offset) const = 0;

  

  
  virtual void DiscardNonCommittedEntries() = 0;

  
  
  virtual NavigationEntry* GetPendingEntry() const = 0;

  
  
  virtual int GetPendingEntryIndex() const = 0;

  

  
  
  
  virtual NavigationEntry* GetTransientEntry() const = 0;

  
  
  
  
  
  
  
  virtual void SetTransientEntry(NavigationEntry* entry) = 0;

  

  
  
  virtual void LoadURL(const GURL& url,
                       const Referrer& referrer,
                       PageTransition type,
                       const std::string& extra_headers) = 0;

  
  
  virtual void LoadURLWithParams(const LoadURLParams& params) = 0;

  
  
  
  virtual void LoadIfNecessary() = 0;

  

  
  virtual bool CanGoBack() const = 0;
  virtual bool CanGoForward() const = 0;
  virtual bool CanGoToOffset(int offset) const = 0;
  virtual void GoBack() = 0;
  virtual void GoForward() = 0;

  
  virtual void GoToIndex(int index) = 0;

  
  
  virtual void GoToOffset(int offset) = 0;

  
  
  
  
  virtual void Reload(bool check_for_repost) = 0;

  
  virtual void ReloadIgnoringCache(bool check_for_repost) = 0;

  
  
  
  virtual void ReloadOriginalRequestURL(bool check_for_repost) = 0;

  

  
  
  
  virtual bool RemoveEntryAtIndex(int index) = 0;

  

  
  
#if !defined(OS_IOS)
  
  
  virtual const SessionStorageNamespaceMap&
      GetSessionStorageNamespaceMap() const = 0;

  
  
  virtual SessionStorageNamespace* GetDefaultSessionStorageNamespace() = 0;
#endif

  
  
  virtual void SetMaxRestoredPageID(int32 max_id) = 0;

  
  
  virtual int32 GetMaxRestoredPageID() const = 0;

  
  
  
  
  virtual bool NeedsReload() const = 0;

  
  
  virtual void SetNeedsReload() = 0;

  
  virtual void CancelPendingReload() = 0;
  
  virtual void ContinuePendingReload() = 0;

  
  
  virtual bool IsInitialNavigation() const = 0;

  
  
  
  virtual void NotifyEntryChanged(const NavigationEntry* entry, int index) = 0;

  
  
  virtual void CopyStateFrom(const NavigationController& source) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  virtual void CopyStateFromAndPrune(NavigationController* source,
                                     bool replace_entry) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual bool CanPruneAllButLastCommitted() = 0;

  
  
  
  
  virtual void PruneAllButLastCommitted() = 0;

  
  
  virtual void ClearAllScreenshots() = 0;

 private:
  
  friend class NavigationControllerImpl;
  NavigationController() {}
};

}  

#endif  
