// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_DISPLAY_DISPLAY_MANAGER_H_
#define ASH_DISPLAY_DISPLAY_MANAGER_H_

#include <string>
#include <vector>

#include "ash/ash_export.h"
#include "ash/display/display_info.h"
#include "ash/display/display_layout.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "ui/gfx/display.h"

#if defined(OS_CHROMEOS)
#include "chromeos/display/output_configurator.h"
#endif

namespace gfx {
class Display;
class Insets;
class Rect;
}

namespace ash {
class AcceleratorControllerTest;
class DisplayController;

namespace test {
class DisplayManagerTestApi;
class SystemGestureEventFilterTest;
}
namespace internal {
class DisplayLayoutStore;

class ASH_EXPORT DisplayManager
#if defined(OS_CHROMEOS)
    : public chromeos::OutputConfigurator::SoftwareMirroringController
#endif
      {
 public:
  class ASH_EXPORT Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual void CreateOrUpdateNonDesktopDisplay(
        const DisplayInfo& display_info) = 0;

    
    virtual void CloseNonDesktopDisplay() = 0;

    
    
    
    virtual void PreDisplayConfigurationChange(bool clear_focus) = 0;
    virtual void PostDisplayConfigurationChange() = 0;
  };

  
  
  
  
  
  
  
  enum SecondDisplayMode {
    EXTENDED,
    MIRRORING,
    VIRTUAL_KEYBOARD
  };

  
  static std::vector<float> GetScalesForDisplay(const DisplayInfo& info);

  
  static float GetNextUIScale(const DisplayInfo& info, bool up);

  
  
  static void UpdateDisplayBoundsForLayoutById(
      const DisplayLayout& layout,
      const gfx::Display& primary_display,
      int64 secondary_display_id);

  DisplayManager();
  virtual ~DisplayManager();

  DisplayLayoutStore* layout_store() {
    return layout_store_.get();
  }

  void set_delegate(Delegate* delegate) { delegate_ = delegate; }

  
  
  
  void set_force_bounds_changed(bool force_bounds_changed) {
    force_bounds_changed_ = force_bounds_changed;
  }

  
  int64 first_display_id() const { return first_display_id_; }

  
  
  bool InitFromCommandLine();

  
  void InitDefaultDisplay();

  
  bool IsActiveDisplay(const gfx::Display& display) const;

  
  bool HasInternalDisplay() const;

  bool IsInternalDisplayId(int64 id) const;

  
  DisplayLayout GetCurrentDisplayLayout();

  
  DisplayIdPair GetCurrentDisplayIdPair() const;

  
  
  void SetLayoutForCurrentDisplays(
      const DisplayLayout& layout_relative_to_primary);

  
  const gfx::Display& GetDisplayForId(int64 id) const;

  
  
  
  const gfx::Display& FindDisplayContainingPoint(
      const gfx::Point& point_in_screen) const;

  
  bool UpdateWorkAreaOfDisplay(int64 display_id, const gfx::Insets& insets);

  
  
  
  void SetOverscanInsets(int64 display_id, const gfx::Insets& insets_in_dip);

  
  void SetDisplayRotation(int64 display_id, gfx::Display::Rotation rotation);

  
  void SetDisplayUIScale(int64 display_id, float ui_scale);

  
  void SetDisplayResolution(int64 display_id, const gfx::Size& resolution);

  
  
  void RegisterDisplayProperty(int64 display_id,
                               gfx::Display::Rotation rotation,
                               float ui_scale,
                               const gfx::Insets* overscan_insets,
                               const gfx::Size& resolution_in_pixels);

  
  bool GetSelectedResolutionForDisplayId(int64 display_id,
                                         gfx::Size* resolution_out) const;

  
  bool IsDisplayUIScalingEnabled() const;

  
  
  
  gfx::Insets GetOverscanInsets(int64 display_id) const;

  
  
  
  void OnNativeDisplaysChanged(
      const std::vector<DisplayInfo>& display_info_list);

  
  void UpdateDisplays(const std::vector<DisplayInfo>& display_info_list);

  
  void UpdateDisplays();

  
  
