// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_KEYBOARD_KEYBOARD_CONTROLLER_PROXY_H_
#define UI_KEYBOARD_KEYBOARD_CONTROLLER_PROXY_H_

#include "base/memory/scoped_ptr.h"
#include "content/public/common/media_stream_request.h"
#include "ui/base/ime/text_input_type.h"
#include "ui/keyboard/keyboard_export.h"

namespace aura {
class Window;
}
namespace content {
class BrowserContext;
class SiteInstance;
class WebContents;
}
namespace gfx {
class Rect;
}
namespace ui {
class InputMethod;
}

namespace keyboard {

class KEYBOARD_EXPORT KeyboardControllerProxy {
 public:
  KeyboardControllerProxy();
  virtual ~KeyboardControllerProxy();

  
  
  virtual aura::Window* GetKeyboardWindow();

  
  void SetOverrideContentUrl(const GURL& url);

  
  bool resizing_from_contents() const { return resizing_from_contents_; }

  
  
  void set_resizing_from_contents(bool resizing) {
    resizing_from_contents_ = resizing;
  }

  
  
  virtual ui::InputMethod* GetInputMethod() = 0;

  
  virtual void RequestAudioInput(content::WebContents* web_contents,
      const content::MediaStreamRequest& request,
      const content::MediaResponseCallback& callback) = 0;

  
  
  
  virtual void ShowKeyboardContainer(aura::Window* container);

  
  
  
  virtual void HideKeyboardContainer(aura::Window* container);

  
  
  
  virtual void SetUpdateInputType(ui::TextInputType type);

 protected:
  
  
  virtual content::BrowserContext* GetBrowserContext() = 0;

  
  
  
  
  virtual void SetupWebContents(content::WebContents* contents);

 private:
  
  void ReloadContents();

  
  const GURL& GetValidUrl();

  const GURL default_url_;
  GURL override_url_;

  scoped_ptr<content::WebContents> keyboard_contents_;

  
  bool resizing_from_contents_;

  DISALLOW_COPY_AND_ASSIGN(KeyboardControllerProxy);
};

}  

#endif  
