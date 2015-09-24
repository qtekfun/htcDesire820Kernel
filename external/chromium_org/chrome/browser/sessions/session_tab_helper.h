// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SESSIONS_SESSION_TAB_HELPER_H_
#define CHROME_BROWSER_SESSIONS_SESSION_TAB_HELPER_H_

#include "base/basictypes.h"
#include "chrome/browser/sessions/session_id.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

class SessionTabHelper : public content::WebContentsObserver,
                         public content::WebContentsUserData<SessionTabHelper> {
 public:
  virtual ~SessionTabHelper();

  
  const SessionID& session_id() const { return session_id_; }

  
  void SetWindowID(const SessionID& id);
  const SessionID& window_id() const { return window_id_; }

  
  virtual void RenderViewCreated(
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void UserAgentOverrideSet(const std::string& user_agent) OVERRIDE;

 private:
  explicit SessionTabHelper(content::WebContents* contents);
  friend class content::WebContentsUserData<SessionTabHelper>;

  
  
  
  const SessionID session_id_;

  
  SessionID window_id_;

  DISALLOW_COPY_AND_ASSIGN(SessionTabHelper);
};

#endif  
