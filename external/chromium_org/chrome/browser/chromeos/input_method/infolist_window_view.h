// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_INPUT_METHOD_INFOLIST_WINDOW_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_INPUT_METHOD_INFOLIST_WINDOW_VIEW_H_

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "ui/views/view.h"

namespace gfx {
class Font;
}

namespace chromeos {
namespace input_method {

class InfolistEntryView;

class InfolistWindowView : public views::View {
 public:
  
  struct Entry {
    std::string title;
    std::string body;
  };

  InfolistWindowView();
  virtual ~InfolistWindowView();
  void Init();

  
  
  void Relayout(const std::vector<Entry>& entries, size_t focused_index);

  
  static const size_t InvalidFocusIndex();

 private:
  
  
  views::View* infolist_area_;

  
  
  ScopedVector<InfolistEntryView> infolist_views_;

  
  scoped_ptr<gfx::Font> title_font_;

  
  scoped_ptr<gfx::Font> description_font_;

  DISALLOW_COPY_AND_ASSIGN(InfolistWindowView);
};

}  
}  

#endif  
