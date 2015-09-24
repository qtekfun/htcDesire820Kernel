// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_COOKIE_INFO_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_COOKIE_INFO_VIEW_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "ui/views/view.h"

namespace views {
class GridLayout;
class Label;
class Textfield;
}

namespace net {
class CanonicalCookie;
}

class CookieInfoView : public views::View {
 public:
  CookieInfoView();
  virtual ~CookieInfoView();

  
  void SetCookie(const std::string& domain,
                 const net::CanonicalCookie& cookie_node);

  
  void SetCookieString(const GURL& url, const std::string& cookie_line);

  
  
  void ClearCookieDisplay();

  
  void EnableCookieDisplay(bool enabled);

 protected:
  
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;

 private:
  
  void AddLabelRow(int layout_id, views::GridLayout* layout,
                   views::Label* label, views::Textfield* value);

  
  void Init();

  
  views::Label* name_label_;
  views::Textfield* name_value_field_;
  views::Label* content_label_;
  views::Textfield* content_value_field_;
  views::Label* domain_label_;
  views::Textfield* domain_value_field_;
  views::Label* path_label_;
  views::Textfield* path_value_field_;
  views::Label* send_for_label_;
  views::Textfield* send_for_value_field_;
  views::Label* created_label_;
  views::Textfield* created_value_field_;
  views::Label* expires_label_;
  views::Textfield* expires_value_field_;

  DISALLOW_COPY_AND_ASSIGN(CookieInfoView);
};

#endif  
