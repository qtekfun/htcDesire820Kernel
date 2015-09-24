// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_NOTIFICATIONS_BALLOON_H_
#define CHROME_BROWSER_NOTIFICATIONS_BALLOON_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"

class Balloon;
class BalloonCollection;
class BalloonHost;
class Notification;
class Profile;

class BalloonView {
 public:
  virtual ~BalloonView() {}

  
  virtual void Show(Balloon* balloon) = 0;

  
  virtual void Update() = 0;

  
  virtual void RepositionToBalloon() = 0;

  
  virtual void Close(bool by_user) = 0;

  
  virtual gfx::Size GetSize() const = 0;

  
  
  virtual BalloonHost* GetHost() const = 0;

  
  static int GetHorizontalMargin();
};

class Balloon {
 public:
  Balloon(const Notification& notification,
          Profile* profile,
          BalloonCollection* collection);
  virtual ~Balloon();

  const Notification& notification() const { return *notification_.get(); }
  Profile* profile() const { return profile_; }

  gfx::Point GetPosition() const {
    return position_ + offset_;
  }
  void SetPosition(const gfx::Point& upper_left, bool reposition);

  const gfx::Vector2d& offset() const { return offset_; }
  void set_offset(const gfx::Vector2d& offset) { offset_ = offset; }
  void add_offset(const gfx::Vector2d& offset) { offset_.Add(offset); }

  const gfx::Size& content_size() const { return content_size_; }
  void set_content_size(const gfx::Size& size) { content_size_ = size; }

  const BalloonCollection* collection() const { return collection_; }

  const gfx::Size& min_scrollbar_size() const { return min_scrollbar_size_; }
  void set_min_scrollbar_size(const gfx::Size& size) {
    min_scrollbar_size_ = size;
  }

  
  
  
  void ResizeDueToAutoResize(const gfx::Size& size);

  
  void set_view(BalloonView* balloon_view);

  
  BalloonView* balloon_view() const { return balloon_view_.get(); }

  
  gfx::Size GetViewSize() const { return balloon_view_->GetSize(); }

  
  virtual void Show();

  
  virtual void Update(const Notification& notification);

  
  virtual void OnClick();

  
  virtual void OnButtonClick(int button_index);

  
  
  virtual void OnClose(bool by_user);

  
  virtual void CloseByScript();

  
  std::string GetExtensionId();

 private:
  
  Profile* profile_;

  
  scoped_ptr<Notification> notification_;

  
  BalloonCollection* collection_;

  
  scoped_ptr<BalloonView> balloon_view_;

  
  gfx::Point position_;
  gfx::Size content_size_;

  
  
  gfx::Vector2d offset_;

  
  gfx::Size min_scrollbar_size_;

  DISALLOW_COPY_AND_ASSIGN(Balloon);
};

#endif  
