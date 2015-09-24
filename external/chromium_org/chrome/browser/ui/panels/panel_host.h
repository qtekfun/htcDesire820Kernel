// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_PANELS_PANEL_HOST_H_
#define CHROME_BROWSER_UI_PANELS_PANEL_HOST_H_

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/extensions/extension_function_dispatcher.h"
#include "content/public/browser/web_contents_delegate.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/common/page_zoom.h"

class FaviconTabHelper;
class GURL;
class Panel;
class PrefsTabHelper;
class Profile;

namespace content {
class WebContents;
}

namespace extensions {
class WindowController;
}

namespace gfx {
class Image;
class Rect;
}

class PanelHost : public content::WebContentsDelegate,
                  public content::WebContentsObserver,
                  public ExtensionFunctionDispatcher::Delegate {
 public:
  PanelHost(Panel* panel, Profile* profile);
  virtual ~PanelHost();

  void Init(const GURL& url);
  content::WebContents* web_contents() { return web_contents_.get(); }
  void DestroyWebContents();

  
  gfx::Image GetPageIcon() const;

  
  virtual content::WebContents* OpenURLFromTab(
      content::WebContents* source,
      const content::OpenURLParams& params) OVERRIDE;
  virtual void NavigationStateChanged(const content::WebContents* source,
                                      unsigned changed_flags) OVERRIDE;
  virtual void AddNewContents(content::WebContents* source,
                              content::WebContents* new_contents,
                              WindowOpenDisposition disposition,
                              const gfx::Rect& initial_pos,
                              bool user_gesture,
                              bool* was_blocked) OVERRIDE;
  virtual void ActivateContents(content::WebContents* contents) OVERRIDE;
  virtual void DeactivateContents(content::WebContents* contents) OVERRIDE;
  virtual void LoadingStateChanged(content::WebContents* source) OVERRIDE;
  virtual void CloseContents(content::WebContents* source) OVERRIDE;
  virtual void MoveContents(content::WebContents* source,
                            const gfx::Rect& pos) OVERRIDE;
  virtual bool IsPopupOrPanel(
      const content::WebContents* source) const OVERRIDE;
  virtual void ContentsZoomChange(bool zoom_in) OVERRIDE;
  virtual void HandleKeyboardEvent(
      content::WebContents* source,
      const content::NativeWebKeyboardEvent& event) OVERRIDE;
  virtual void WebContentsFocused(content::WebContents* contents) OVERRIDE;
  virtual void ResizeDueToAutoResize(content::WebContents* web_contents,
                                     const gfx::Size& new_size) OVERRIDE;

  
  virtual void RenderViewCreated(
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void RenderProcessGone(base::TerminationStatus status) OVERRIDE;
  virtual void WebContentsDestroyed(
      content::WebContents* web_contents) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  virtual extensions::WindowController* GetExtensionWindowController() const
      OVERRIDE;
  virtual content::WebContents* GetAssociatedWebContents() const OVERRIDE;

  
  void Reload();
  void ReloadIgnoringCache();
  void StopLoading();
  void Zoom(content::PageZoom zoom);

 private:
  
  void ClosePanel();

  
  void OnRequest(const ExtensionHostMsg_Request_Params& params);

  Panel* panel_;  
  Profile* profile_;
  ExtensionFunctionDispatcher extension_function_dispatcher_;

  scoped_ptr<content::WebContents> web_contents_;

  
  base::WeakPtrFactory<PanelHost> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(PanelHost);
};

#endif  
