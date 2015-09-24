// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_SEARCH_BOX_MODEL_H_
#define UI_APP_LIST_SEARCH_BOX_MODEL_H_

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "base/strings/string16.h"
#include "ui/app_list/app_list_export.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/selection_model.h"

namespace app_list {

class SearchBoxModelObserver;

class APP_LIST_EXPORT SearchBoxModel {
 public:
  
  struct APP_LIST_EXPORT ButtonProperty {
    ButtonProperty(const gfx::ImageSkia& icon, const base::string16& tooltip);
    ~ButtonProperty();

    gfx::ImageSkia icon;
    base::string16 tooltip;
  };

  SearchBoxModel();
  ~SearchBoxModel();

  
  void SetIcon(const gfx::ImageSkia& icon);
  const gfx::ImageSkia& icon() const { return icon_; }

  
  void SetSpeechRecognitionButton(scoped_ptr<ButtonProperty> speech_button);
  const ButtonProperty* speech_button() const { return speech_button_.get(); }

  
  void SetHintText(const base::string16& hint_text);
  const base::string16& hint_text() const { return hint_text_; }

  
  void SetSelectionModel(const gfx::SelectionModel& sel);
  const gfx::SelectionModel& selection_model() const {
    return selection_model_;
  }

  
  void SetText(const base::string16& text);
  const base::string16& text() const { return text_; }

  void AddObserver(SearchBoxModelObserver* observer);
  void RemoveObserver(SearchBoxModelObserver* observer);

 private:
  gfx::ImageSkia icon_;
  scoped_ptr<ButtonProperty> speech_button_;
  base::string16 hint_text_;
  gfx::SelectionModel selection_model_;
  base::string16 text_;

  ObserverList<SearchBoxModelObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(SearchBoxModel);
};

}  

#endif  
