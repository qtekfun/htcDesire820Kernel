// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WIN8_METRO_DRIVER_CHROME_APP_VIEW_ASH_H_
#define WIN8_METRO_DRIVER_CHROME_APP_VIEW_ASH_H_

#include <windows.applicationmodel.core.h>
#include <windows.ui.core.h>
#include <windows.ui.input.h>
#include <windows.ui.viewmanagement.h>

#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/strings/string16.h"
#include "ui/events/event_constants.h"
#include "win8/metro_driver/direct3d_helper.h"
#include "win8/metro_driver/ime/ime_popup_observer.h"
#include "win8/metro_driver/ime/input_source_observer.h"
#include "win8/metro_driver/ime/text_service_delegate.h"

namespace base {
class FilePath;
}

namespace IPC {
class Listener;
class ChannelProxy;
}

namespace metro_driver {
class InputSource;
class TextService;
}

namespace metro_viewer {
struct CharacterBounds;
struct UnderlineInfo;
}

class OpenFilePickerSession;
class SaveFilePickerSession;
class FolderPickerSession;
class FilePickerSessionBase;

struct MetroViewerHostMsg_SaveAsDialogParams;

class ChromeAppViewAsh
    : public mswr::RuntimeClass<winapp::Core::IFrameworkView>,
      public metro_driver::ImePopupObserver,
      public metro_driver::InputSourceObserver,
      public metro_driver::TextServiceDelegate {
 public:
  ChromeAppViewAsh();
  ~ChromeAppViewAsh();

  
  IFACEMETHOD(Initialize)(winapp::Core::ICoreApplicationView* view);
  IFACEMETHOD(SetWindow)(winui::Core::ICoreWindow* window);
  IFACEMETHOD(Load)(HSTRING entryPoint);
  IFACEMETHOD(Run)();
  IFACEMETHOD(Uninitialize)();

  
  
  static HRESULT Unsnap();

  void OnActivateDesktop(const base::FilePath& file_path, bool ash_exit);
  void OnOpenURLOnDesktop(const base::FilePath& shortcut, const string16& url);
  void OnSetCursor(HCURSOR cursor);
  void OnDisplayFileOpenDialog(const string16& title,
                               const string16& filter,
                               const base::FilePath& default_path,
                               bool allow_multiple_files);
  void OnDisplayFileSaveAsDialog(
      const MetroViewerHostMsg_SaveAsDialogParams& params);
  void OnDisplayFolderPicker(const string16& title);
  void OnSetCursorPos(int x, int y);

  
  
  
  
  void OnOpenFileCompleted(OpenFilePickerSession* open_file_picker,
                           bool success);

  
  
  
  
  void OnSaveFileCompleted(SaveFilePickerSession* save_file_picker,
                           bool success);

  
  
  
  
  void OnFolderPickerCompleted(FolderPickerSession* folder_picker,
                               bool success);

  void OnImeCancelComposition();
  void OnImeUpdateTextInputClient(
      const std::vector<int32>& input_scopes,
      const std::vector<metro_viewer::CharacterBounds>& character_bounds);

  HWND core_window_hwnd() const { return  core_window_hwnd_; }


 private:
  
  virtual void OnImePopupChanged(ImePopupObserver::EventType event) OVERRIDE;

  
  virtual void OnInputSourceChanged() OVERRIDE;

  
  virtual void OnCompositionChanged(
      const string16& text,
      int32 selection_start,
      int32 selection_end,
      const std::vector<metro_viewer::UnderlineInfo>& underlines) OVERRIDE;
  virtual void OnTextCommitted(const string16& text) OVERRIDE;

  HRESULT OnActivate(winapp::Core::ICoreApplicationView* view,
                     winapp::Activation::IActivatedEventArgs* args);

  HRESULT OnPointerMoved(winui::Core::ICoreWindow* sender,
                         winui::Core::IPointerEventArgs* args);

  HRESULT OnPointerPressed(winui::Core::ICoreWindow* sender,
                           winui::Core::IPointerEventArgs* args);

  HRESULT OnPointerReleased(winui::Core::ICoreWindow* sender,
                            winui::Core::IPointerEventArgs* args);

  HRESULT OnWheel(winui::Core::ICoreWindow* sender,
                  winui::Core::IPointerEventArgs* args);

  HRESULT OnKeyDown(winui::Core::ICoreWindow* sender,
                    winui::Core::IKeyEventArgs* args);

  HRESULT OnKeyUp(winui::Core::ICoreWindow* sender,
                  winui::Core::IKeyEventArgs* args);

  
  HRESULT OnAcceleratorKeyDown(winui::Core::ICoreDispatcher* sender,
                               winui::Core::IAcceleratorKeyEventArgs* args);

  HRESULT OnCharacterReceived(winui::Core::ICoreWindow* sender,
                              winui::Core::ICharacterReceivedEventArgs* args);

  HRESULT OnWindowActivated(winui::Core::ICoreWindow* sender,
                            winui::Core::IWindowActivatedEventArgs* args);

  
  HRESULT HandleSearchRequest(winapp::Activation::IActivatedEventArgs* args);
  
  HRESULT HandleProtocolRequest(winapp::Activation::IActivatedEventArgs* args);

  HRESULT OnEdgeGestureCompleted(winui::Input::IEdgeGesture* gesture,
                                 winui::Input::IEdgeGestureEventArgs* args);

  
  
  void OnSearchRequest(const string16& search_string);
  void OnNavigateToUrl(const string16& url);

  HRESULT OnSizeChanged(winui::Core::ICoreWindow* sender,
                        winui::Core::IWindowSizeChangedEventArgs* args);

  mswr::ComPtr<winui::Core::ICoreWindow> window_;
  mswr::ComPtr<winapp::Core::ICoreApplicationView> view_;
  EventRegistrationToken activated_token_;
  EventRegistrationToken pointermoved_token_;
  EventRegistrationToken pointerpressed_token_;
  EventRegistrationToken pointerreleased_token_;
  EventRegistrationToken wheel_token_;
  EventRegistrationToken keydown_token_;
  EventRegistrationToken keyup_token_;
  EventRegistrationToken character_received_token_;
  EventRegistrationToken accel_keydown_token_;
  EventRegistrationToken accel_keyup_token_;
  EventRegistrationToken window_activated_token_;
  EventRegistrationToken sizechange_token_;
  EventRegistrationToken edgeevent_token_;

  
  
  ui::EventFlags mouse_down_flags_;

  
  metro_driver::Direct3DHelper direct3d_helper_;

  
  IPC::ChannelProxy* ui_channel_;

  
  HWND core_window_hwnd_;

  
  base::MessageLoop ui_loop_;

  
  scoped_ptr<metro_driver::InputSource> input_source_;
  scoped_ptr<metro_driver::TextService> text_service_;
};

#endif  
