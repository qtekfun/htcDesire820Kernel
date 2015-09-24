// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_SPELLING_MENU_OBSERVER_H_
#define CHROME_BROWSER_TAB_CONTENTS_SPELLING_MENU_OBSERVER_H_

#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_member.h"
#include "base/strings/string16.h"
#include "base/timer/timer.h"
#include "chrome/browser/spellchecker/spelling_service_client.h"
#include "chrome/browser/tab_contents/render_view_context_menu_observer.h"

class RenderViewContextMenuProxy;
struct SpellCheckResult;

class SpellingMenuObserver : public RenderViewContextMenuObserver {
 public:
  explicit SpellingMenuObserver(RenderViewContextMenuProxy* proxy);
  virtual ~SpellingMenuObserver();

  
  virtual void InitMenu(const content::ContextMenuParams& params) OVERRIDE;
  virtual bool IsCommandIdSupported(int command_id) OVERRIDE;
  virtual bool IsCommandIdChecked(int command_id) OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) OVERRIDE;
  virtual void ExecuteCommand(int command_id) OVERRIDE;
  virtual void OnMenuCancel() OVERRIDE;

  
  
  void OnTextCheckComplete(
      SpellingServiceClient::ServiceType type,
      bool success,
      const base::string16& text,
      const std::vector<SpellCheckResult>& results);

 private:
  
  
  void OnAnimationTimerExpired();

  
  
  RenderViewContextMenuProxy* proxy_;

  
  
  
  std::vector<base::string16> suggestions_;

  
  
  
  base::string16 loading_message_;
  size_t loading_frame_;

  
  
  
  
  bool succeeded_;

  
  
  base::string16 misspelled_word_;

  
  
  uint32 misspelling_hash_;

  
  
  
  
  
  
  base::string16 result_;

  
  scoped_ptr<SpellingServiceClient> client_;

  
  base::RepeatingTimer<SpellingMenuObserver> animation_timer_;

  
  
  
  BooleanPrefMember integrate_spelling_service_;

  
  BooleanPrefMember autocorrect_spelling_;

  DISALLOW_COPY_AND_ASSIGN(SpellingMenuObserver);
};

#endif  
