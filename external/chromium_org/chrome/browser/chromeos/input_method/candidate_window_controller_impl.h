// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_INPUT_METHOD_CANDIDATE_WINDOW_CONTROLLER_IMPL_H_
#define CHROME_BROWSER_CHROMEOS_INPUT_METHOD_CANDIDATE_WINDOW_CONTROLLER_IMPL_H_

#include "chrome/browser/chromeos/input_method/candidate_window_controller.h"

#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "chrome/browser/chromeos/input_method/candidate_window_view.h"
#include "chrome/browser/chromeos/input_method/infolist_window_view.h"
#include "ui/base/ime/chromeos/ibus_bridge.h"

namespace views {
class Widget;
}  

namespace chromeos {
namespace input_method {

class CandidateWindow;
class DelayableWidget;
class ModeIndicatorController;

class CandidateWindowControllerImpl
    : public CandidateWindowController,
      public CandidateWindowView::Observer,
      public IBusPanelCandidateWindowHandlerInterface {
 public:
  CandidateWindowControllerImpl();
  virtual ~CandidateWindowControllerImpl();

  
  virtual void AddObserver(
      CandidateWindowController::Observer* observer) OVERRIDE;
  virtual void RemoveObserver(
      CandidateWindowController::Observer* observer) OVERRIDE;
  virtual void Hide() OVERRIDE;

 protected:
  
  
  
  
  
  
  
  static gfx::Point GetInfolistWindowPosition(
      const gfx::Rect& candidate_window_rect,
      const gfx::Rect& screen_rect,
      const gfx::Size& infolist_winodw_size);

  
  
  static void ConvertLookupTableToInfolistEntry(
      const CandidateWindow& candidate_window,
      std::vector<InfolistWindowView::Entry>* infolist_entries,
      size_t* focused_index);

  
  
  static bool ShouldUpdateInfolist(
      const std::vector<InfolistWindowView::Entry>& old_entries,
      size_t old_focused_index,
      const std::vector<InfolistWindowView::Entry>& new_entries,
      size_t new_focused_index);

 private:
  
  virtual void OnCandidateCommitted(int index) OVERRIDE;
  virtual void OnCandidateWindowOpened() OVERRIDE;
  virtual void OnCandidateWindowClosed() OVERRIDE;

  
  void CreateView();

  
  virtual void SetCursorBounds(const gfx::Rect& cursor_bounds,
                               const gfx::Rect& composition_head) OVERRIDE;
  virtual void UpdateAuxiliaryText(const std::string& utf8_text,
                                   bool visible) OVERRIDE;
  virtual void UpdateLookupTable(const CandidateWindow& candidate_window,
                                 bool visible) OVERRIDE;
  virtual void UpdatePreeditText(const std::string& utf8_text,
                                 unsigned int cursor, bool visible) OVERRIDE;
  virtual void FocusStateChanged(bool is_focused) OVERRIDE;

  
  
  void UpdateInfolistBounds();

  
  CandidateWindowView* candidate_window_view_;

  
  
  scoped_ptr<views::Widget> frame_;

  
  
  scoped_ptr<DelayableWidget> infolist_window_;

  
  scoped_ptr<ModeIndicatorController> mode_indicator_controller_;

  
  
  std::vector<InfolistWindowView::Entry> latest_infolist_entries_;
  size_t latest_infolist_focused_index_;

  ObserverList<CandidateWindowController::Observer> observers_;

  DISALLOW_COPY_AND_ASSIGN(CandidateWindowControllerImpl);
};

#endif  

}  
}  
