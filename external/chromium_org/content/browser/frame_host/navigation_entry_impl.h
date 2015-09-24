// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_FRAME_HOST_NAVIGATION_ENTRY_IMPL_H_
#define CONTENT_BROWSER_FRAME_HOST_NAVIGATION_ENTRY_IMPL_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "content/browser/site_instance_impl.h"
#include "content/public/browser/favicon_status.h"
#include "content/public/browser/global_request_id.h"
#include "content/public/browser/navigation_entry.h"
#include "content/public/common/page_state.h"
#include "content/public/common/ssl_status.h"

namespace content {

class CONTENT_EXPORT NavigationEntryImpl
    : public NON_EXPORTED_BASE(NavigationEntry) {
 public:
  static NavigationEntryImpl* FromNavigationEntry(NavigationEntry* entry);

  
  static int kInvalidBindings;

  NavigationEntryImpl();
  NavigationEntryImpl(SiteInstanceImpl* instance,
                      int page_id,
                      const GURL& url,
                      const Referrer& referrer,
                      const base::string16& title,
                      PageTransition transition_type,
                      bool is_renderer_initiated);
  virtual ~NavigationEntryImpl();

  
  virtual int GetUniqueID() const OVERRIDE;
  virtual PageType GetPageType() const OVERRIDE;
  virtual void SetURL(const GURL& url) OVERRIDE;
  virtual const GURL& GetURL() const OVERRIDE;
  virtual void SetBaseURLForDataURL(const GURL& url) OVERRIDE;
  virtual const GURL& GetBaseURLForDataURL() const OVERRIDE;
  virtual void SetReferrer(const Referrer& referrer) OVERRIDE;
  virtual const Referrer& GetReferrer() const OVERRIDE;
  virtual void SetVirtualURL(const GURL& url) OVERRIDE;
  virtual const GURL& GetVirtualURL() const OVERRIDE;
  virtual void SetTitle(const base::string16& title) OVERRIDE;
  virtual const base::string16& GetTitle() const OVERRIDE;
  virtual void SetPageState(const PageState& state) OVERRIDE;
  virtual const PageState& GetPageState() const OVERRIDE;
  virtual void SetPageID(int page_id) OVERRIDE;
  virtual int32 GetPageID() const OVERRIDE;
  virtual const base::string16& GetTitleForDisplay(
      const std::string& languages) const OVERRIDE;
  virtual bool IsViewSourceMode() const OVERRIDE;
  virtual void SetTransitionType(PageTransition transition_type) OVERRIDE;
  virtual PageTransition GetTransitionType() const OVERRIDE;
  virtual const GURL& GetUserTypedURL() const OVERRIDE;
  virtual void SetHasPostData(bool has_post_data) OVERRIDE;
  virtual bool GetHasPostData() const OVERRIDE;
  virtual void SetPostID(int64 post_id) OVERRIDE;
  virtual int64 GetPostID() const OVERRIDE;
  virtual void SetBrowserInitiatedPostData(
      const base::RefCountedMemory* data) OVERRIDE;
  virtual const base::RefCountedMemory*
      GetBrowserInitiatedPostData() const OVERRIDE;
  virtual const FaviconStatus& GetFavicon() const OVERRIDE;
  virtual FaviconStatus& GetFavicon() OVERRIDE;
  virtual const SSLStatus& GetSSL() const OVERRIDE;
  virtual SSLStatus& GetSSL() OVERRIDE;
  virtual void SetOriginalRequestURL(const GURL& original_url) OVERRIDE;
  virtual const GURL& GetOriginalRequestURL() const OVERRIDE;
  virtual void SetIsOverridingUserAgent(bool override) OVERRIDE;
  virtual bool GetIsOverridingUserAgent() const OVERRIDE;
  virtual void SetTimestamp(base::Time timestamp) OVERRIDE;
  virtual base::Time GetTimestamp() const OVERRIDE;
  virtual void SetCanLoadLocalResources(bool allow) OVERRIDE;
  virtual bool GetCanLoadLocalResources() const OVERRIDE;
  virtual void SetFrameToNavigate(const std::string& frame_name) OVERRIDE;
  virtual const std::string& GetFrameToNavigate() const OVERRIDE;
  virtual void SetExtraData(const std::string& key,
                            const base::string16& data) OVERRIDE;
  virtual bool GetExtraData(const std::string& key,
                            base::string16* data) const OVERRIDE;
  virtual void ClearExtraData(const std::string& key) OVERRIDE;
  virtual void SetHttpStatusCode(int http_status_code) OVERRIDE;
  virtual int GetHttpStatusCode() const OVERRIDE;

  
  
  void ResetForCommit();

  void set_unique_id(int unique_id) {
    unique_id_ = unique_id;
  }

  
  
  
  
  
  
  void set_site_instance(SiteInstanceImpl* site_instance);
  SiteInstanceImpl* site_instance() const {
    return site_instance_.get();
  }

  
  
  
  void SetBindings(int bindings);
  int bindings() const {
    return bindings_;
  }

  void set_page_type(PageType page_type) {
    page_type_ = page_type;
  }

  bool has_virtual_url() const {
    return !virtual_url_.is_empty();
  }

  bool update_virtual_url_with_url() const {
    return update_virtual_url_with_url_;
  }
  void set_update_virtual_url_with_url(bool update) {
    update_virtual_url_with_url_ = update;
  }

  
  void set_extra_headers(const std::string& extra_headers) {
    extra_headers_ = extra_headers;
  }
  const std::string& extra_headers() const {
    return extra_headers_;
  }

  
  
  void set_is_renderer_initiated(bool is_renderer_initiated) {
    is_renderer_initiated_ = is_renderer_initiated;
  }
  bool is_renderer_initiated() const {
    return is_renderer_initiated_;
  }

  void set_user_typed_url(const GURL& user_typed_url) {
    user_typed_url_ = user_typed_url;
  }

  
  enum RestoreType {
    
    RESTORE_LAST_SESSION_EXITED_CLEANLY,
    RESTORE_LAST_SESSION_CRASHED,

    
    
    RESTORE_CURRENT_SESSION,

    
    RESTORE_NONE
  };

  
  
  void set_restore_type(RestoreType type) {
    restore_type_ = type;
  }
  RestoreType restore_type() const {
    return restore_type_;
  }

  void set_transferred_global_request_id(
      const GlobalRequestID& transferred_global_request_id) {
    transferred_global_request_id_ = transferred_global_request_id;
  }

  GlobalRequestID transferred_global_request_id() const {
    return transferred_global_request_id_;
  }

  
  
  bool should_replace_entry() const {
    return should_replace_entry_;
  }

  void set_should_replace_entry(bool should_replace_entry) {
    should_replace_entry_ = should_replace_entry;
  }

  
  
  const std::vector<GURL>& redirect_chain() const {
    return redirect_chain_;
  }

  void set_redirect_chain(const std::vector<GURL>& redirect_chain) {
    redirect_chain_ = redirect_chain;
  }

  void SetScreenshotPNGData(scoped_refptr<base::RefCountedBytes> png_data);
  const scoped_refptr<base::RefCountedBytes> screenshot() const {
    return screenshot_;
  }

  
  
  bool should_clear_history_list() const {
    return should_clear_history_list_;
  }
  void set_should_clear_history_list(bool should_clear_history_list) {
    should_clear_history_list_ = should_clear_history_list;
  }

  
  
  int64 frame_tree_node_id() const {
    return frame_tree_node_id_;
  }
  void set_frame_tree_node_id(int64 frame_tree_node_id) {
    frame_tree_node_id_ = frame_tree_node_id;
  }

 private:
  
  
  
  
  
  

  
  int unique_id_;
  scoped_refptr<SiteInstanceImpl> site_instance_;
  
  int bindings_;
  PageType page_type_;
  GURL url_;
  Referrer referrer_;
  GURL virtual_url_;
  bool update_virtual_url_with_url_;
  base::string16 title_;
  FaviconStatus favicon_;
  PageState page_state_;
  int32 page_id_;
  SSLStatus ssl_;
  PageTransition transition_type_;
  GURL user_typed_url_;
  bool has_post_data_;
  int64 post_id_;
  RestoreType restore_type_;
  GURL original_request_url_;
  bool is_overriding_user_agent_;
  base::Time timestamp_;
  int http_status_code_;

  
  
  
  
  
  scoped_refptr<const base::RefCountedMemory> browser_initiated_post_data_;

  
  
  
  
  
  
  
  scoped_refptr<base::RefCountedBytes> screenshot_;

  
  std::string extra_headers_;

  
  
  GURL base_url_for_data_url_;

  
  
  
  bool is_renderer_initiated_;

  
  
  
  
  mutable base::string16 cached_display_title_;

  
  
  
  
  
  
  
  
  GlobalRequestID transferred_global_request_id_;

  
  
  
  
  
  
  
  
  
  bool should_replace_entry_;

  
  
  std::vector<GURL> redirect_chain_;

  
  
  
  
  
  bool should_clear_history_list_;

  
  
  bool can_load_local_resources_;

  
  
  std::string frame_to_navigate_;

  
  
  
  
  
  int64 frame_tree_node_id_;

  
  
  
  std::map<std::string, base::string16> extra_data_;

  
};

}  

#endif  
