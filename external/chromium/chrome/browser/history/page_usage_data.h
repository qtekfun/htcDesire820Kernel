// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_PAGE_USAGE_DATA_H__
#define CHROME_BROWSER_HISTORY_PAGE_USAGE_DATA_H__
#pragma once

#include "base/string16.h"
#include "chrome/browser/history/history_types.h"
#include "googleurl/src/gurl.h"

class SkBitmap;

class PageUsageData {
 public:
  explicit PageUsageData(history::URLID id);

  virtual ~PageUsageData();

  
  history::URLID GetID() const {
    return id_;
  }

  void SetURL(const GURL& url) {
    url_ = url;
  }

  const GURL& GetURL() const {
    return url_;
  }

  void SetTitle(const string16& s) {
    title_ = s;
  }

  const string16& GetTitle() const {
    return title_;
  }

  void SetScore(double v) {
    score_ = v;
  }

  double GetScore() const {
    return score_;
  }

  void SetThumbnailMissing() {
    thumbnail_set_ = true;
  }

  void SetThumbnail(SkBitmap* img);

  bool HasThumbnail() const {
    return thumbnail_set_;
  }

  const SkBitmap* GetThumbnail() const {
    return thumbnail_;
  }

  bool thumbnail_pending() const {
    return thumbnail_pending_;
  }

  void set_thumbnail_pending(bool pending) {
    thumbnail_pending_ = pending;
  }

  void SetFaviconMissing() {
    favicon_set_ = true;
  }

  void SetFavicon(SkBitmap* img);

  bool HasFavicon() const {
    return favicon_set_;
  }

  bool favicon_pending() const {
    return favicon_pending_;
  }

  void set_favicon_pending(bool pending) {
    favicon_pending_ = pending;
  }

  const SkBitmap* GetFavicon() const {
    return favicon_;
  }

  
  static bool Predicate(const PageUsageData* dud1,
                        const PageUsageData* dud2);

 private:
  history::URLID id_;
  GURL url_;
  string16 title_;

  SkBitmap* thumbnail_;
  bool thumbnail_set_;
  
  bool thumbnail_pending_;

  SkBitmap* favicon_;
  bool favicon_set_;
  
  bool favicon_pending_;

  double score_;
};

#endif  