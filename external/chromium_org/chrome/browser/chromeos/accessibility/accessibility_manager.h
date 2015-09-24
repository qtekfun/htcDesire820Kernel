// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_ACCESSIBILITY_ACCESSIBILITY_MANAGER_H_
#define CHROME_BROWSER_CHROMEOS_ACCESSIBILITY_ACCESSIBILITY_MANAGER_H_

#include <set>

#include "ash/accessibility_delegate.h"
#include "ash/session_state_observer.h"
#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/time/time.h"
#include "chrome/browser/chromeos/accessibility/accessibility_util.h"
#include "chrome/browser/extensions/api/braille_display_private/braille_controller.h"
#include "chrome/browser/extensions/extension_system.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "extensions/browser/event_router.h"

class Profile;

namespace chromeos {

struct AccessibilityStatusEventDetails {
  AccessibilityStatusEventDetails(
      bool enabled,
      ash::AccessibilityNotificationVisibility notify);

  AccessibilityStatusEventDetails(
      bool enabled,
      ash::MagnifierType magnifier_type,
      ash::AccessibilityNotificationVisibility notify);

  bool enabled;
  ash::MagnifierType magnifier_type;
  ash::AccessibilityNotificationVisibility notify;
};

class AccessibilityManager : public content::NotificationObserver,
    extensions::api::braille_display_private::BrailleObserver,
    public ash::SessionStateObserver {
 public:
  
  
  static void Initialize();
  
  static void Shutdown();
  
  static AccessibilityManager* Get();

  
  
  
  
  class PrefHandler {
   public:
    explicit PrefHandler(const char* pref_path);
    virtual ~PrefHandler();

    
    void HandleProfileChanged(Profile* previous_profile,
                              Profile* current_profile);

   private:
    const char* pref_path_;
  };

  
  bool ShouldShowAccessibilityMenu();

  
  void EnableLargeCursor(bool enabled);
  
  bool IsLargeCursorEnabled();

  
  void EnableStickyKeys(bool enabled);

  
  bool IsIncognitoAllowed();

  
  bool IsStickyKeysEnabled();

  
  
  void EnableSpokenFeedback(bool enabled,
                            ash::AccessibilityNotificationVisibility notify);

  
  bool IsSpokenFeedbackEnabled();

  
  void ToggleSpokenFeedback(ash::AccessibilityNotificationVisibility notify);

  
  void EnableHighContrast(bool enabled);

  
  bool IsHighContrastEnabled();

  
  void EnableAutoclick(bool enabled);

  
  bool IsAutoclickEnabled();

  
  void SetAutoclickDelay(int delay_ms);

  
  int GetAutoclickDelay() const;

  
  virtual void ActiveUserChanged(const std::string& user_id) OVERRIDE;

  void SetProfileForTest(Profile* profile);

  static void SetBrailleControllerForTest(
      extensions::api::braille_display_private::BrailleController* controller);

  
  void EnableSystemSounds(bool system_sounds_enabled);

  
  base::TimeDelta PlayShutdownSound();

 protected:
  AccessibilityManager();
  virtual ~AccessibilityManager();

 private:
  void LoadChromeVox();
  void LoadChromeVoxToUserScreen();
  void LoadChromeVoxToLockScreen();
  void UnloadChromeVox();
  void UnloadChromeVoxFromLockScreen();
  void PostLoadChromeVox(Profile* profile);
  void PostUnloadChromeVox(Profile* profile);

  void UpdateLargeCursorFromPref();
  void UpdateStickyKeysFromPref();
  void UpdateSpokenFeedbackFromPref();
  void UpdateHighContrastFromPref();
  void UpdateAutoclickFromPref();
  void UpdateAutoclickDelayFromPref();
  void LocalePrefChanged();

  void CheckBrailleState();
  void ReceiveBrailleDisplayState(
      scoped_ptr<extensions::api::braille_display_private::DisplayState> state);


  void SetProfile(Profile* profile);

  void UpdateChromeOSAccessibilityHistograms();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  virtual void OnDisplayStateChanged(
      const extensions::api::braille_display_private::DisplayState&
          display_state) OVERRIDE;

  
  
  
  void PlaySound(int sound_key) const;

  
  Profile* profile_;

  
  
  bool chrome_vox_loaded_on_lock_screen_;
  bool chrome_vox_loaded_on_user_screen_;

  content::NotificationRegistrar notification_registrar_;
  scoped_ptr<PrefChangeRegistrar> pref_change_registrar_;
  scoped_ptr<PrefChangeRegistrar> local_state_pref_change_registrar_;
  scoped_ptr<ash::ScopedSessionStateObserver> session_state_observer_;

  PrefHandler large_cursor_pref_handler_;
  PrefHandler spoken_feedback_pref_handler_;
  PrefHandler high_contrast_pref_handler_;
  PrefHandler autoclick_pref_handler_;
  PrefHandler autoclick_delay_pref_handler_;

  bool large_cursor_enabled_;
  bool sticky_keys_enabled_;
  bool spoken_feedback_enabled_;
  bool high_contrast_enabled_;
  bool autoclick_enabled_;
  int autoclick_delay_ms_;

  ash::AccessibilityNotificationVisibility spoken_feedback_notification_;

  base::WeakPtrFactory<AccessibilityManager> weak_ptr_factory_;

  bool should_speak_chrome_vox_announcements_on_user_screen_;

  bool system_sounds_enabled_;

  DISALLOW_COPY_AND_ASSIGN(AccessibilityManager);
};

}  

#endif  
