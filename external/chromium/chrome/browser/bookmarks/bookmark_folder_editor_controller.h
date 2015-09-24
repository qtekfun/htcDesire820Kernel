// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_FOLDER_EDITOR_CONTROLLER_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_FOLDER_EDITOR_CONTROLLER_H_
#pragma once

#include "base/basictypes.h"
#include "chrome/browser/bookmarks/base_bookmark_model_observer.h"
#include "chrome/browser/ui/input_window_dialog.h"
#include "ui/gfx/native_widget_types.h"

class Profile;

class BookmarkFolderEditorController : public InputWindowDialog::Delegate,
                                       public BaseBookmarkModelObserver {
 public:
  enum Type {
    NEW_BOOKMARK,  
    EXISTING_BOOKMARK,  
  };

  virtual ~BookmarkFolderEditorController();

  static void Show(Profile* profile,
                   gfx::NativeWindow wnd,
                   const BookmarkNode* node,
                   int index,
                   Type type);

 private:
  BookmarkFolderEditorController(Profile* profile,
                                 gfx::NativeWindow wnd,
                                 const BookmarkNode* node,
                                 int index,
                                 Type type);

  
  virtual bool IsValid(const std::wstring& text);
  virtual void InputAccepted(const std::wstring& text);
  virtual void InputCanceled();

  
  virtual void BookmarkModelChanged();
  virtual void BookmarkModelBeingDeleted(BookmarkModel* model);

  Profile* profile_;

  BookmarkModel* model_;

  
  
  const BookmarkNode* node_;

  
  int index_;

  const bool is_new_;

  InputWindowDialog* dialog_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkFolderEditorController);
};

#endif  
