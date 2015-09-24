// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_NATIVE_THEME_NATIVE_THEME_WIN_H_
#define UI_NATIVE_THEME_NATIVE_THEME_WIN_H_


#include <map>

#include <windows.h>
#include <uxtheme.h>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/size.h"
#include "ui/gfx/sys_color_change_listener.h"
#include "ui/native_theme/native_theme.h"

class SkCanvas;

namespace ui {

class NATIVE_THEME_EXPORT NativeThemeWin : public NativeTheme,
                                           public gfx::SysColorChangeListener {
 public:
  enum ThemeName {
    BUTTON,
    LIST,
    MENU,
    MENULIST,
    SCROLLBAR,
    STATUS,
    TAB,
    TEXTFIELD,
    TRACKBAR,
    WINDOW,
    PROGRESS,
    SPIN,
    LAST
  };

  bool IsThemingActive() const;

  HRESULT GetThemeColor(ThemeName theme,
                        int part_id,
                        int state_id,
                        int prop_id,
                        SkColor* color) const;

  
  
  
  SkColor GetThemeColorWithDefault(ThemeName theme,
                                   int part_id,
                                   int state_id,
                                   int prop_id,
                                   int default_sys_color) const;

  
  
  
  gfx::Size GetThemeBorderSize(ThemeName theme) const;

  
  
  
  
  
  void DisableTheming() const;

  
  
  void CloseHandles() const;

  
  bool IsClassicTheme(ThemeName name) const;

  
  static NativeThemeWin* instance();

  HRESULT PaintTextField(HDC hdc,
                         int part_id,
                         int state_id,
                         int classic_state,
                         RECT* rect,
                         COLORREF color,
                         bool fill_content_area,
                         bool draw_edges) const;

  
  virtual gfx::Size GetPartSize(Part part,
                                State state,
                                const ExtraParams& extra) const OVERRIDE;
  virtual void Paint(SkCanvas* canvas,
                     Part part,
                     State state,
                     const gfx::Rect& rect,
                     const ExtraParams& extra) const OVERRIDE;
  virtual SkColor GetSystemColor(ColorId color_id) const OVERRIDE;

 private:
  NativeThemeWin();
  ~NativeThemeWin();

  
  virtual void OnSysColorChange() OVERRIDE;

  
  void UpdateSystemColors();

  
  void PaintDirect(SkCanvas* canvas,
                   Part part,
                   State state,
                   const gfx::Rect& rect,
                   const ExtraParams& extra) const;

  
  
  
  void PaintIndirect(SkCanvas* canvas,
                     Part part,
                     State state,
                     const gfx::Rect& rect,
                     const ExtraParams& extra) const;

  HRESULT GetThemePartSize(ThemeName themeName,
                           HDC hdc,
                           int part_id,
                           int state_id,
                           RECT* rect,
                           int ts,
                           SIZE* size) const;

  HRESULT PaintButton(HDC hdc,
                      State state,
                      const ButtonExtraParams& extra,
                      int part_id,
                      int state_id,
                      RECT* rect) const;

  HRESULT PaintMenuSeparator(HDC hdc,
                             const gfx::Rect& rect,
                             const MenuSeparatorExtraParams& extra) const;

  HRESULT PaintMenuGutter(HDC hdc, const gfx::Rect& rect) const;

  
  
  
  HRESULT PaintMenuArrow(HDC hdc,
                         State state,
                         const gfx::Rect& rect,
                         const MenuArrowExtraParams& extra) const;

  HRESULT PaintMenuBackground(HDC hdc, const gfx::Rect& rect) const;

  HRESULT PaintMenuCheck(HDC hdc,
                         State state,
                         const gfx::Rect& rect,
                         const MenuCheckExtraParams& extra) const;

  HRESULT PaintMenuCheckBackground(HDC hdc,
                                   State state,
                                   const gfx::Rect& rect) const;

  HRESULT PaintMenuItemBackground(HDC hdc,
                                  State state,
                                  const gfx::Rect& rect,
                                  const MenuItemExtraParams& extra) const;

  HRESULT PaintPushButton(HDC hdc,
                          Part part,
                          State state,
                          const gfx::Rect& rect,
                          const ButtonExtraParams& extra) const;

  HRESULT PaintRadioButton(HDC hdc,
                           Part part,
                           State state,
                           const gfx::Rect& rect,
                           const ButtonExtraParams& extra) const;

  HRESULT PaintCheckbox(HDC hdc,
                        Part part,
                        State state,
                        const gfx::Rect& rect,
                        const ButtonExtraParams& extra) const;

  HRESULT PaintMenuList(HDC hdc,
                        State state,
                        const gfx::Rect& rect,
                        const MenuListExtraParams& extra) const;

  
  
  HRESULT PaintScrollbarArrow(HDC hdc,
                              Part part,
                              State state,
                              const gfx::Rect& rect,
                              const ScrollbarArrowExtraParams& extra) const;

  HRESULT PaintScrollbarThumb(HDC hdc,
                              Part part,
                              State state,
                              const gfx::Rect& rect,
                              const ScrollbarThumbExtraParams& extra) const;

  
  
  
  
  HRESULT PaintScrollbarTrack(SkCanvas* canvas,
                              HDC hdc,
                              Part part,
                              State state,
                              const gfx::Rect& rect,
                              const ScrollbarTrackExtraParams& extra) const;

  HRESULT PaintSpinButton(HDC hdc,
                          Part part,
                          State state,
                          const gfx::Rect& rect,
                          const InnerSpinButtonExtraParams& extra) const;

  HRESULT PaintTrackbar(SkCanvas* canvas,
                        HDC hdc,
                        Part part,
                        State state,
                        const gfx::Rect& rect,
                        const TrackbarExtraParams& extra) const;

  HRESULT PaintProgressBar(HDC hdc,
                           const gfx::Rect& rect,
                           const ProgressBarExtraParams& extra) const;

  HRESULT PaintWindowResizeGripper(HDC hdc, const gfx::Rect& rect) const;

  HRESULT PaintTabPanelBackground(HDC hdc, const gfx::Rect& rect) const;

  HRESULT PaintTextField(HDC hdc,
                         Part part,
                         State state,
                         const gfx::Rect& rect,
                         const TextFieldExtraParams& extra) const;

  
  
  
  
  
  HRESULT PaintScaledTheme(HANDLE theme,
                           HDC hdc,
                           int part_id,
                           int state_id,
                           const gfx::Rect& rect) const;

  
  
  
  
  static ThemeName GetThemeName(Part part);
  static int GetWindowsPart(Part part, State state, const ExtraParams& extra);
  static int GetWindowsState(Part part, State state, const ExtraParams& extra);

  HRESULT GetThemeInt(ThemeName theme,
                      int part_id,
                      int state_id,
                      int prop_id,
                      int *result) const;

  HRESULT PaintFrameControl(HDC hdc,
                            const gfx::Rect& rect,
                            UINT type,
                            UINT state,
                            bool is_selected,
                            State control_state) const;

  
  HANDLE GetThemeHandle(ThemeName theme_name) const;

  typedef HRESULT (WINAPI* DrawThemeBackgroundPtr)(HANDLE theme,
                                                   HDC hdc,
                                                   int part_id,
                                                   int state_id,
                                                   const RECT* rect,
                                                   const RECT* clip_rect);
  typedef HRESULT (WINAPI* DrawThemeBackgroundExPtr)(HANDLE theme,
                                                     HDC hdc,
                                                     int part_id,
                                                     int state_id,
                                                     const RECT* rect,
                                                     const DTBGOPTS* opts);
  typedef HRESULT (WINAPI* GetThemeColorPtr)(HANDLE hTheme,
                                             int part_id,
                                             int state_id,
                                             int prop_id,
                                             COLORREF* color);
  typedef HRESULT (WINAPI* GetThemeContentRectPtr)(HANDLE hTheme,
                                                   HDC hdc,
                                                   int part_id,
                                                   int state_id,
                                                   const RECT* rect,
                                                   RECT* content_rect);
  typedef HRESULT (WINAPI* GetThemePartSizePtr)(HANDLE hTheme,
                                                HDC hdc,
                                                int part_id,
                                                int state_id,
                                                RECT* rect,
                                                int ts,
                                                SIZE* size);
  typedef HANDLE (WINAPI* OpenThemeDataPtr)(HWND window,
                                            LPCWSTR class_list);
  typedef HRESULT (WINAPI* CloseThemeDataPtr)(HANDLE theme);

  typedef void (WINAPI* SetThemeAppPropertiesPtr) (DWORD flags);
  typedef BOOL (WINAPI* IsThemeActivePtr)();
  typedef HRESULT (WINAPI* GetThemeIntPtr)(HANDLE hTheme,
                                           int part_id,
                                           int state_id,
                                           int prop_id,
                                           int *value);

  
  DrawThemeBackgroundPtr draw_theme_;
  DrawThemeBackgroundExPtr draw_theme_ex_;
  GetThemeColorPtr get_theme_color_;
  GetThemeContentRectPtr get_theme_content_rect_;
  GetThemePartSizePtr get_theme_part_size_;
  OpenThemeDataPtr open_theme_;
  CloseThemeDataPtr close_theme_;
  SetThemeAppPropertiesPtr set_theme_properties_;
  IsThemeActivePtr is_theme_active_;
  GetThemeIntPtr get_theme_int_;

  
  HMODULE theme_dll_;

  
  mutable HANDLE theme_handles_[LAST];

  
  gfx::ScopedSysColorChangeListener color_change_listener_;
  mutable std::map<int, SkColor> system_colors_;

  DISALLOW_COPY_AND_ASSIGN(NativeThemeWin);
};

}  

#endif  
