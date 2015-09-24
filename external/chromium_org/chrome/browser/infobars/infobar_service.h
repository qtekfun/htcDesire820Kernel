// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INFOBARS_INFOBAR_SERVICE_H_
#define CHROME_BROWSER_INFOBARS_INFOBAR_SERVICE_H_

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

class InfoBar;

class InfoBarService : public content::WebContentsObserver,
                       public content::WebContentsUserData<InfoBarService> {
 public:
  
  void set_infobars_enabled(bool enabled) { infobars_enabled_ = enabled; }

  
  
  
  
  
  
  
  
  virtual InfoBar* AddInfoBar(scoped_ptr<InfoBar> infobar);

  
  
  
  
  
  
  void RemoveInfoBar(InfoBar* infobar);

  
  
  
  
  
  
  
  
  
  InfoBar* ReplaceInfoBar(InfoBar* old_infobar,
                          scoped_ptr<InfoBar> new_infobar);

  
  size_t infobar_count() const { return infobars_.size(); }

  
  
  
  
  InfoBar* infobar_at(size_t index) { return infobars_[index]; }

  
  content::WebContents* web_contents() {
    return content::WebContentsObserver::web_contents();
  }

 private:
  friend class content::WebContentsUserData<InfoBarService>;

  
  
  
  
  
  typedef std::vector<InfoBar*> InfoBars;

  explicit InfoBarService(content::WebContents* web_contents);
  virtual ~InfoBarService();

  
  virtual void RenderProcessGone(base::TerminationStatus status) OVERRIDE;
  virtual void NavigationEntryCommitted(
      const content::LoadCommittedDetails& load_details) OVERRIDE;
  virtual void WebContentsDestroyed(
      content::WebContents* web_contents) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  void RemoveInfoBarInternal(InfoBar* infobar, bool animate);
  void RemoveAllInfoBars(bool animate);

  
  void OnDidBlockDisplayingInsecureContent();
  void OnDidBlockRunningInsecureContent();

  InfoBars infobars_;
  bool infobars_enabled_;

  DISALLOW_COPY_AND_ASSIGN(InfoBarService);
};

#endif  
