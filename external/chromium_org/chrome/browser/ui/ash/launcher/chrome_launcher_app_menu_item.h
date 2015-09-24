// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_ASH_LAUNCHER_CHROME_LAUNCHER_APP_MENU_ITEM_H_
#define CHROME_BROWSER_UI_ASH_LAUNCHER_CHROME_LAUNCHER_APP_MENU_ITEM_H_

#include "base/strings/string16.h"
#include "ui/gfx/image/image.h"

class ChromeLauncherAppMenuItem {
 public:
  
  ChromeLauncherAppMenuItem(const base::string16 title,
                            const gfx::Image* icon,
                            bool has_leading_separator);

  virtual ~ChromeLauncherAppMenuItem();

  
  const base::string16& title() const { return title_; }

  
  const gfx::Image& icon() const { return icon_; }

  
  const bool HasLeadingSeparator() const { return has_leading_separator_; }

  
  virtual bool IsActive() const;

  
  virtual bool IsEnabled() const;

  
  
  
  virtual void Execute(int event_flags);

 private:
  const base::string16 title_;
  const gfx::Image icon_;

  
  const bool has_leading_separator_;

  DISALLOW_COPY_AND_ASSIGN(ChromeLauncherAppMenuItem);
};
#endif  
