// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_INFOBARS_INFOBAR_CONTAINER_GTK_H_
#define CHROME_BROWSER_UI_GTK_INFOBARS_INFOBAR_CONTAINER_GTK_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/infobars/infobar_container.h"
#include "ui/base/gtk/owned_widget_gtk.h"

class InfoBar;
class InfoBarGtk;
class InfoBarDelegate;
class Profile;

namespace gfx {
class Rect;
}

typedef struct _GdkColor GdkColor;
typedef struct _GdkEventExpose GdkEventExpose;
typedef struct _GtkWidget GtkWidget;

class InfoBarContainerGtk : public InfoBarContainer {
 public:
  InfoBarContainerGtk(InfoBarContainer::Delegate* delegate,
                      Profile* profile);
  virtual ~InfoBarContainerGtk();

  
  GtkWidget* widget() const { return container_.get(); }

  
  
  
  void RemoveDelegate(InfoBarDelegate* delegate);

  
  
  int TotalHeightOfAnimatingBars() const;

  
  bool ContainsInfobars() const;

  
  
  
  
  void PaintInfobarBitsOn(GtkWidget* widget,
                          GdkEventExpose* expose,
                          InfoBarGtk* infobar);

 protected:
  
  virtual void PlatformSpecificAddInfoBar(InfoBar* infobar,
                                          size_t position) OVERRIDE;
  virtual void PlatformSpecificRemoveInfoBar(InfoBar* infobar) OVERRIDE;
  virtual void PlatformSpecificInfoBarStateChanged(bool is_animating) OVERRIDE;

 private:
  
  void PaintArrowOn(GtkWidget* widget,
                    GdkEventExpose* expose,
                    const gfx::Rect& bounds,
                    InfoBarGtk* source);

  
  Profile* profile_;

  
  
  std::vector<InfoBarGtk*> infobars_gtk_;

  
  ui::OwnedWidgetGtk container_;

  DISALLOW_COPY_AND_ASSIGN(InfoBarContainerGtk);
};

#endif  
