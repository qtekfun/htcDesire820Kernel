// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_FRAME_HOST_NAVIGATION_CONTROLLER_IMPL_H_
#define CONTENT_BROWSER_FRAME_HOST_NAVIGATION_CONTROLLER_IMPL_H_

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/linked_ptr.h"
#include "base/time/time.h"
#include "build/build_config.h"
#include "content/browser/frame_host/navigation_controller_delegate.h"
#include "content/browser/ssl/ssl_manager.h"
#include "content/public/browser/navigation_controller.h"
#include "content/public/browser/navigation_type.h"

struct ViewHostMsg_FrameNavigate_Params;

namespace content {
class NavigationEntryImpl;
class RenderViewHost;
class NavigationEntryScreenshotManager;
class SiteInstance;
struct LoadCommittedDetails;

class CONTENT_EXPORT NavigationControllerImpl
    : public NON_EXPORTED_BASE(NavigationController) {
 public:
  NavigationControllerImpl(
      NavigationControllerDelegate* delegate,
      BrowserContext* browser_context);
  virtual ~NavigationControllerImpl();

  
  virtual WebContents* GetWebContents() const OVERRIDE;
  virtual BrowserContext* GetBrowserContext() const OVERRIDE;
  virtual void SetBrowserContext(
      BrowserContext* browser_context) OVERRIDE;
  virtual void Restore(
      int selected_navigation,
      RestoreType type,
      std::vector<NavigationEntry*>* entries) OVERRIDE;
  virtual NavigationEntry* GetActiveEntry() const OVERRIDE;
  virtual NavigationEntry* GetVisibleEntry() const OVERRIDE;
  virtual int GetCurrentEntryIndex() const OVERRIDE;
  virtual NavigationEntry* GetLastCommittedEntry() const OVERRIDE;
  virtual int GetLastCommittedEntryIndex() const OVERRIDE;
  virtual bool CanViewSource() const OVERRIDE;
  virtual int GetEntryCount() const OVERRIDE;
  virtual NavigationEntry* GetEntryAtIndex(int index) const OVERRIDE;
  virtual NavigationEntry* GetEntryAtOffset(int offset) const OVERRIDE;
  virtual void DiscardNonCommittedEntries() OVERRIDE;
  virtual NavigationEntry* GetPendingEntry() const OVERRIDE;
  virtual int GetPendingEntryIndex() const OVERRIDE;
  virtual NavigationEntry* GetTransientEntry() const OVERRIDE;
  virtual void SetTransientEntry(NavigationEntry* entry) OVERRIDE;
  virtual void LoadURL(const GURL& url,
                       const Referrer& referrer,
                       PageTransition type,
                       const std::string& extra_headers) OVERRIDE;
  virtual void LoadURLWithParams(const LoadURLParams& params) OVERRIDE;
  virtual void LoadIfNecessary() OVERRIDE;
  virtual bool CanGoBack() const OVERRIDE;
  virtual bool CanGoForward() const OVERRIDE;
  virtual bool CanGoToOffset(int offset) const OVERRIDE;
  virtual void GoBack() OVERRIDE;
  virtual void GoForward() OVERRIDE;
  virtual void GoToIndex(int index) OVERRIDE;
  virtual void GoToOffset(int offset) OVERRIDE;
  virtual bool RemoveEntryAtIndex(int index) OVERRIDE;
  virtual const SessionStorageNamespaceMap&
      GetSessionStorageNamespaceMap() const OVERRIDE;
  virtual SessionStorageNamespace*
      GetDefaultSessionStorageNamespace() OVERRIDE;
  virtual void SetMaxRestoredPageID(int32 max_id) OVERRIDE;
  virtual int32 GetMaxRestoredPageID() const OVERRIDE;
  virtual bool NeedsReload() const OVERRIDE;
  virtual void SetNeedsReload() OVERRIDE;
  virtual void CancelPendingReload() OVERRIDE;
  virtual void ContinuePendingReload() OVERRIDE;
  virtual bool IsInitialNavigation() const OVERRIDE;
  virtual void Reload(bool check_for_repost) OVERRIDE;
  virtual void ReloadIgnoringCache(bool check_for_repost) OVERRIDE;
  virtual void ReloadOriginalRequestURL(bool check_for_repost) OVERRIDE;
  virtual void NotifyEntryChanged(const NavigationEntry* entry,
                                 int index) OVERRIDE;
  virtual void CopyStateFrom(
      const NavigationController& source) OVERRIDE;
  virtual void CopyStateFromAndPrune(NavigationController* source,
                                     bool replace_entry) OVERRIDE;
  virtual bool CanPruneAllButLastCommitted() OVERRIDE;
  virtual void PruneAllButLastCommitted() OVERRIDE;
  virtual void ClearAllScreenshots() OVERRIDE;

  
  
  SessionStorageNamespace* GetSessionStorageNamespace(
      SiteInstance* instance);

  
  
  int GetIndexOfEntry(const NavigationEntryImpl* entry) const;

  
  
  int GetEntryIndexWithPageID(SiteInstance* instance,
                              int32 page_id) const;

  
  
  NavigationEntryImpl* GetEntryWithPageID(
      SiteInstance* instance,
      int32 page_id) const;

  NavigationControllerDelegate* delegate() const {
    return delegate_;
  }

  

  
  
  void SetPendingEntry(content::NavigationEntryImpl* entry);

  
  
  
  
  
  
  
  
  
  
  bool RendererDidNavigate(const ViewHostMsg_FrameNavigate_Params& params,
                           LoadCommittedDetails* details);

  
  
  void SetActive(bool is_active);

  
  
  
  
  
  
  
  
  
  
  
  
  bool IsURLInPageNavigation(const GURL& url) const {
    return IsURLInPageNavigation(url, false, NAVIGATION_TYPE_UNKNOWN);
  }

  
  
  
  
  
  bool IsURLInPageNavigation(
      const GURL& url,
      bool renderer_says_in_page,
      NavigationType navigation_type) const;

  
  
  
  
  
  
  
  
  void SetSessionStorageNamespace(
      const std::string& partition_id,
      SessionStorageNamespace* session_storage_namespace);

  

  SSLManager* ssl_manager() { return &ssl_manager_; }

  
  static void set_max_entry_count_for_testing(size_t max_entry_count) {
    max_entry_count_for_testing_ = max_entry_count;
  }
  static size_t max_entry_count();

  void SetGetTimestampCallbackForTest(
      const base::Callback<base::Time()>& get_timestamp_callback);

  
  void TakeScreenshot();

  
  
  
  
  
  void SetScreenshotManager(NavigationEntryScreenshotManager* manager);

  
  void DiscardPendingEntry();

 private:
  friend class RestoreHelper;

  FRIEND_TEST_ALL_PREFIXES(NavigationControllerTest,
                           PurgeScreenshot);
  FRIEND_TEST_ALL_PREFIXES(TimeSmoother, Basic);
  FRIEND_TEST_ALL_PREFIXES(TimeSmoother, SingleDuplicate);
  FRIEND_TEST_ALL_PREFIXES(TimeSmoother, ManyDuplicates);
  FRIEND_TEST_ALL_PREFIXES(TimeSmoother, ClockBackwardsJump);

  
  
  class CONTENT_EXPORT TimeSmoother {
   public:
    
    base::Time GetSmoothedTime(base::Time t);

   private:
    
    
    base::Time low_water_mark_;
    base::Time high_water_mark_;
  };

  
  NavigationType ClassifyNavigation(
      const ViewHostMsg_FrameNavigate_Params& params) const;

  
  
  
  void LoadEntry(NavigationEntryImpl* entry);

  
  
  
  
  
  
  
  
  
  
  
  
  void RendererDidNavigateToNewPage(
      const ViewHostMsg_FrameNavigate_Params& params, bool replace_entry);
  void RendererDidNavigateToExistingPage(
      const ViewHostMsg_FrameNavigate_Params& params);
  void RendererDidNavigateToSamePage(
      const ViewHostMsg_FrameNavigate_Params& params);
  void RendererDidNavigateInPage(
      const ViewHostMsg_FrameNavigate_Params& params, bool* did_replace_entry);
  void RendererDidNavigateNewSubframe(
      const ViewHostMsg_FrameNavigate_Params& params);
  bool RendererDidNavigateAutoSubframe(
      const ViewHostMsg_FrameNavigate_Params& params);

  
  void ReloadInternal(bool check_for_repost, ReloadType reload_type);

  
  void NavigateToPendingEntry(ReloadType reload_type);

  
  
  void NotifyNavigationEntryCommitted(LoadCommittedDetails* details);

  
  
  void UpdateVirtualURLToURL(NavigationEntryImpl* entry,
                             const GURL& new_url);

  
  
  
  void FinishRestore(int selected_index, RestoreType type);

  
  
  void InsertOrReplaceEntry(NavigationEntryImpl* entry, bool replace);

  
  void RemoveEntryAtIndexInternal(int index);

  
  void DiscardNonCommittedEntriesInternal();

  
  void DiscardTransientEntry();

  
  
  void PruneOldestEntryIfFull();

  
  
  
  
  
  void PruneAllButLastCommittedInternal();

  
  
  bool IsLikelyAutoNavigation(base::TimeTicks now);

  
  
  
  
  void InsertEntriesFrom(const NavigationControllerImpl& source, int max_index);

  
  
  int GetIndexForOffset(int offset) const;

  

  
  BrowserContext* browser_context_;

  
  typedef std::vector<linked_ptr<NavigationEntryImpl> > NavigationEntries;
  NavigationEntries entries_;

  
  
  
  
  
  
  
  NavigationEntryImpl* pending_entry_;

  
  int last_committed_entry_index_;

  
  
  int pending_entry_index_;

  
  
  
  
  
  int transient_entry_index_;

  
  
  NavigationControllerDelegate* delegate_;

  
  
  
  int32 max_restored_page_id_;

  
  SSLManager ssl_manager_;

  
  bool needs_reload_;

  
  
  bool is_initial_navigation_;

  
  
  
  
  
  
  
  SessionStorageNamespaceMap session_storage_namespace_map_;

  
  static size_t max_entry_count_for_testing_;

  
  
  ReloadType pending_reload_;

  
  base::Callback<base::Time()> get_timestamp_callback_;

  
  
  
  
  
  TimeSmoother time_smoother_;

  scoped_ptr<NavigationEntryScreenshotManager> screenshot_manager_;

  DISALLOW_COPY_AND_ASSIGN(NavigationControllerImpl);
};

}  

#endif  
