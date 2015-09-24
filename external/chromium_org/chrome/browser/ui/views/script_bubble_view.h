// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_SCRIPT_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_SCRIPT_BUBBLE_VIEW_H_

#include <map>
#include <string>

#include "base/compiler_specific.h"
#include "ui/gfx/image/image.h"
#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/controls/link_listener.h"

namespace content {
class WebContents;
}

namespace extensions {
class ScriptBubbleController;
}

namespace views {
class ImageView;
}

class ScriptBubbleView : public views::BubbleDelegateView,
                         public views::LinkListener,
                         public base::SupportsWeakPtr<ScriptBubbleView> {
 public:
  ScriptBubbleView(views::View* anchor_view,
                   content::WebContents* web_contents);
  virtual ~ScriptBubbleView();

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;

  
  virtual void LinkClicked(views::Link* source, int event_flags) OVERRIDE;

 private:
  struct ScriptEntry {
    ScriptEntry();

    std::string extension_id;
    base::string16 extension_name;
    views::ImageView* extension_imageview;
  };

  
  virtual void Init() OVERRIDE;

  
  void OnImageLoaded(size_t index, const gfx::Image& image);

  
  extensions::ScriptBubbleController* GetScriptBubbleController();

  
  int height_;

  
  content::WebContents* web_contents_;

  
  std::vector<ScriptEntry> entries_;

  DISALLOW_COPY_AND_ASSIGN(ScriptBubbleView);
};

#endif  
