// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_NATIVE_DESKTOP_MEDIA_LIST_H_
#define CHROME_BROWSER_MEDIA_NATIVE_DESKTOP_MEDIA_LIST_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/sequenced_task_runner.h"
#include "chrome/browser/media/desktop_media_list.h"
#include "content/public/browser/desktop_media_id.h"
#include "ui/gfx/image/image_skia.h"

namespace webrtc {
class ScreenCapturer;
class WindowCapturer;
}

class NativeDesktopMediaList : public DesktopMediaList {
 public:
  
  
  
  NativeDesktopMediaList(
      scoped_ptr<webrtc::ScreenCapturer> screen_capturer,
      scoped_ptr<webrtc::WindowCapturer> window_capturer);
  virtual ~NativeDesktopMediaList();

  
  virtual void SetUpdatePeriod(base::TimeDelta period) OVERRIDE;
  virtual void SetThumbnailSize(const gfx::Size& thumbnail_size) OVERRIDE;
  virtual void StartUpdating(DesktopMediaListObserver* observer) OVERRIDE;
  virtual int GetSourceCount() const OVERRIDE;
  virtual const Source& GetSource(int index) const OVERRIDE;
  virtual void SetViewDialogWindowId(
      content::DesktopMediaID::Id dialog_id) OVERRIDE;

 private:
  class Worker;
  friend class Worker;

  
  struct SourceDescription {
    SourceDescription(content::DesktopMediaID id, const base::string16& name);

    content::DesktopMediaID id;
    base::string16 name;
  };

  
  static bool CompareSources(const SourceDescription& a,
                             const SourceDescription& b);

  
  void Refresh();

  
  
  
  
  void OnSourcesList(const std::vector<SourceDescription>& sources);
  void OnSourceThumbnail(int index, const gfx::ImageSkia& thumbnail);
  void OnRefreshFinished();

  
  scoped_ptr<webrtc::ScreenCapturer> screen_capturer_;
  scoped_ptr<webrtc::WindowCapturer> window_capturer_;

  
  base::TimeDelta update_period_;

  
  gfx::Size thumbnail_size_;

  
  content::DesktopMediaID::Id view_dialog_id_;

  
  DesktopMediaListObserver* observer_;

  
  scoped_refptr<base::SequencedTaskRunner> capture_task_runner_;

  
  
  
  scoped_ptr<Worker> worker_;

  
  std::vector<Source> sources_;

  base::WeakPtrFactory<NativeDesktopMediaList> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(NativeDesktopMediaList);
};

#endif  
