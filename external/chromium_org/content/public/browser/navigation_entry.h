// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_NAVIGATION_ENTRY_H_
#define CONTENT_PUBLIC_BROWSER_NAVIGATION_ENTRY_H_

#include <string>

#include "base/memory/ref_counted_memory.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "content/common/content_export.h"
#include "content/public/common/page_transition_types.h"
#include "content/public/common/page_type.h"
#include "content/public/common/referrer.h"

class GURL;

namespace content {

class PageState;
struct FaviconStatus;
struct SSLStatus;

class NavigationEntry {
 public:
  virtual ~NavigationEntry() {}

  CONTENT_EXPORT static NavigationEntry* Create();
  CONTENT_EXPORT static NavigationEntry* Create(const NavigationEntry& copy);

  

  
  
  
  
  virtual int GetUniqueID() const = 0;

  
  virtual content::PageType GetPageType() const = 0;

  
  
  
  virtual void SetURL(const GURL& url) = 0;
  virtual const GURL& GetURL() const = 0;

  
  virtual void SetBaseURLForDataURL(const GURL& url) = 0;
  virtual const GURL& GetBaseURLForDataURL() const = 0;

  
  virtual void SetReferrer(const content::Referrer& referrer) = 0;
  virtual const content::Referrer& GetReferrer() const = 0;

  
  
  
  
  
  
  
  virtual void SetVirtualURL(const GURL& url) = 0;
  virtual const GURL& GetVirtualURL() const = 0;

  
  
  
  
  virtual void SetTitle(const base::string16& title) = 0;
  virtual const base::string16& GetTitle() const = 0;

  
  
  
  
  
  
  
  
  virtual void SetPageState(const PageState& state) = 0;
  virtual const PageState& GetPageState() const = 0;

  
  
  
  virtual void SetPageID(int page_id) = 0;
  virtual int32 GetPageID() const = 0;

  

  
  
  
  
  virtual const base::string16& GetTitleForDisplay(
      const std::string& languages) const = 0;

  
  
  virtual bool IsViewSourceMode() const = 0;

  

  
  
  virtual void SetTransitionType(content::PageTransition transition_type) = 0;
  virtual content::PageTransition GetTransitionType() const = 0;

  
  
  
  
  
  
  
  
  
  virtual const GURL& GetUserTypedURL() const = 0;

  
  
  
  
  
  
  
  
  virtual void SetHasPostData(bool has_post_data) = 0;
  virtual bool GetHasPostData() const = 0;

  
  virtual void SetPostID(int64 post_id) = 0;
  virtual int64 GetPostID() const = 0;

  
  
  
  
  
  
  
  virtual void SetBrowserInitiatedPostData(
      const base::RefCountedMemory* data) = 0;
  virtual const base::RefCountedMemory* GetBrowserInitiatedPostData() const = 0;

  
  virtual const FaviconStatus& GetFavicon() const = 0;
  virtual FaviconStatus& GetFavicon() = 0;

  
  virtual const SSLStatus& GetSSL() const = 0;
  virtual SSLStatus& GetSSL() = 0;

  
  virtual void SetOriginalRequestURL(const GURL& original_url) = 0;
  virtual const GURL& GetOriginalRequestURL() const = 0;

  
  virtual void SetIsOverridingUserAgent(bool override) = 0;
  virtual bool GetIsOverridingUserAgent() const = 0;

  
  
  
  
  
  
  
  
  
  
  virtual void SetTimestamp(base::Time timestamp) = 0;
  virtual base::Time GetTimestamp() const = 0;

  
  
  virtual void SetCanLoadLocalResources(bool allow) = 0;
  virtual bool GetCanLoadLocalResources() const = 0;

  
  
  
  virtual void SetFrameToNavigate(const std::string& frame_name) = 0;
  virtual const std::string& GetFrameToNavigate() const = 0;

  
  
  virtual void SetExtraData(const std::string& key,
                            const base::string16& data) = 0;
  
  virtual bool GetExtraData(const std::string& key,
                            base::string16* data) const = 0;
  
  virtual void ClearExtraData(const std::string& key) = 0;

  
  
  
  
  
  
  
  virtual void SetHttpStatusCode(int http_status_code) = 0;
  virtual int GetHttpStatusCode() const = 0;
};

}  

#endif  
