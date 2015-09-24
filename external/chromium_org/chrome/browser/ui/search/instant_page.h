// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SEARCH_INSTANT_PAGE_H_
#define CHROME_BROWSER_UI_SEARCH_INSTANT_PAGE_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/ui/search/instant_ipc_sender.h"
#include "chrome/browser/ui/search/search_model_observer.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/common/page_transition_types.h"

class GURL;
class Profile;

namespace content {
struct FrameNavigateParams;
struct LoadCommittedDetails;
class WebContents;
}

namespace gfx {
class Rect;
}

class InstantPage : public content::WebContentsObserver,
                    public SearchModelObserver {
 public:
  
  
  
  class Delegate {
   public:
    
    
    virtual void InstantSupportDetermined(const content::WebContents* contents,
                                          bool supports_instant) = 0;

    
    virtual void InstantPageAboutToNavigateMainFrame(
        const content::WebContents* contents,
        const GURL& url) = 0;

    
    virtual void InstantPageLoadFailed(content::WebContents* contents) = 0;

   protected:
    virtual ~Delegate();
  };

  virtual ~InstantPage();

  
  content::WebContents* contents() const { return web_contents(); }

  
  InstantIPCSender* sender() const { return ipc_sender_.get(); }

  
  
  virtual const std::string& instant_url() const;

  
  
  
  
  virtual bool supports_instant() const;

  
  
  virtual bool IsLocal() const;

 protected:
  InstantPage(Delegate* delegate, const std::string& instant_url,
              Profile* profile, bool is_incognito);

  
  
  void SetContents(content::WebContents* web_contents);

  Delegate* delegate() const { return delegate_; }

  Profile* profile() const { return profile_; }

  
  
  
  
  virtual bool ShouldProcessAboutToNavigateMainFrame();

 private:
  FRIEND_TEST_ALL_PREFIXES(InstantPageTest, IsLocal);
  FRIEND_TEST_ALL_PREFIXES(InstantPageTest,
                           DetermineIfPageSupportsInstant_Local);
  FRIEND_TEST_ALL_PREFIXES(InstantPageTest,
                           DetermineIfPageSupportsInstant_NonLocal);
  FRIEND_TEST_ALL_PREFIXES(InstantPageTest,
                           PageURLDoesntBelongToInstantRenderer);
  FRIEND_TEST_ALL_PREFIXES(InstantPageTest, PageSupportsInstant);
  FRIEND_TEST_ALL_PREFIXES(InstantPageTest,
                           AppropriateMessagesSentToIncognitoPages);

  
  virtual void DidCommitProvisionalLoadForFrame(
      int64 frame_id,
      const base::string16& frame_unique_name,
      bool is_main_frame,
      const GURL& url,
      content::PageTransition transition_type,
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;
  virtual void DidFailProvisionalLoad(
      int64 frame_id,
      const base::string16& frame_unique_name,
      bool is_main_frame,
      const GURL& validated_url,
      int error_code,
      const base::string16& error_description,
      content::RenderViewHost* render_view_host) OVERRIDE;

  
  virtual void ModelChanged(const SearchModel::State& old_state,
                            const SearchModel::State& new_state) OVERRIDE;

  
  void InstantSupportDetermined(bool supports_instant);

  void ClearContents();

  
  
  Profile* profile_;

  Delegate* const delegate_;
  scoped_ptr<InstantIPCSender> ipc_sender_;
  const std::string instant_url_;
  const bool is_incognito_;

  DISALLOW_COPY_AND_ASSIGN(InstantPage);
};

#endif  
