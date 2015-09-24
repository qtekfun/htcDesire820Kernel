// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_NOTIFICATIONS_BALLOON_COLLECTION_BASE_H_
#define CHROME_BROWSER_NOTIFICATIONS_BALLOON_COLLECTION_BASE_H_
#pragma once

#include <deque>
#include <string>

#include "base/basictypes.h"

class Balloon;
class GURL;
class Notification;

class BalloonCollectionBase {
 public:
  BalloonCollectionBase();
  virtual ~BalloonCollectionBase();

  typedef std::deque<Balloon*> Balloons;

  
  virtual void Add(Balloon* balloon);

  
  
  virtual void Remove(Balloon* balloon);

  
  
  
  virtual bool CloseById(const std::string& id);

  
  
  
  virtual bool CloseAllBySourceOrigin(const GURL& source_origin);

  
  virtual void CloseAll();

  const Balloons& balloons() const { return balloons_; }

  
  
  Balloon* FindBalloon(const Notification& notification);

  
  int count() const { return static_cast<int>(balloons_.size()); }

 private:
  
  Balloons balloons_;

  DISALLOW_COPY_AND_ASSIGN(BalloonCollectionBase);
};

#endif  
