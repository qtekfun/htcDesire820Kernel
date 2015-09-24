// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_SEARCH_RESULT_H_
#define UI_APP_LIST_SEARCH_RESULT_H_

#include <vector>

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "base/strings/string16.h"
#include "ui/app_list/app_list_export.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/range/range.h"

namespace ui {
class MenuModel;
}

namespace app_list {

class SearchResultObserver;

class APP_LIST_EXPORT SearchResult {
 public:
  
  struct APP_LIST_EXPORT Tag {
    
    
    enum Style {
      NONE  = 0,
      URL   = 1 << 0,
      MATCH = 1 << 1,
      DIM   = 1 << 2,
    };

    Tag(int styles, size_t start, size_t end)
        : styles(styles),
          range(start, end) {
    }

    int styles;
    gfx::Range range;
  };
  typedef std::vector<Tag> Tags;

  
  
  
  
  struct APP_LIST_EXPORT Action {
    Action(const gfx::ImageSkia& base_image,
           const gfx::ImageSkia& hover_image,
           const gfx::ImageSkia& pressed_image,
           const base::string16& tooltip_text);
    Action(const base::string16& label_text,
           const base::string16& tooltip_text);
    ~Action();

    gfx::ImageSkia base_image;
    gfx::ImageSkia hover_image;
    gfx::ImageSkia pressed_image;

    base::string16 tooltip_text;
    base::string16 label_text;
  };
  typedef std::vector<Action> Actions;

  SearchResult();
  virtual ~SearchResult();

  const gfx::ImageSkia& icon() const { return icon_; }
  void SetIcon(const gfx::ImageSkia& icon);

  const base::string16& title() const { return title_; }
  void set_title(const base::string16& title) { title_ = title;}

  const Tags& title_tags() const { return title_tags_; }
  void set_title_tags(const Tags& tags) { title_tags_ = tags; }

  const base::string16& details() const { return details_; }
  void set_details(const base::string16& details) { details_ = details; }

  const Tags& details_tags() const { return details_tags_; }
  void set_details_tags(const Tags& tags) { details_tags_ = tags; }

  const Actions& actions() const {
    return actions_;
  }
  void SetActions(const Actions& sets);

  bool is_installing() const { return is_installing_; }
  void SetIsInstalling(bool is_installing);

  int percent_downloaded() const { return percent_downloaded_; }
  void SetPercentDownloaded(int percent_downloaded);

  void NotifyItemInstalled();
  void NotifyItemUninstalled();

  void AddObserver(SearchResultObserver* observer);
  void RemoveObserver(SearchResultObserver* observer);

  
  
  
  virtual ui::MenuModel* GetContextMenuModel();

 private:
  gfx::ImageSkia icon_;

  base::string16 title_;
  Tags title_tags_;

  base::string16 details_;
  Tags details_tags_;

  Actions actions_;

  bool is_installing_;
  int percent_downloaded_;

  ObserverList<SearchResultObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(SearchResult);
};

}  

#endif  
