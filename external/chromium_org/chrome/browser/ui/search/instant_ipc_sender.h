// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SEARCH_INSTANT_IPC_SENDER_H_
#define CHROME_BROWSER_UI_SEARCH_INSTANT_IPC_SENDER_H_

#include "base/memory/scoped_ptr.h"
#include "chrome/common/omnibox_focus_state.h"
#include "content/public/browser/web_contents_observer.h"

namespace gfx {
class Rect;
}

namespace IPC {
class Sender;
}

class InstantIPCSender : public content::WebContentsObserver {
 public:
  
  
  static scoped_ptr<InstantIPCSender> Create(bool is_incognito);

  virtual ~InstantIPCSender() {}

  
  void SetContents(content::WebContents* web_contents);


  
  
  virtual void SetOmniboxBounds(const gfx::Rect& bounds) {}

  
  virtual void FocusChanged(OmniboxFocusState state,
                            OmniboxFocusChangeReason reason) {}

  
  virtual void SetInputInProgress(bool input_in_progress) {}

 protected:
  InstantIPCSender() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(InstantIPCSender);
};

#endif  
