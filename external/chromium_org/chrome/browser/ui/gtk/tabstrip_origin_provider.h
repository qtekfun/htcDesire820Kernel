// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_TABSTRIP_ORIGIN_PROVIDER_H_
#define CHROME_BROWSER_UI_GTK_TABSTRIP_ORIGIN_PROVIDER_H_

namespace gfx{
class Point;
}

class TabstripOriginProvider {
 public:
  virtual ~TabstripOriginProvider() { }

  
  
  
  
  
  
  
  virtual gfx::Point GetTabStripOriginForWidget(GtkWidget* widget) = 0;
};

#endif  
