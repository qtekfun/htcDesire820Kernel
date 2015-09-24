// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_OMNIBOX_ALTERNATE_NAV_INFOBAR_DELEGATE_H_
#define CHROME_BROWSER_UI_OMNIBOX_ALTERNATE_NAV_INFOBAR_DELEGATE_H_

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "chrome/browser/infobars/infobar_delegate.h"

class AlternateNavInfoBarDelegate : public InfoBarDelegate {
 public:
  virtual ~AlternateNavInfoBarDelegate();

  
  
  static void Create(content::WebContents* web_contents,
                     const base::string16& text,
                     const AutocompleteMatch& match,
                     const GURL& search_url);

  base::string16 GetMessageTextWithOffset(size_t* link_offset) const;
  base::string16 GetLinkText() const;
  bool LinkClicked(WindowOpenDisposition disposition);

 private:
  AlternateNavInfoBarDelegate(Profile* profile,
                              const base::string16& text,
                              const AutocompleteMatch& match,
                              const GURL& search_url);

  
  static scoped_ptr<InfoBar> CreateInfoBar(
      scoped_ptr<AlternateNavInfoBarDelegate> delegate);

  
  virtual int GetIconID() const OVERRIDE;
  virtual Type GetInfoBarType() const OVERRIDE;

  Profile* profile_;
  const base::string16 text_;
  const AutocompleteMatch match_;
  const GURL search_url_;

  DISALLOW_COPY_AND_ASSIGN(AlternateNavInfoBarDelegate);
};

#endif  
