// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_BACKGROUND_CONTENTS_H_
#define CHROME_BROWSER_TAB_CONTENTS_BACKGROUND_CONTENTS_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_delegate.h"
#include "content/public/browser/web_contents_observer.h"
#include "ui/base/window_open_disposition.h"

class Profile;

namespace content {
class SessionStorageNamespace;
class SiteInstance;
};

class BackgroundContents : public content::WebContentsDelegate,
                           public content::WebContentsObserver,
                           public content::NotificationObserver {
 public:
  class Delegate {
   public:
    
    
    
    
    virtual void AddWebContents(content::WebContents* new_contents,
                                WindowOpenDisposition disposition,
                                const gfx::Rect& initial_pos,
                                bool user_gesture,
                                bool* was_blocked) = 0;

   protected:
    virtual ~Delegate() {}
  };

  BackgroundContents(
      content::SiteInstance* site_instance,
      int routing_id,
      Delegate* delegate,
      const std::string& partition_id,
      content::SessionStorageNamespace* session_storage_namespace);
  virtual ~BackgroundContents();

  content::WebContents* web_contents() const { return web_contents_.get(); }
  virtual const GURL& GetURL() const;

  
  virtual void CloseContents(content::WebContents* source) OVERRIDE;
  virtual bool ShouldSuppressDialogs() OVERRIDE;
  virtual void DidNavigateMainFramePostCommit(
      content::WebContents* tab) OVERRIDE;
  virtual void AddNewContents(content::WebContents* source,
                              content::WebContents* new_contents,
                              WindowOpenDisposition disposition,
                              const gfx::Rect& initial_pos,
                              bool user_gesture,
                              bool* was_blocked) OVERRIDE;

  
  virtual void RenderProcessGone(base::TerminationStatus status) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 protected:
  
  BackgroundContents();

 private:
  
  Delegate* delegate_;

  Profile* profile_;
  scoped_ptr<content::WebContents> web_contents_;
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(BackgroundContents);
};

struct BackgroundContentsOpenedDetails {
  
  BackgroundContents* contents;

  
  const base::string16& frame_name;

  
  const base::string16& application_id;
};

#endif  
