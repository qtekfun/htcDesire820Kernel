// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_REMOTE_ROOT_WINDOW_HOST_WIN_H_
#define UI_AURA_REMOTE_ROOT_WINDOW_HOST_WIN_H_

#include <vector>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "ui/aura/window_tree_host.h"
#include "ui/base/ime/remote_input_method_delegate_win.h"
#include "ui/events/event.h"
#include "ui/events/event_constants.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/metro_viewer/ime_types.h"

namespace base {
class FilePath;
}

namespace ui {
class RemoteInputMethodPrivateWin;
class ViewProp;
}

namespace IPC {
class Message;
class Sender;
}

namespace aura {

typedef base::Callback<void(const base::FilePath&, int, void*)>
    OpenFileCompletion;

typedef base::Callback<void(const std::vector<base::FilePath>&, void*)>
    OpenMultipleFilesCompletion;

typedef base::Callback<void(const base::FilePath&, int, void*)>
    SaveFileCompletion;

typedef base::Callback<void(const base::FilePath&, int, void*)>
    SelectFolderCompletion;

typedef base::Callback<void(void*)> FileSelectionCanceled;

AURA_EXPORT void HandleOpenFile(const base::string16& title,
                                const base::FilePath& default_path,
                                const base::string16& filter,
                                const OpenFileCompletion& on_success,
                                const FileSelectionCanceled& on_failure);

AURA_EXPORT void HandleOpenMultipleFiles(
    const base::string16& title,
    const base::FilePath& default_path,
    const base::string16& filter,
    const OpenMultipleFilesCompletion& on_success,
    const FileSelectionCanceled& on_failure);

AURA_EXPORT void HandleSaveFile(const base::string16& title,
                                const base::FilePath& default_path,
                                const base::string16& filter,
                                int filter_index,
                                const base::string16& default_extension,
                                const SaveFileCompletion& on_success,
                                const FileSelectionCanceled& on_failure);

AURA_EXPORT void HandleSelectFolder(const base::string16& title,
                                    const SelectFolderCompletion& on_success,
                                    const FileSelectionCanceled& on_failure);

AURA_EXPORT void HandleActivateDesktop(
    const base::FilePath& shortcut,
    bool ash_exit);

AURA_EXPORT void HandleMetroExit();

class AURA_EXPORT RemoteRootWindowHostWin
    : public RootWindowHost,
      public ui::internal::RemoteInputMethodDelegateWin {
 public:
  
  
  static RemoteRootWindowHostWin* Instance();
  static RemoteRootWindowHostWin* Create(const gfx::Rect& bounds);

  
  
  
  void Connected(IPC::Sender* host, HWND remote_window);
  
  void Disconnected();

  
  bool OnMessageReceived(const IPC::Message& message);

  void HandleOpenURLOnDesktop(const base::FilePath& shortcut,
                              const base::string16& url);

  
  
  void HandleActivateDesktop(const base::FilePath& shortcut, bool ash_exit);

  
  void HandleMetroExit();

  void HandleOpenFile(const base::string16& title,
                      const base::FilePath& default_path,
                      const base::string16& filter,
                      const OpenFileCompletion& on_success,
                      const FileSelectionCanceled& on_failure);

  void HandleOpenMultipleFiles(const base::string16& title,
                               const base::FilePath& default_path,
                               const base::string16& filter,
                               const OpenMultipleFilesCompletion& on_success,
                               const FileSelectionCanceled& on_failure);

  void HandleSaveFile(const base::string16& title,
                      const base::FilePath& default_path,
                      const base::string16& filter,
                      int filter_index,
                      const base::string16& default_extension,
                      const SaveFileCompletion& on_success,
                      const FileSelectionCanceled& on_failure);

  void HandleSelectFolder(const base::string16& title,
                          const SelectFolderCompletion& on_success,
                          const FileSelectionCanceled& on_failure);

  void HandleWindowSizeChanged(uint32 width, uint32 height);

  
  Window* GetAshWindow();

  
  
  bool IsForegroundWindow();

 private:
  explicit RemoteRootWindowHostWin(const gfx::Rect& bounds);
  virtual ~RemoteRootWindowHostWin();

  
  void OnMouseMoved(int32 x, int32 y, int32 flags);
  void OnMouseButton(int32 x,
                     int32 y,
                     int32 extra,
                     ui::EventType type,
                     ui::EventFlags flags);
  void OnKeyDown(uint32 vkey,
                 uint32 repeat_count,
                 uint32 scan_code,
                 uint32 flags);
  void OnKeyUp(uint32 vkey,
               uint32 repeat_count,
               uint32 scan_code,
               uint32 flags);
  void OnChar(uint32 key_code,
              uint32 repeat_count,
              uint32 scan_code,
              uint32 flags);
  void OnWindowActivated();
  void OnTouchDown(int32 x, int32 y, uint64 timestamp, uint32 pointer_id);
  void OnTouchUp(int32 x, int32 y, uint64 timestamp, uint32 pointer_id);
  void OnTouchMoved(int32 x, int32 y, uint64 timestamp, uint32 pointer_id);
  void OnFileSaveAsDone(bool success,
                        const base::FilePath& filename,
                        int filter_index);
  void OnFileOpenDone(bool success, const base::FilePath& filename);
  void OnMultiFileOpenDone(bool success,
                           const std::vector<base::FilePath>& files);
  void OnSelectFolderDone(bool success, const base::FilePath& folder);
  void OnSetCursorPosAck();

  
  ui::RemoteInputMethodPrivateWin* GetRemoteInputMethodPrivate();
  void OnImeCandidatePopupChanged(bool visible);
  void OnImeCompositionChanged(
      const string16& text,
      int32 selection_start,
      int32 selection_end,
      const std::vector<metro_viewer::UnderlineInfo>& underlines);
  void OnImeTextCommitted(const string16& text);
  void OnImeInputSourceChanged(uint16 language_id, bool is_ime);

  
  virtual RootWindow* GetRootWindow() OVERRIDE;
  virtual gfx::AcceleratedWidget GetAcceleratedWidget() OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void ToggleFullScreen() OVERRIDE;
  virtual gfx::Rect GetBounds() const OVERRIDE;
  virtual void SetBounds(const gfx::Rect& bounds) OVERRIDE;
  virtual gfx::Insets GetInsets() const OVERRIDE;
  virtual void SetInsets(const gfx::Insets& insets) OVERRIDE;
  virtual gfx::Point GetLocationOnNativeScreen() const OVERRIDE;
  virtual void SetCapture() OVERRIDE;
  virtual void ReleaseCapture() OVERRIDE;
  virtual void SetCursor(gfx::NativeCursor cursor) OVERRIDE;
  virtual bool QueryMouseLocation(gfx::Point* location_return) OVERRIDE;
  virtual bool ConfineCursorToRootWindow() OVERRIDE;
  virtual void UnConfineCursor() OVERRIDE;
  virtual void OnCursorVisibilityChanged(bool show) OVERRIDE;
  virtual void MoveCursorTo(const gfx::Point& location) OVERRIDE;
  virtual void PostNativeEvent(const base::NativeEvent& native_event) OVERRIDE;
  virtual void OnDeviceScaleFactorChanged(float device_scale_factor) OVERRIDE;
  virtual void PrepareForShutdown() OVERRIDE;

  
  virtual void CancelComposition() OVERRIDE;
  virtual void OnTextInputClientUpdated(
      const std::vector<int32>& input_scopes,
      const std::vector<gfx::Rect>& composition_character_bounds) OVERRIDE;

  
  
  
  
  
  void DispatchKeyboardMessage(ui::EventType type,
                               uint32 vkey,
                               uint32 repeat_count,
                               uint32 scan_code,
                               uint32 flags,
                               bool is_character);

  
  
  
  void SetEventFlags(uint32 flags);

  uint32 mouse_event_flags() const {
    return event_flags_ & (ui::EF_LEFT_MOUSE_BUTTON |
                           ui::EF_MIDDLE_MOUSE_BUTTON |
                           ui::EF_RIGHT_MOUSE_BUTTON);
  }

  uint32 key_event_flags() const {
    return event_flags_ & (ui::EF_SHIFT_DOWN | ui::EF_CONTROL_DOWN |
                           ui::EF_ALT_DOWN | ui::EF_CAPS_LOCK_DOWN);
  }

  HWND remote_window_;
  IPC::Sender* host_;
  scoped_ptr<ui::ViewProp> prop_;

  
  
  OpenFileCompletion file_open_completion_callback_;
  OpenMultipleFilesCompletion multi_file_open_completion_callback_;
  SaveFileCompletion file_saveas_completion_callback_;
  SelectFolderCompletion select_folder_completion_callback_;
  FileSelectionCanceled failure_callback_;

  
  
  bool ignore_mouse_moves_until_set_cursor_ack_;

  
  scoped_ptr<ui::MouseEvent> last_mouse_click_event_;

  
  
  uint32 event_flags_;

  
  gfx::Size window_size_;

  DISALLOW_COPY_AND_ASSIGN(RemoteRootWindowHostWin);
};

}  

#endif  
