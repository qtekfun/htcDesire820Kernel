// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_NOTIFICATIONS_BALLOON_COLLECTION_H_
#define CHROME_BROWSER_NOTIFICATIONS_BALLOON_COLLECTION_H_

#include <deque>
#include <string>

#include "base/callback.h"

class Balloon;
class GURL;
class Notification;
class Profile;

namespace gfx {
class Size;
}

class BalloonCollection {
 public:
  class BalloonSpaceChangeListener {
   public:
    virtual ~BalloonSpaceChangeListener() {}

    
    
    virtual void OnBalloonSpaceChanged() = 0;
  };

  
  
  enum PositionPreference {
    UPPER_RIGHT        = 0,
    LOWER_RIGHT        = 1,
    UPPER_LEFT         = 2,
    LOWER_LEFT         = 3,

    
    DEFAULT_POSITION   = -1
  };

  static BalloonCollection* Create();

  BalloonCollection();

  virtual ~BalloonCollection();

  
  virtual void Add(const Notification& notification,
                   Profile* profile) = 0;

  
  virtual const Notification* FindById(const std::string& id) const = 0;

  
  
  virtual bool RemoveById(const std::string& id) = 0;

  
  
  virtual bool RemoveBySourceOrigin(const GURL& source_origin) = 0;

  
  virtual bool RemoveByProfile(Profile* profile) = 0;

  
  virtual void RemoveAll() = 0;

  
  virtual bool HasSpace() const = 0;

  
  virtual void ResizeBalloon(Balloon* balloon, const gfx::Size& size) = 0;

  
  virtual void SetPositionPreference(PositionPreference position) = 0;

  
  virtual void DisplayChanged() = 0;

  
  virtual void OnBalloonClosed(Balloon* source) = 0;

  
  typedef std::deque<Balloon*> Balloons;
  virtual const Balloons& GetActiveBalloons() = 0;

  BalloonSpaceChangeListener* space_change_listener() {
    return space_change_listener_;
  }
  void set_space_change_listener(BalloonSpaceChangeListener* listener) {
    space_change_listener_ = listener;
  }

  void set_on_collection_changed_callback(const base::Closure& callback) {
    on_collection_changed_callback_ = callback;
  }

 protected:
  
  BalloonSpaceChangeListener* space_change_listener_;

  
  
  base::Closure on_collection_changed_callback_;
};

#endif  
