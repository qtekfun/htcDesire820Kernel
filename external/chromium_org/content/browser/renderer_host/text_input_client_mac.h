// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_TEXT_INPUT_CLIENT_MAC_H_
#define CONTENT_BROWSER_RENDERER_HOST_TEXT_INPUT_CLIENT_MAC_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "base/synchronization/condition_variable.h"
#include "base/synchronization/lock.h"
#include "content/common/content_export.h"
#include "ui/gfx/point.h"

template <typename T> struct DefaultSingletonTraits;

namespace content {
class RenderWidgetHost;

class CONTENT_EXPORT TextInputClientMac {
 public:
  
  static TextInputClientMac* GetInstance();

  
  
  
  
  
  
  
  
  
  NSUInteger GetCharacterIndexAtPoint(RenderWidgetHost* rwh, gfx::Point point);
  
  NSAttributedString* GetAttributedSubstringFromRange(
      RenderWidgetHost* rwh, NSRange range);
  
  
  NSRect GetFirstRectForRange(RenderWidgetHost* rwh, NSRange range);

  
  
  
  void SetCharacterIndexAndSignal(NSUInteger index);
  void SetFirstRectAndSignal(NSRect first_rect);
  void SetSubstringAndSignal(NSAttributedString* string);

 private:
  friend struct DefaultSingletonTraits<TextInputClientMac>;
  TextInputClientMac();
  ~TextInputClientMac();

  
  
  
  
  void BeforeRequest();
  
  
  void AfterRequest();

  NSUInteger character_index_;
  NSRect first_rect_;
  base::scoped_nsobject<NSAttributedString> substring_;

  base::Lock lock_;
  base::ConditionVariable condition_;

  DISALLOW_COPY_AND_ASSIGN(TextInputClientMac);
};

}  

#endif  