  const gfx::Display& GetDisplayAt(size_t index) const;

  const gfx::Display& GetPrimaryDisplayCandidate() const;

  
  
  size_t GetNumDisplays() const;

  const std::vector<gfx::Display>& displays() const { return displays_; }

  
  
  size_t num_connected_displays() const { return num_connected_displays_; }

  
  bool IsMirrored() const;
  int64 mirrored_display_id() const { return mirrored_display_id_; }

  
  const gfx::Display& non_desktop_display() const {
    return non_desktop_display_;
  }

  
  const DisplayInfo& GetDisplayInfo(int64 display_id) const;

  
  std::string GetDisplayNameForId(int64 id);

  
  
  
  
  int64 GetDisplayIdForUIScaling() const;

  
  void SetMirrorMode(bool mirrored);

  
  
  void AddRemoveDisplay();
  void ToggleDisplayScaleFactor();

  
#if defined(OS_CHROMEOS)
  virtual void SetSoftwareMirroring(bool enabled) OVERRIDE;
#endif
  bool software_mirroring_enabled() const {
    return second_display_mode_ == MIRRORING;
  };

  bool virtual_keyboard_root_window_enabled() const {
    return second_display_mode_ == VIRTUAL_KEYBOARD;
  };

  
  void SetSecondDisplayMode(SecondDisplayMode mode);
  SecondDisplayMode second_display_mode() const {
    return second_display_mode_;
  }

  
  bool UpdateDisplayBounds(int64 display_id,
                           const gfx::Rect& new_bounds);

  
  
  void CreateMirrorWindowIfAny();

private:
  FRIEND_TEST_ALL_PREFIXES(ExtendedDesktopTest, ConvertPoint);
  FRIEND_TEST_ALL_PREFIXES(DisplayManagerTest, TestNativeDisplaysChanged);
  FRIEND_TEST_ALL_PREFIXES(DisplayManagerTest,
                           NativeDisplaysChangedAfterPrimaryChange);
  FRIEND_TEST_ALL_PREFIXES(DisplayManagerTest, AutomaticOverscanInsets);
  friend class ash::AcceleratorControllerTest;
  friend class test::DisplayManagerTestApi;
  friend class test::SystemGestureEventFilterTest;
  friend class DisplayManagerTest;

  typedef std::vector<gfx::Display> DisplayList;

  void set_change_display_upon_host_resize(bool value) {
    change_display_upon_host_resize_ = value;
  }

  gfx::Display* FindDisplayForId(int64 id);

  
  
  void AddMirrorDisplayInfoIfAny(std::vector<DisplayInfo>* display_info_list);

  
  
  
  
  
  void InsertAndUpdateDisplayInfo(const DisplayInfo& new_info);

  
  gfx::Display CreateDisplayFromDisplayInfoById(int64 display_id);

  
  
  
  
  
  bool UpdateSecondaryDisplayBoundsForLayout(DisplayList* display_list,
                                             size_t* updated_index) const;

  static void UpdateDisplayBoundsForLayout(
      const DisplayLayout& layout,
      const gfx::Display& primary_display,
      gfx::Display* secondary_display);

  Delegate* delegate_;  

  scoped_ptr<DisplayLayoutStore> layout_store_;

  int64 first_display_id_;

  
  DisplayList displays_;

  int num_connected_displays_;

  bool force_bounds_changed_;

  
  std::map<int64, DisplayInfo> display_info_;

  
  std::map<int64, gfx::Size> resolutions_;

  
  
  
  
  
  bool change_display_upon_host_resize_;

  SecondDisplayMode second_display_mode_;
  int64 mirrored_display_id_;
  gfx::Display non_desktop_display_;

  DISALLOW_COPY_AND_ASSIGN(DisplayManager);
};

}  
}  

#endif  
